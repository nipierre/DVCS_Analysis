

//#include "TargetCoordinates.cc"
#include "TargetCoordinates.h"
#include "plot_target.h"


Double_t sqrt2pi = TMath::Sqrt(TMath::Pi()*2.);

TCanvas c("c","c", 800, 600);

bool debug = false;

bool plot_fits = true;

int diff_rebin = 10;
bool plot_all = 0;
char pdfname[500];

Double_t modulation(Double_t *x,Double_t *par)
{
	Double_t phi = x[0]*TMath::Pi()/180.;
	Double_t val1 = (-1.0)*par[0]*TMath::Cos(phi-par[1]);
	Double_t val2 = par[0]*TMath::Cos(phi-par[1]);
	Double_t val3 = TMath::Sqrt(val2*val2+par[2]*par[2]-par[0]*par[0]);
	return(val1 + val3);
}


TH1D* differentiate_hist(TH1D* h1, int reb)
{
	char tstr[500];
	TH1D* h1_2 = (TH1D*) h1->Clone();
	h1_2->Rebin(reb);
	TH1D* h1_d = (TH1D*) h1_2->Clone();
	sprintf(tstr,"%s diff.",h1_2->GetTitle());
	h1_d->SetTitle(tstr);
	for(int b = 1; b < h1_2->GetXaxis()->GetNbins(); b++) {
		float diff = h1_2->GetBinContent(b+1)-h1_2->GetBinContent(b);
		h1_d->SetBinContent(b, diff);
	}

	TH1D* h1_dd = (TH1D*) h1_d->Clone();
	sprintf(tstr,"%s diff. 2nd",h1_2->GetTitle());
	h1_dd->SetTitle(tstr);
	for(int b = 1; b < h1_d->GetXaxis()->GetNbins(); b++) {
		//float diff = TMath::Abs(h1_d->GetBinContent(b+1)-h1_d->GetBinContent(b));
		float diff = h1_d->GetBinContent(b+1)-h1_d->GetBinContent(b);
		h1_dd->SetBinContent(b, diff);
	}

	delete h1_2;
	delete h1_d;
	return h1_dd;
}


TF1* fit_hist(TH1D* h1, TH1D* h1d, float lastpeak=0)
{
	int binmax = 0;
	int bin1;
	int bin2;
	if( lastpeak > 0 ) {
		bin1 = h1d->GetXaxis()->FindBin(lastpeak-0.3);
		bin2 = h1d->GetXaxis()->FindBin(lastpeak+0.3);
	} else {
		bin1 = h1d->GetXaxis()->FindBin(1);
		bin2 = h1d->GetXaxis()->FindBin(3);
	}
	float max = 0;
	float xpeak = 0;
	for(int i = bin1; i <= bin2; i++) {
		if( h1d->GetBinContent(i) <= max ) continue;
		max = h1d->GetBinContent(i);
		binmax = i;
	}
	if(binmax > 0) xpeak = h1d->GetXaxis()->GetBinCenter(binmax);
	cout<<"Peak position: "<<xpeak<<endl;

	cout<<"Peak search window: "<<xpeak-0.05<<" "<<xpeak+0.5<<endl;
	int binmax2 = h1->GetXaxis()->FindBin(xpeak);
	bin1 = h1->GetXaxis()->FindBin(xpeak-0.05);
	bin2 = h1->GetXaxis()->FindBin(xpeak+0.5);
	max = 0;
	int binmax3 = 0;
	for(int i = bin1; i <= bin2; i++) {
		if( h1->GetBinContent(i) <= max ) continue;
		max = h1->GetBinContent(i);
		binmax3 = i;
	}
	if(binmax3 > 0) xpeak = h1->GetXaxis()->GetBinCenter(binmax3);
	cout<<"Peak position #2: "<<xpeak<<endl;

	TF1* fgaus1 = new TF1("fgaus1","gaus(0)+pol1(3)");
	fgaus1->SetLineColor(2);
	fgaus1->SetLineWidth(1);
	fgaus1->SetParameter(0,100);
	fgaus1->SetParLimits(0,1,1000000000);
	fgaus1->SetParameter(1,xpeak);
	fgaus1->SetParLimits(1,xpeak-0.1,xpeak+0.1);
	fgaus1->SetParameter(2,0.03);
	fgaus1->SetParLimits(2,0.01,0.05);

	h1->Fit("fgaus1","RQN","",xpeak-0.5,xpeak+0.2);
	xpeak = fgaus1->GetParameter(1);
	h1->Fit("fgaus1","RQ","",xpeak-0.5,xpeak+0.2);

	return fgaus1;
}


int fit_slice(TH2F* h2, int D, float* xv, float *yv, float* eyv, TF1* func, TF1* previousfunc, int iter)
{
	int N = 0;
	char tstr[500];
	//  int bin_min =  h2->GetXaxis()->FindBin(30);  2012
	//  int bin_max =  236;							 2012
	//if( func->GetParameter(2) > 0.0001)
	//bin_max =  h2->GetXaxis()->GetNbins();
	//bin_min = 21;

	int bin_min =  h2->GetXaxis()->FindBin(0.);
	int bin_max =  h2->GetXaxis()->FindBin(359);
	if(iter == 0) {
		bin_min =  h2->GetXaxis()->FindBin(20);
		bin_max =  h2->GetXaxis()->FindBin(200);
	}

	float lastpeak = 0;
	for(int bin = bin_min; bin <= bin_max; bin += D) {
		//for(int bin = bin_min; bin <= bin_min+D; bin+=D) {
		int bin1 = bin;
		int bin2 = bin+D-1;
		xv[N] = (h2->GetXaxis()->GetBinCenter(bin1)+h2->GetXaxis()->GetBinCenter(bin2))/2;

		//Zone à exclure pour 2016 :
		//if(xv[N] > 125 &&  xv[N] < 180 ) continue;
		//if(xv[N] > 230 &&  xv[N] < 280 ) continue;

		//Zone à exclure pour 2017:
		if(xv[N] > 150 &&  xv[N] < 300 ) continue;

		//Zone à exclure pour 2017 P00:
//		if(xv[N] < 70 || xv[N] > 270 ) continue;

		sprintf(tstr,"%s_%d",h2->GetName(),bin);
		TH1D* h1 = h2->ProjectionY(tstr,bin1,bin2);
		h1->SetTitle(tstr);
		//h1->Rebin(2);
		//TH1D* h1_2 = h1->Clone();
		//h1_2->Rebin(10);
		TH1D* h1_d = differentiate_hist( h1, diff_rebin );
		lastpeak = (N>0 ? yv[N-1] : 0);
		if(iter == 0 && previousfunc != NULL && previousfunc->GetParameter(2) > 0.0001)
			lastpeak = previousfunc->Eval(xv[N]);
		cout<<"Fitting bin #"<<bin<<endl;
		if( iter > 0 && func->GetParameter(2) > 0.0001)
			lastpeak = func->Eval(xv[N]);
		cout<<"lastpeak="<<lastpeak<<endl;
		//h1->Rebin(2);
		TF1* fgaus;
		fgaus = fit_hist(h1, h1_d, lastpeak);
		/*
    h1_2->Draw();
    c.Print("plots/target-107924-109081.pdf","pdf");
		 */



		if(plot_all) {
			h1->Draw();
			if( lastpeak > 0 ) {
				TLine l(lastpeak,0,lastpeak,h1->GetMaximum());
				l.SetLineColor(2);
				l.Draw();
				c.Print(pdfname,"pdf");
			} else {
				c.Print(pdfname,"pdf");
			}
			h1_d->Draw();
			c.Print(pdfname,"pdf");
		}

		yv[N] = fgaus->GetParameter(1);
		eyv[N] = fgaus->GetParError(1);
		N += 1;
		delete fgaus;
		delete h1_d;
		delete h1;
	}
	return N;
}


void fit_circle(TH2F* h2, float& x0, float& y0)
{
	int bx0 = h2->GetXaxis()->FindBin(x0);
	int by0 = h2->GetYaxis()->FindBin(y0);

	TH2F* h2_sigma = new TH2F("h2_sigma", "h2_sigma",
			h2->GetXaxis()->GetNbins(),
			h2->GetXaxis()->GetXmin(),
			h2->GetXaxis()->GetXmax(),
			h2->GetYaxis()->GetNbins(),
			h2->GetYaxis()->GetXmin(),
			h2->GetYaxis()->GetXmax());

	int D = 3;
	for( int bx = bx0-D; bx <= bx0+D; bx++ ) {

		float xc = h2->GetXaxis()->GetBinCenter(bx);
		for( int by = by0-D; by <= by0+D; by++ ) {

			float yc = h2->GetYaxis()->GetBinCenter(by);
			TH1D* hR = new TH1D("hR","R distribution",150,0,3);

			// Loop over the whole histogram
			for( int ix = 1; ix <= h2->GetXaxis()->GetNbins(); ix++ ) {
				float x = h2->GetXaxis()->GetBinCenter(ix);
				for( int iy = 1; iy <= h2->GetYaxis()->GetNbins(); iy++ ) {
					float y = h2->GetYaxis()->GetBinCenter(iy);
					float dx = x-xc;
					float dy = y-yc;
					float R = TMath::Sqrt(dx*dx + dy*dy);
					for(int n = 0; n < h2->GetBinContent(ix,iy); n++)
						hR->Fill(R);
				}
			}
			hR->Draw();
			TF1* fgaus = new TF1("Rgaus","gaus(0)+gaus(3)+pol1(6)");
			fgaus->SetParLimits(0,0,100000000);
			fgaus->SetParameter(1,2-0.1);
			fgaus->SetParLimits(1,1.5,2.5);
			fgaus->SetParameter(2,0.01);
			fgaus->SetParLimits(2,0.005,10);
			fgaus->SetParLimits(0+3,0,100000000);
			fgaus->SetParameter(1+3,2+0.1);
			fgaus->SetParLimits(1+3,1.5,2.5);
			fgaus->SetParameter(2+3,0.01);
			fgaus->SetParLimits(2+3,0.005,10);
			TH1D* h1_d = differentiate_hist( hR, 2 );
			h1_d->Draw();
			c.Print("plots/target-107924-109081.pdf","pdf");
			hR->Fit("Rgaus","RNQ","",1.5,2.5);
			hR->Fit("Rgaus","RNQ","",1.5,2.5);
			hR->Fit("Rgaus","RQ","",1.5,2.5);
			c.Print("plots/target-107924-109081.pdf","pdf");
			cout<<"h2_sigma->SetBinContent("<<bx<<","<<by<<","<<fgaus->GetParameter(2)<<")"<<endl;
			h2_sigma->SetBinContent(bx, by, fgaus->GetParameter(2)*1000000);
			h2_sigma->SetBinError(bx, by, fgaus->GetParError(2)*1000000);
			delete fgaus;
			delete hR;
		}
	}

	h2_sigma->GetXaxis()->SetRange(bx0-D,bx0+D);
	h2_sigma->GetYaxis()->SetRange(by0-D,by0+D);
	h2_sigma->Draw("colz");
	c.Print("plots/target-107924-109081.pdf","pdf");
}


//void plot_target(int run1, int run2)
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << FRED("ERROR : Not enough arguments.") << endl;
		cout << FRED("Asked : 2 *** Received : ") << argc-1 << endl;
		cout << FRED("./plot_target [run1] [run2]") << endl;
		cout << FRED("Use ./plot_target -h for further help") << endl;

		return 1;
	}

	int run1, run2;

	for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :") << endl;
      cout << FCYN("-pm") << endl;
			cout << FCYN("./plot_target  [run1] [run2] ") << endl;
      return 0;
    }

		if(argc < 2)
	  {
	    cout << FRED("ERROR : Not enough arguments.") << endl;
	    cout << FRED("Asked : 2 *** Received : ") << argc-1 << endl;
	    cout << FRED("./plot_target -pm [run1] [run2]") << endl;

	    return 1;
	  }

    if (string(argv[i]) == "-pm")
    {
      run1 = atoi(argv[2]);
			run2 = atoi(argv[3]);
    }
		else
		{
			run1 = atoi(argv[1]);
			run2 = atoi(argv[2]);
		}
  }


	InitTargetCoordinates(2016);

	char tstr[500];
	char tstr2[500];

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	gStyle->SetOptFit(1111);
	//gStyle->SetPalette(1,0);
	gStyle->SetPalette(53);

	//LoadFiles(runmin,runmax);
	gROOT->ProcessLine(".L HistLoader.cc+");

	// replace this with the actual output folder for the root files
	// The structure of the folder and file names should be the following:
	// [base folder]/[run number]/hist-[user event number]-[sequential index].root
	//
	// The HistLoader loads all the root files from run1 to run2 included,
	// and provides the summed histograms on-demand, like this:
	//
	// TH1F* h = histloader.GetTH1("All Events/Primary Histos - theta/ZVertex");
	//
	HistLoader histloader("../outputs/2016/phast/%d/hist-4-%d.root",run1,run2);
	// /afs/cern.ch/user/b/btuybens/Phast/Output/270475/hist-3-1.root
	// /afs/cern.ch/user/b/btuybens/Phast/Output/270475/hist-3-2.root
	// /afs/cern.ch/user/b/btuybens/Phast/Output/270475/hist-3-3.root
	// /afs/cern.ch/user/b/btuybens/Phast/Output/270475/hist-3-4.root
	// ....
	// /afs/cern.ch/user/b/btuybens/Phast/Output/270475/hist-3-189.root

	int colors[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	TCanvas c("c","c", 800, 800);
	c.SetLeftMargin(0.1);
	c.SetBottomMargin(0.1);
	c.SetTopMargin(0.07);
	c.SetRightMargin(0.05);
	TCanvas c2("c2","c2");

	c2.Divide(1,3);

	c2.cd(1);
	gPad->Divide(2,1);
	c2.cd(2);
	gPad->Divide(6,1);
	c2.cd(3);
	gPad->Divide(6,1);


	c.cd();

	TH1F* h = histloader.GetTH1("All Events/Primary Histos - theta/ZVertex");
	h->Draw();
	sprintf(tstr,"%s - 2mrad < theta < 5mrad",h->GetTitle());
	h->SetTitle(tstr);
	TLine *l1,*l2,*l0;
	l0 = new TLine(target_inlet_zmin,h->GetMaximum()/3,target_inlet_zmax,h->GetMaximum()/3);
	l1 = new TLine(target_zmin,h->GetMaximum()/3,target_zmax,h->GetMaximum()/3);
	l2 = new TLine(target_cryostat_window_z,0,target_cryostat_window_z,h->GetMaximum());
	l0->SetLineColor(4); l0->SetLineWidth(7);
	l1->SetLineColor(2); l1->SetLineWidth(3);
	l2->SetLineColor(2); l2->SetLineWidth(1);
	for(int i = 26; i >= 0; i--) {
		float z0 = target_cell_center - 135 + i*10;
		TLine* l = new TLine(z0,0,z0,h->GetMaximum());
		l->SetLineColor(8); l->SetLineWidth(1);
		l->Draw();
	}
	l0->Draw();l1->Draw();l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf(",run1, run2);
	c.Print(tstr,"pdf");


	sprintf(tstr,"All Events/Primary Histos - theta2/ZVertex");

	delete h;
	h = histloader.GetTH1(tstr); h->Draw();
	sprintf(tstr,"%s - theta > 5mrad",h->GetTitle());
	h->SetTitle(tstr);

	delete l0;
	delete l1;
	delete l2;

	//TLine *l1,*l2,*l0;
	l0 = new TLine(target_inlet_zmin,h->GetMaximum()/3,target_inlet_zmax,h->GetMaximum()/3);
	l1 = new TLine(target_zmin,h->GetMaximum()/3,target_zmax,h->GetMaximum()/3);
	l2 = new TLine(target_cryostat_window_z,0,target_cryostat_window_z,h->GetMaximum());
	l0->SetLineColor(4); l0->SetLineWidth(7);
	l1->SetLineColor(2); l1->SetLineWidth(3);
	l2->SetLineColor(2); l2->SetLineWidth(1);
	for(int i = 26; i >= 0; i--) {
		float z0 = target_cell_center - 135 + i*10;
		TLine* l = new TLine(z0,0,z0,h->GetMaximum());
		l->SetLineColor(8); l->SetLineWidth(1);
		l->Draw();
	}
	l0->Draw();l1->Draw();l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	c.Print(tstr,"pdf");


	TF1* fgaus = new TF1("fgausbeam","gaus",-1.5,1.5);

	//	int slices[4] = {0,1,2,15};
	//
	//	for(int s = 0; s < 4; s++) {
	//		sprintf(tstr,"All Events/Primary Histos - theta2/XYVertexSlices_%d",slices[s]);
	//		TH2F* h2 = histloader.GetTH2(tstr); h2->Draw("col");
	//		sprintf(tstr,"%s - theta > 5mrad",h2->GetTitle());
	//		h2->SetTitle(tstr);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//		int bin1 = h2->GetYaxis()->FindBin(-0.5);
	//		int bin2 = h2->GetYaxis()->FindBin(+0.5);
	//		TH1F* h = (TH1F*)h2->ProjectionX("_px",bin1,bin2); h->Draw();
	//		sprintf(tstr,"%s - X projection",h->GetTitle());
	//		h->Rebin(4);
	//		h->SetTitle(tstr);
	//		h->Fit("fgausbeam","R","",-1.5,1.5);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//		int bin1 = h2->GetXaxis()->FindBin(-0.5);
	//		int bin2 = h2->GetXaxis()->FindBin(+0.5);
	//		TH1F* h = (TH1F*)h2->ProjectionY("_py",bin1,bin2); h->Draw();
	//		sprintf(tstr,"%s - Y projection",h->GetTitle());
	//		h->Rebin(4);
	//		h->SetTitle(tstr);
	//		h->Fit("fgausbeam","R","",-1.5,1.5);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//	}


	sprintf(tstr,"All Events/Primary Histos - Q2/ZVertex");
	delete h;

	h = histloader.GetTH1(tstr); h->Draw();
	sprintf(tstr,"%s - Q^{2} > 0.4",h->GetTitle());
	h->SetTitle(tstr);

	delete l0;
	delete l1;
	delete l2;

	//TLine* l1,l2,l0;
	l0 = new TLine(target_inlet_zmin,h->GetMaximum()/3,target_inlet_zmax,h->GetMaximum()/3);
	l1 = new TLine(target_zmin,h->GetMaximum()/3,target_zmax,h->GetMaximum()/3);
	l2 = new TLine(target_cryostat_window_z,0,target_cryostat_window_z,h->GetMaximum());
	l0->SetLineColor(4); l0->SetLineWidth(7);
	l1->SetLineColor(2); l1->SetLineWidth(3);
	l2->SetLineColor(2); l2->SetLineWidth(1);
	for(int i = 26; i >= 0; i--) {
		float z0 = target_cell_center - 135 + i*10;
		TLine* l = new TLine(z0,0,z0,h->GetMaximum());
		l->SetLineColor(8); l->SetLineWidth(1);
		l->Draw();
	}
	l0->Draw();l1->Draw();l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	c.Print(tstr,"pdf");


	sprintf(tstr,"All Events/Primary Histos - 2 tracks + Q2/ZVertex");

	delete h;
	h = histloader.GetTH1(tstr); h->Draw();
	sprintf(tstr,"%s - 2 tracks",h->GetTitle());
	h->SetTitle(tstr);

	delete l0;
	delete l1;
	delete l2;

	//TLine* l1,l2,l0;
	l0 = new TLine(target_inlet_zmin,h->GetMaximum()/3,target_inlet_zmax,h->GetMaximum()/3);
	l1 = new TLine(target_zmin,h->GetMaximum()/3,target_zmax,h->GetMaximum()/3);
	l2 = new TLine(target_cryostat_window_z,0,target_cryostat_window_z,h->GetMaximum());
	l0->SetLineColor(4); l0->SetLineWidth(7);
	l1->SetLineColor(2); l1->SetLineWidth(3);
	l2->SetLineColor(2); l2->SetLineWidth(1);
	for(int i = 26; i >= 0; i--) {
		float z0 = target_cell_center - 135 + i*10;
		TLine* l = new TLine(z0,0,z0,h->GetMaximum());
		l->SetLineColor(8); l->SetLineWidth(1);
		l->Draw();
	}
	l0->Draw();l1->Draw();l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	c.Print(tstr,"pdf");


	sprintf(tstr,"All Events/Primary Histos - 3 tracks/ZVertex");
	h = histloader.GetTH1(tstr); h->Draw();
	sprintf(tstr,"%s - 3+ tracks",h->GetTitle());
	h->SetTitle(tstr);

	delete l0;
	delete l1;
	delete l2;

	//TLine* l1,l2,l0;
	l0 = new TLine(target_inlet_zmin,h->GetMaximum()/3,target_inlet_zmax,h->GetMaximum()/3);
	l1 = new TLine(target_zmin,h->GetMaximum()/3,target_zmax,h->GetMaximum()/3);
	l2 = new TLine(target_cryostat_window_z,0,target_cryostat_window_z,h->GetMaximum());
	l0->SetLineColor(4); l0->SetLineWidth(7);
	l1->SetLineColor(2); l1->SetLineWidth(3);
	l2->SetLineColor(2); l2->SetLineWidth(1);
	for(int i = 26; i >= 0; i--) {
		float z0 = target_cell_center - 135 + i*10;
		TLine* l = new TLine(z0,0,z0,h->GetMaximum());
		l->SetLineColor(8); l->SetLineWidth(1);
		l->Draw();
	}
	l0->Draw();l1->Draw();l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	c.Print(tstr,"pdf");

	h->GetXaxis()->SetRangeUser(-80,-40);h->Draw();
	l2->Draw();
	sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	c.Print(tstr,"pdf");


	//	for(int s = 0; s < 4; s++) {
	//		sprintf(tstr,"All Events/Primary Histos - 3 tracks/XYVertexSlices_%d",slices[s]);
	//		TH2F* h2 = histloader.GetTH2(tstr); h2->Draw("col");
	//		sprintf(tstr,"%s - 3+ tracks",h2->GetTitle());
	//		h2->SetTitle(tstr);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//		int bin1 = h2->GetYaxis()->FindBin(-0.5);
	//		int bin2 = h2->GetYaxis()->FindBin(+0.5);
	//		TH1F* h = (TH1F*)h2->ProjectionX("_px",bin1,bin2); h->Draw();
	//		sprintf(tstr,"%s - X projection",h->GetTitle());
	//		h->Rebin(4);
	//		h->SetTitle(tstr);
	//		h->Fit("fgausbeam","R","",-1.5,1.5);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//		int bin1 = h2->GetXaxis()->FindBin(-0.5);
	//		int bin2 = h2->GetXaxis()->FindBin(+0.5);
	//		TH1F* h = (TH1F*)h2->ProjectionY("_py",bin1,bin2); h->Draw();
	//		sprintf(tstr,"%s - Y projection",h->GetTitle());
	//		h->Rebin(4);
	//		h->SetTitle(tstr);
	//		h->Fit("fgausbeam","R","",-1.5,1.5);
	//		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
	//		c.Print(tstr,"pdf");
	//	}
	//


	string cuts[2] = {"Q2", "3 tracks"};

	float zv[100];
	float cell_rv[2][100];
	float cell_erv[2][100];
	float cell_phiv[2][100];
	float cell_ephiv[2][100];
	float cell_xv[2][100];
	float cell_yv[2][100];
	float cell_Rv[2][100];
	float cell_exv[2][100];
	float cell_eyv[2][100];
	float cell_eRv[2][100];
	TF1* funcv[2][100];

	for(int i = 0; i < 100; i++) {
		for(int cut = 0; cut < 2; cut++) {
			zv[i] = cell_xv[cut][i] = cell_yv[cut][i] = cell_Rv[cut][i] = 0;
			cell_rv[cut][i] = cell_erv[cut][i] = cell_phiv[cut][i] = cell_ephiv[cut][i] = 0;
			cell_exv[cut][i] = cell_eyv[cut][i] = cell_eRv[cut][i] = 0;
			funcv[cut][i]=NULL;
		}
	}


	float xv[100][500];
	float yv[100][500];
	float eyv[100][500];
	for(int pi=0; pi < 100; pi++) {
		for(int pj=0; pj < 500; pj++) {
			xv[pi][pj]=yv[pi][pj]=eyv[pi][pj]=0;
		}
	}

	sprintf(pdfname,"plots/target-%d-%d.pdf",run1, run2);

	Style_t axis_label_font;
	Float_t axis_label_size;
	Style_t axis_title_font;
	Float_t axis_title_size;

	int charge = 0;

	//c.SetLogz(kTRUE);
	float lastR = 2;
	int zfirst = 0;
	int zlast = 26; //26
	int zstart = 20; //20


	//float target_cell_center = -193.79; //-191.19;    -316.79,-70.79

	for(int cut = 0; cut < 1; cut++) {
		for(int i = zstart; i >= zfirst; i--) {
			float z0 = target_cell_center - 135 + i*10;

			sprintf(tstr,"All Events/Primary Histos - %s/RPhiVertexSlices_%d",cuts[cut].c_str(),i);
			cout<<tstr<<endl;
			TH2F* h2 = histloader.GetTH2(tstr,charge);
			sprintf(tstr,"%s - %s cut",h2->GetTitle(),cuts[cut].c_str());
			h2->SetTitle(tstr);
			h2->GetXaxis()->SetTitle("vertex #phi (degrees)");
			h2->GetYaxis()->SetTitle("vertex r (cm)");
			h2->GetYaxis()->SetTitleOffset(1.1);

			axis_label_font = h2->GetXaxis()->GetLabelFont();
			axis_label_size = h2->GetXaxis()->GetLabelSize();
			axis_title_font = h2->GetXaxis()->GetTitleFont();
			axis_title_size = h2->GetXaxis()->GetTitleSize();

			TF1 *func = new TF1("modulation",modulation,20*TMath::Pi()/180.,TMath::Pi()*2,3);
			funcv[cut][i]=func;
			func->SetLineColor(3);
			func->SetLineWidth(1);
			func->SetParameter(2,0);
			for(int pi = 0; pi < 500; pi++) yv[i][pi] = yv[i+1][pi];
			int iter_max = 1;
			for( int iter = 0; iter <= iter_max; iter++ ) {
				int N = fit_slice(h2, 6, xv[i], yv[i], eyv[i], func, funcv[cut][i+1], iter);
				TGraphErrors gr(N,xv[i],yv[i],NULL,eyv[i]);
				if( iter == 0 ) {
					func->SetParameter(2,2);
					if( lastR > 0 ) func->FixParameter(2,lastR);
				} else {
					func->ReleaseParameter(2);
				}
				gr.Fit("modulation");
				for( int n=0; n<N; n++ ) yv[i][n] = func->Eval(xv[i][n]);
				if( iter == iter_max || plot_fits) {
					h2->Draw("col");
					sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
					c.Print(tstr,"pdf");
					if(plot_fits) {
						c.Clear();
						gr.Draw("A*");
						sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
						c.Print(tstr,"pdf");
						h2->Draw("col");
						gr.Draw("*same");
						sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
						c.Print(tstr,"pdf");
					}
				}
			}



			float cell_x = -1.0*func->GetParameter(0)*TMath::Cos(func->GetParameter(1));
			float cell_y = -1.0*func->GetParameter(0)*TMath::Sin(func->GetParameter(1));
			float cell_R = func->GetParameter(2);


			// for 2012
			//      if(i==2) {
			//        cell_x = -0.05;
			//        cell_y = -0.4549;
			//        cell_R = cell_Rv[cut][i+1];
			//      }

			zv[i] = z0+5;
			cell_rv[cut][i] = func->GetParameter(0);
			cell_phiv[cut][i] = func->GetParameter(1);
			cell_xv[cut][i] = cell_x;
			cell_yv[cut][i] = cell_y;
			cell_Rv[cut][i] = cell_R;
			cell_erv[cut][i] = func->GetParError(0);
			cell_ephiv[cut][i] = func->GetParError(1);
			cell_eRv[cut][i] = func->GetParError(2);

			cout<<"cell x="<<cell_x<<"  y="<<cell_y<<"  R="<<cell_Rv[cut][i]<<endl;


			sprintf(tstr,"All Events/Primary Histos - %s/XYVertexSlices_%d",cuts[cut].c_str(),i);
			cout<<tstr<<endl;
			TH2F* h2xy = histloader.GetTH2(tstr,charge);
			sprintf(tstr,"%s - %s cut",h2xy->GetTitle(),cuts[cut].c_str());
			h2xy->SetTitle(tstr);
			h2xy->GetXaxis()->SetTitle("vertex x (cm)");
			h2xy->GetYaxis()->SetTitle("vertex y (cm)");
			//sprintf(tstr,"%s - Q2",h2->GetTitle());
			//h2xy->SetTitle(tstr);
			//h2xy->GetXaxis()->SetRangeUser(cell_x-2.5, cell_x+2.5);
			//h2xy->GetYaxis()->SetRangeUser(cell_y-2.5, cell_y+2.5);
			h2xy->GetXaxis()->SetRangeUser(-3, 3);
			h2xy->GetYaxis()->SetRangeUser(-3.5, 2.5);
			h2xy->Draw("col");
			for(int phi = 0; phi < 360; phi += 6) {
				float px = TMath::Cos(TMath::Pi()*phi/180.)*cell_R + cell_x;
				float py = TMath::Sin(TMath::Pi()*phi/180.)*cell_R + cell_y;
				TEllipse* ell = new TEllipse(px, py, 0.02);
				ell->SetLineColor(4);
				ell->SetFillColor(4);
				if(plot_fits) ell->Draw();
			}

			sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
			c.Print(tstr,"pdf");

			//delete func;

			//fit_circle(h2xy, cell_x, cell_y);
		}


		for(int i = zstart +1 ; i <= zlast; i++) {
			float z0 = target_cell_center - 135 + i*10;

			sprintf(tstr,"All Events/Primary Histos - %s/RPhiVertexSlices_%d",cuts[cut].c_str(),i);
			cout<<tstr<<endl;
			TH2F* h2 = histloader.GetTH2(tstr,charge);
			sprintf(tstr,"%s - %s cut",h2->GetTitle(),cuts[cut].c_str());
			h2->SetTitle(tstr);
			h2->GetXaxis()->SetTitle("vertex #phi (degrees)");
			h2->GetYaxis()->SetTitle("vertex r (cm)");
			h2->GetYaxis()->SetTitleOffset(1.1);

			axis_label_font = h2->GetXaxis()->GetLabelFont();
			axis_label_size = h2->GetXaxis()->GetLabelSize();
			axis_title_font = h2->GetXaxis()->GetTitleFont();
			axis_title_size = h2->GetXaxis()->GetTitleSize();

			TF1 *func = new TF1("modulation",modulation,20*TMath::Pi()/180.,TMath::Pi()*2,3);
			funcv[cut][i]=func;
			func->SetLineColor(3);
			func->SetLineWidth(1);
			func->SetParameter(2,0);
			for(int pi = 0; pi < 500; pi++) yv[i][pi] = yv[i+1][pi];
			int iter_max = 1;
			for( int iter = 0; iter <= iter_max; iter++ ) {
				int N = fit_slice(h2, 6, xv[i], yv[i], eyv[i], func, funcv[cut][i+1], iter);
				TGraphErrors gr(N,xv[i],yv[i],NULL,eyv[i]);
				if( iter == 0 ) {
					func->SetParameter(2,2);
					if( lastR > 0 ) func->FixParameter(2,lastR);
				} else {
					func->ReleaseParameter(2);
				}
				gr.Fit("modulation");
				for( int n=0; n<N; n++ ) yv[i][n] = func->Eval(xv[i][n]);
				if( iter == iter_max || plot_fits) {
					h2->Draw("col");
					sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
					c.Print(tstr,"pdf");
					if(plot_fits) {
						c.Clear();
						gr.Draw("A*");
						sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
						c.Print(tstr,"pdf");
						h2->Draw("col");
						gr.Draw("*same");
						sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
						c.Print(tstr,"pdf");
					}
				}
			}


			float cell_x = -1.0*func->GetParameter(0)*TMath::Cos(func->GetParameter(1));
			float cell_y = -1.0*func->GetParameter(0)*TMath::Sin(func->GetParameter(1));
			float cell_R = func->GetParameter(2);


			// for 2012
			//      if(i==2) {
			//        cell_x = -0.05;
			//        cell_y = -0.4549;
			//        cell_R = cell_Rv[cut][i+1];
			//      }

			zv[i] = z0+5;
			cell_rv[cut][i] = func->GetParameter(0);
			cell_phiv[cut][i] = func->GetParameter(1);
			cell_xv[cut][i] = cell_x;
			cell_yv[cut][i] = cell_y;
			cell_Rv[cut][i] = cell_R;
			cell_erv[cut][i] = func->GetParError(0);
			cell_ephiv[cut][i] = func->GetParError(1);
			cell_eRv[cut][i] = func->GetParError(2);

			cout<<"cell x="<<cell_x<<"  y="<<cell_y<<"  R="<<cell_Rv[cut][i]<<endl;


			sprintf(tstr,"All Events/Primary Histos - %s/XYVertexSlices_%d",cuts[cut].c_str(),i);
			cout<<tstr<<endl;
			TH2F* h2xy = histloader.GetTH2(tstr,charge);
			sprintf(tstr,"%s - %s cut",h2xy->GetTitle(),cuts[cut].c_str());
			h2xy->SetTitle(tstr);
			h2xy->GetXaxis()->SetTitle("vertex x (cm)");
			h2xy->GetYaxis()->SetTitle("vertex y (cm)");
			//sprintf(tstr,"%s - Q2",h2->GetTitle());
			//h2xy->SetTitle(tstr);
			//h2xy->GetXaxis()->SetRangeUser(cell_x-2.5, cell_x+2.5);
			//h2xy->GetYaxis()->SetRangeUser(cell_y-2.5, cell_y+2.5);
			h2xy->GetXaxis()->SetRangeUser(-3, 3);
			h2xy->GetYaxis()->SetRangeUser(-3.5, 2.5);
			h2xy->Draw("col");
			for(int phi = 0; phi < 360; phi += 6) {
				float px = TMath::Cos(TMath::Pi()*phi/180.)*cell_R + cell_x;
				float py = TMath::Sin(TMath::Pi()*phi/180.)*cell_R + cell_y;
				TEllipse* ell = new TEllipse(px, py, 0.02);
				ell->SetLineColor(4);
				ell->SetFillColor(4);
				if(plot_fits) ell->Draw();
			}

			sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
			c.Print(tstr,"pdf");

			//delete func;

			//fit_circle(h2xy, cell_x, cell_y);
		}


		TLine *l1, *l2;
		//		l1 = new TLine(-314.19,-1,-314.19,0);
		//		l2 = new TLine(-68.19,-1,-68.19,0);
		l1 = new TLine(-314.19,-0.5,-314.19,0.5);
		l2 = new TLine(-68.19,-0.5,-68.19,0.5);
		l1->SetLineColor(2); l2->SetLineWidth(1);
		l2->SetLineColor(2); l2->SetLineWidth(1);
		TGraph* xgr = new TGraph(zlast-zfirst+1,&(zv[zfirst]),&(cell_xv[cut][zfirst]));
		xgr->Draw("A*");
		//		xgr->SetMinimum(-1);
		//		xgr->SetMaximum(0);
		xgr->SetMinimum(-0.5);
		xgr->SetMaximum(0.5);
		xgr->SetTitle("X position of target center along Z-axis");
		xgr->GetXaxis()->SetTitle("vertex z (cm)");
		xgr->GetYaxis()->SetTitle("x position (cm)");
		xgr->GetXaxis()->SetLabelFont(axis_label_font);
		xgr->GetXaxis()->SetLabelSize(axis_label_size);
		xgr->GetXaxis()->SetTitleFont(axis_title_font);
		xgr->GetXaxis()->SetTitleSize(axis_title_size);
		xgr->GetYaxis()->SetLabelFont(axis_label_font);
		xgr->GetYaxis()->SetLabelSize(axis_label_size);
		xgr->GetYaxis()->SetTitleFont(axis_title_font);
		xgr->GetYaxis()->SetTitleSize(axis_title_size);
		xgr->GetYaxis()->SetTitleOffset(1.1);
		l1->Draw(); l2->Draw();
		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
		c.Print(tstr,"pdf");

		delete l1;
		delete l2;

		l1 = new TLine(-314.19,-1,-314.19,0);
		l2 = new TLine(-68.19,-1,-68.19,0);
		l1->SetLineColor(2); l2->SetLineWidth(1);
		l2->SetLineColor(2); l2->SetLineWidth(1);
		TGraph* ygr = new TGraph(zlast-zfirst+1,&(zv[zfirst]),&(cell_yv[cut][zfirst]));
		ygr->Draw("A*");
		ygr->SetMinimum(-1);
		ygr->SetMaximum(0);
		ygr->SetTitle("Y position of target center along Z-axis");
		ygr->GetXaxis()->SetTitle("vertex z (cm)");
		ygr->GetYaxis()->SetTitle("y position (cm)");
		ygr->GetXaxis()->SetLabelFont(axis_label_font);
		ygr->GetXaxis()->SetLabelSize(axis_label_size);
		ygr->GetXaxis()->SetTitleFont(axis_title_font);
		ygr->GetXaxis()->SetTitleSize(axis_title_size);
		ygr->GetYaxis()->SetLabelFont(axis_label_font);
		ygr->GetYaxis()->SetLabelSize(axis_label_size);
		ygr->GetYaxis()->SetTitleFont(axis_title_font);
		ygr->GetYaxis()->SetTitleSize(axis_title_size);
		ygr->GetYaxis()->SetTitleOffset(1.1);
		l1->Draw(); l2->Draw();
		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
		c.Print(tstr,"pdf");

		delete l1;
		delete l2;

		l1 = new TLine(-314.19,1.9,-314.19,2.1);
		l2 = new TLine(-68.19,1.9,-68.19,2.1);
		l1->SetLineColor(2); l2->SetLineWidth(1);
		l2->SetLineColor(2); l2->SetLineWidth(1);
		TGraphErrors* Rgr = new TGraphErrors(zlast-zfirst+1,&(zv[zfirst]),&(cell_Rv[cut][zfirst]), NULL, &(cell_eRv[cut][zfirst]));
		Rgr->Draw("A*");
		Rgr->SetMinimum(1.9);
		Rgr->SetMaximum(2.1);
		Rgr->SetTitle("Radius of target cell along Z-axis");
		Rgr->GetXaxis()->SetTitle("vertex z (cm)");
		Rgr->GetYaxis()->SetTitle("R (cm)");
		Rgr->GetXaxis()->SetLabelFont(axis_label_font);
		Rgr->GetXaxis()->SetLabelSize(axis_label_size);
		Rgr->GetXaxis()->SetTitleFont(axis_title_font);
		Rgr->GetXaxis()->SetTitleSize(axis_title_size);
		Rgr->GetYaxis()->SetLabelFont(axis_label_font);
		Rgr->GetYaxis()->SetLabelSize(axis_label_size);
		Rgr->GetYaxis()->SetTitleFont(axis_title_font);
		Rgr->GetYaxis()->SetTitleSize(axis_title_size);
		Rgr->GetYaxis()->SetTitleOffset(1.1);
		l1->Draw(); l2->Draw();
		sprintf(tstr,"plots/target-%d-%d.pdf",run1, run2);
		c.Print(tstr,"pdf");

		sprintf(tstr,"plots/target-%d-%d-%d.dat",run1,run2,cut);
		ofstream outf;
		outf.open(tstr);
		for(int i = zfirst; i <= zlast; i++) {
			outf<<zv[i]<<" "<<cell_rv[cut][i]<<" "<<cell_erv[cut][i]
																  <<" "<<cell_phiv[cut][i]<<" "<<cell_ephiv[cut][i]
																												 <<" "<<cell_Rv[cut][i]<<" "<<cell_eRv[cut][i]
																																							<<" "<<cell_xv[cut][i]<<" "<<cell_yv[cut][i]<<endl;
		}

	}

	c.Clear();
	sprintf(tstr,"plots/target-%d-%d.pdf)",run1, run2);
	c.Print(tstr,"pdf");

	return 0;
}
