#include "include/plot_dis.h"

using namespace std;

void plot_HODO(float x0, float y0, float aXsz, float aYsz,
		float dcx0, float dcy0, float dcXsz, float dcYsz,
		float dpx0, float dpy0, float dpXsz, float dpYsz)
{
	float xc = x0;
	float yc = y0;
	float Xsz = aXsz/2;
	float Ysz = aYsz/2;

	TLine* l = new TLine(-Xsz+xc, -Ysz+yc, Xsz+xc, -Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(-Xsz+xc, Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(-Xsz+xc, -Ysz+yc, -Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(Xsz+xc, -Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	// Dead zone CORAL
	xc = dcx0;
	yc = dcy0;
	Xsz = dcXsz/2;
	Ysz = dcYsz/2;

	l = new TLine(-Xsz+xc, -Ysz+yc, Xsz+xc, -Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(-Xsz+xc, Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(-Xsz+xc, -Ysz+yc, -Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	l = new TLine(Xsz+xc, -Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(2); l->Draw();

	// Dead zone PHAST
	xc = dpx0;
	yc = dpy0;
	Xsz = dpXsz;
	Ysz = dpYsz;

	l = new TLine(-Xsz+xc, -Ysz+yc, Xsz+xc, -Ysz+yc);
	l->SetLineColor(3); l->Draw();

	l = new TLine(-Xsz+xc, Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(3); l->Draw();

	l = new TLine(-Xsz+xc, -Ysz+yc, -Xsz+xc, Ysz+yc);
	l->SetLineColor(3); l->Draw();

	l = new TLine(Xsz+xc, -Ysz+yc, Xsz+xc, Ysz+yc);
	l->SetLineColor(3); l->Draw();
}


void plot_HO03_2016()
{
	plot_HODO(4.4, -0.37353, 250.0, 117.5,
			17.5, -0.3732, 45.0, 50.5,
			4.4, -0.37353, 250.0, 6.5);
}
void plot_HO04_2016()
{
	//HO041
	plot_HODO(132.5, 0.38908, 250.0, 225.0,
			60, 0.2508, 105, 83,
			132.5, 0.38908, 190, 14);
	//
	//HO042
	plot_HODO(-102.50000, -0.57519, 250.0, 225.0,
			15, -0.6200, 14, 83,
			-102.50000, -0.57519, 250, 14);
}
void plot_HM04_2016()
{//down
	plot_HODO(58.5, -41.57190 , 120, 18.950,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(58.5, -24.17190, 120, 16.150,
			0, 0, 0, 0,
			0, 0, 0, 0);
	//up
	plot_HODO(58.5, 23.72157, 120, 16.150 ,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(58.5,  41.12157, 120, 18.950,
			0, 0, 0, 0,
			0, 0, 0, 0);
}
void plot_HM05_2016()
{//down
	plot_HODO(74.50000, -49.09125 , 150.000, 22.950,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(74.50000,-28.29125 , 150, 18.950,
			0, 0, 0, 0,
			0, 0, 0, 0);
	//up
	plot_HODO(74.5, 28.35777, 150, 18.950 ,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(74.5,  49.15777 , 150, 22.950 ,
			0, 0, 0, 0,
			0, 0, 0, 0);
}
void plot_HL04_2016()
{
	plot_HODO(62.00547, 0 , 16.300, 40,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(82.00549,0 , 24.300 , 40,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(112.00549 , 0 , 36.300, 40,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(156.00549 ,0 , 52.300, 40,
			0, 0, 0, 0,
			0, 0, 0, 0);
}
void plot_HL05_2016()
{
	plot_HODO(77.40198, 0 , 20.300, 47.5,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(103.40198, 0  , 32.300  , 47.5,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(143.40198  , 0 ,48.300, 47.5,
			0, 0, 0, 0,
			0, 0, 0, 0);
	plot_HODO(201.40198  ,0 ,  68.300, 47.5,
			0, 0, 0, 0,
			0, 0, 0, 0);
}

void plot_HG01_2016()
{
	plot_HODO(0,  -0.69634  , 230, 195.1,
			0, -0.6963, 50, 37,
			0, 0, 0, 0);

}
void plot_HG02_2016()
{
	plot_HODO(123.50000, -0.37498  , 252.5, 419.7,
			36, -0.3748, 77.5, 78.1,
			0, 0, 0, 0);
	plot_HODO(-123.50000, -1.00319 , 252.5, 419.7,
			-36, -1.0033, 77.5, 78.1,
			0, 0, 0, 0);

}

void rebin2(TH1 *h, Int_t ngx, Int_t ngy)
{
	//Rebin 2-d histogram h, grouping ngx bins together along X
	//and ngy bins together along Y
	//NB: this macro ignores histogram errors if defined

	//make a clone of h
	TH1 *hold = (TH1*)h->Clone();
	hold->SetDirectory(0);

	Int_t  nbinsx = hold->GetXaxis()->GetNbins();
	Int_t  nbinsy = hold->GetYaxis()->GetNbins();
	Float_t xmin  = hold->GetXaxis()->GetXmin();
	Float_t xmax  = hold->GetXaxis()->GetXmax();
	Float_t ymin  = hold->GetYaxis()->GetXmin();
	Float_t ymax  = hold->GetYaxis()->GetXmax();
	Int_t nx = nbinsx/ngx;
	Int_t ny = nbinsy/ngy;
	h->SetBins (nx,xmin,xmax,ny,ymin,ymax);

	//loop on all bins to reset contents and errors
	Double_t cu;
	Float_t bx,by;
	Int_t ix,iy,ibin,bin,binx,biny;
	for (biny=1;biny<=nbinsy;biny++) {
		for (binx=1;binx<=nbinsx;binx++) {
			ibin = h->GetBin(binx,biny);
			h->SetBinContent(ibin,0);
		}
	}
	//loop on all bins and refill
	for (biny=1;biny<=nbinsy;biny++) {
		by  = hold->GetYaxis()->GetBinCenter(biny);
		iy  = h->GetYaxis()->FindBin(by);
		for (binx=1;binx<=nbinsx;binx++) {
			bx = hold->GetXaxis()->GetBinCenter(binx);
			ix  = h->GetXaxis()->FindBin(bx);
			bin = hold->GetBin(binx,biny);
			ibin= h->GetBin(ix,iy);
			cu  = hold->GetBinContent(bin);
			h->AddBinContent(ibin,cu);
		}
	}
	delete hold;
}

int main(int argc, char **argv)
// void plot_dis(int run1, int run2, int runm1=0, int runm2=0)
{
	if(argc < 2)
	{
		cout << FRED("ERROR : Not enough arguments.") << endl;
		cout << FRED("Asked : 3 *** Received : ") << argc-1 << endl;
		cout << FRED("./plot_dis [run1] [run2]") << endl;
		cout << FRED("Use ./plot_dis -h for further help") << endl;

		return 1;
	}

	int run1, run2, runm1, runm2;

	for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :") << endl;
      cout << FCYN("-pm") << endl;
			cout << FCYN("./plot_dis -pm [runp1] [runp2] [runm1] [runm2]") << endl;
      return 0;
    }

		if(argc < 3)
	  {
	    cout << FRED("ERROR : Not enough arguments.") << endl;
	    cout << FRED("Asked : 3 *** Received : ") << argc-1 << endl;
	    cout << FRED("./plot_dis [run1] [run2]") << endl;

	    return 1;
	  }

    if (string(argv[i]) == "-pm")
    {
      run1 = atoi(argv[2]);
			run2 = atoi(argv[3]);
			runm1 = atoi(argv[4]);
			runm2 = atoi(argv[5]);
    }
		else
		{
			run1 = atoi(argv[2]);
			run2 = atoi(argv[3]);
			runm1 = run1;
			runm2 = run2;
		}
  }

	char tstr[500];
	char tstr2[500];

	//
	// The color palette used for xbQ2 plots
	//

	const int nc = 160;
	Int_t MyPalette[nc];
	Double_t Red1[]    = {0.1, 1.0, 0.2, 0.0};
	Double_t Green1[]  = {0., 0.2, 0.2, 0.};
	Double_t Blue1[]   = {0., 0.2, 1.0, 0.1};
	Double_t Length1[] = {0., 0.5, 0.5, 1.0};
	Int_t FI1 = TColor::CreateGradientColorTable(4, Length1, Red1, Green1, Blue1, nc);
	for (int i=0;i<nc;i++) MyPalette[i] = FI1+i;

	TCanvas c("c","c", 800, 600);
	c.SetLeftMargin(0.1);
	c.SetBottomMargin(0.1);
	c.SetTopMargin(0.07);
	c.SetRightMargin(0.1);

	c.cd();


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	//gStyle->SetOptFit(1111);
	gStyle->SetPalette(1,0);
	//gStyle->SetPalette(53);

	//LoadFiles(runmin,runmax);
//	gROOT->ProcessLine(".L HistLoader.cc+");
//	gROOT->ProcessLine(".L GetFlux.cc+");

	// replace this with the actual output folder for the root files
	// The structure of the folder and file names should be the following:
	// [base folder]/[run number]/hist-[user event number]-[sequential index].root
	//
	// The HistLoader loads all the root files from run1 to run2 included,
	// and provides the summed histograms on-demand, like this:
	//
	// TH1F* h = histloader.GetTH1("All Events/Primary Histos - theta/ZVertex");
	//

	HistLoader hlp("..//outputs/2016/phast/%d/hist-1-%d.root",run1,run2);
	HistLoader hlm("..//outputs/2016/phast/%d/hist-1-%d.root",runm1,runm2);

	std::string trignames[5] = {"all", "MT", "LT", "OT", "LAST"};
	Double_t flux_p, eflux_p, flux_m, eflux_m;
	fill_badspills();
	if( get_flux(run1, run2, 1, 2, flux_p, eflux_p) < 0 )
	{
		cout << "ERROR : Flux is negative !" << endl;
		return 1;
	}
	if( get_flux(runm1, runm2, -1, 2, flux_m, eflux_m) < 0 )
	{
		cout << "ERROR : Flux is negative !" << endl;
		return 1;
	}
	//if( get_flux(run1, run1, 0, 2, flux_p, eflux_p) < 0 ) return;
	//if( get_flux(run2, run2, 0, 2, flux_m, eflux_m) < 0 ) return;

	//	flux_m /= flux_p;
	//	flux_p = 1;

	flux_p = flux_p/1000000000;
	flux_m = flux_m/1000000000;

	int ci = 1;
	int cut =0;

	/*****************************************************
  	  	  	  	   	 Vertex distribution
	 *****************************************************/
	cout<<"Vertex distribution ci="<<ci<<endl;
	for(int i = 0; i < 5; i++) {
		sprintf(tstr,"ZVertex_%s", trignames[i].c_str());
		TH1F* variable_plus  = hlp.GetTH1( tstr, 1 );
		TH1F* variable_minus = hlm.GetTH1( tstr, -1 );
		variable_plus->Rebin(20);
		variable_minus->Rebin(20);
		variable_plus->Scale(1/(flux_p));
		variable_minus->Scale(1/(flux_m));



		variable_plus->SetLineColor(4); //blue
		variable_minus->SetLineColor(2); //red


		variable_minus->Draw();
		variable_plus->Draw("same");

		TPaveText *pt = new TPaveText(.8,.8,.95,.95, "NDC");
		double Np = variable_plus->Integral();
		double Nm = variable_minus->Integral();
		pt->AddText(TString::Format("Integral+: %.0f", Np));
		pt->AddText(TString::Format("Integral-: %.0f", Nm));
		pt->Draw();



		if( ci ==  1 )
			sprintf(tstr,"plots/dis-%d-%d.pdf(", run1, run2);
		else
			sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);

		TH1F* hrapport_plusoverminus = variable_plus;
		sprintf(tstr,"Vertex distribution %s, #mu+ / #mu-",trignames[i].c_str());
		hrapport_plusoverminus->SetTitle(tstr);
		hrapport_plusoverminus->SetLineColor(8);
		hrapport_plusoverminus->Divide(variable_minus);
		hrapport_plusoverminus->SetMinimum(0.9);
		hrapport_plusoverminus->SetMaximum(1.1);
		sprintf(tstr,"fit-r");
		TF1* ffit_r = new TF1(tstr, "pol0(0)", -100, 100);
		ffit_r->SetParameter(0,1);
		ffit_r->SetParLimits(0,0.5,1.5);
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "");
		hrapport_plusoverminus->Draw();
		hrapport_plusoverminus->Draw();
		TLine line(hrapport_plusoverminus->GetXaxis()->GetXmin(), 1, hrapport_plusoverminus->GetXaxis()->GetXmax(), 1);
		line.Draw("same");

		double meanvalue = ffit_r->GetParameter(0);
		double sigma = ffit_r->GetParError(0);
		TLine lineup(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue+sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue+sigma);
		lineup.SetLineStyle(2);
		lineup.Draw("same");
		TLine linedw(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue-sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue-sigma);
		linedw.SetLineStyle(2);
		linedw.Draw("same");

		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
	}


	/*****************************************************
	  	 	  	run by run  Vertex distribution
	 *****************************************************/
	/*


	float xvec[1000];
	float npvec[1000];
	float nmvec[1000];
	float epvec[1000];
	float emvec[1000];
	float vecp_MT[1000],vecm_MT[1000],epvec_MT[1000],emvec_MT[1000]; // The MT is taken as a referrence since it appears to be the best trigger both in 2012 and 2015
	float vecp_div[1000],vecm_div[1000],epvec_div[1000],emvec_div[1000];


	for (int ti= 0; ti<5; ti++){
		int id = 0;
		for(int r = run1; r <= run2; r++){
			sprintf(tstr,"ZVertex_%s", trignames[ti].c_str());
			TH1F* variable_plus = histloader.GetTH1( tstr, 1, r, r );
			TH1F* variable_minus = histloader.GetTH1( tstr, -1, r, r );


			if( variable_plus && variable_minus ) {
				cout<<"Run "<<r<<" has both positive an negatove charge!"<<endl;
				continue;
			}
			if( !variable_plus && !variable_minus ) {
				//					cout<<"Run "<<r<<" has both positive an negatove charge!"<<endl;
				continue;
			}
			xvec[id] = r;
			npvec[id] = -10;
			nmvec[id] = -10;
			emvec[id] = 0;
			epvec[id] = 0;
			//			vecp_MT[id] = -10;
			//			epvec_MT[id] = 0;
			vecp_div[id] = -0.1;
			//			epvec_div[id] = 0;
			//			vecm_MT[id] = -10;
			//			emvec_MT[id] = 0;
			vecm_div[id] = -0.1;
			//			emvec_div[id] = 0;


			Double_t rflux, erflux;

			if( variable_plus ){
				if( get_flux(r, r, 1, 2, rflux, erflux) < 0 ) {
					cout<<"Run "<<r<<" has histograms but no flux!!!"<<endl;
					continue;
				}
				//			cout<<"Run "<<r<<"  # of events: "<<h->Integral()<<endl;
				rflux /= 1000000000.;
				//			variable_plus->Sumw2();
				variable_plus->Scale(1/(rflux));

				double ep,em;
				double Np = variable_plus->IntegralAndError(-1000,1000,ep);
				npvec[id] = Np;
				epvec[id] = ep;
				if (ti ==1){
					vecp_MT[id] = Np;
					epvec_MT[id] = ep;
				}
				if (ti >1){
					vecp_div[id] = Np/vecp_MT[id];
					epvec_div[id] = Np/vecp_MT[id]*sqrt(ep*ep/(Np*Np) + epvec_MT[id]*epvec_MT[id]/(vecp_MT[id]*vecp_MT[id])); //Manual prop. of error
				}
			}
			if( variable_minus ){
				if( get_flux(r, r, -1, 2, rflux, erflux) < 0 ) {
					cout<<"Run "<<r<<" has histograms but no flux!!!"<<endl;
					continue;
				}
				//			cout<<"Run "<<r<<"  # of events: "<<h->Integral()<<endl;
				rflux /= 1000000000.;
				//			variable_minus->Sumw2();
				variable_minus->Scale(1/(rflux));

				double ep,em;
				double Nm = variable_minus->IntegralAndError(-1000,1000,em);
				nmvec[id] = Nm ;
				emvec[id] = em ;
				if (ti ==1){
					vecm_MT[id] = Nm;
					emvec_MT[id] = em;
				}
				if (ti >1){
					vecm_div[id] = Nm/vecm_MT[id];
					emvec_div[id] = Nm/vecm_MT[id]*sqrt(em*em/(Nm*Nm) + emvec_MT[id]*emvec_MT[id]/(vecm_MT[id]*vecm_MT[id])); //Manual prop. of error
				}
			}
			//						cout<<tstr<<endl;
			//			cout<<id<<"  "<< r<<"  "<< npvec[id]<<"  "<< nmvec[id]<<endl;
			id++;
		}


		if( id > 0 ) {
			c.Clear();
			TGraph* grp = new TGraphErrors(id, xvec, npvec, 0, epvec);
			TGraph* grm = new TGraphErrors(id, xvec, nmvec, 0, emvec);

			grp->SetMinimum(0);
			//			gr1->SetMaximum(gr1->GetMaximum());
			grp->SetMarkerColor(4);
			sprintf(tstr,"Vertex number vs run %s", trignames[ti].c_str());
			grp->SetTitle(tstr);
			cout<<"Drawing graphs..."<<endl;
			grp->Draw("A*");
			grm->SetMarkerColor(2);
			grm->Draw("*same");
			sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
			c.SaveAs(tstr);
			if(ti>1){
				TGraph* grrp = new TGraphErrors(id, xvec, vecp_div, 0, epvec_div);
				TGraph* grrm = new TGraphErrors(id, xvec, vecm_div, 0, emvec_div);
				//				TGraph* grrp = new TGraph(id, xvec, vecp_div);
				//				TGraph* grrm = new TGraph(id, xvec, vecm_div);
				grrp->SetMinimum(0);
				//			gr1->SetMaximum(gr1->GetMaximum());
				grrp->SetMarkerColor(38);
				sprintf(tstr,"Vertex number vs run %s over MT", trignames[ti].c_str());
				grrp->SetTitle(tstr);
				cout<<"Drawing graphs..."<<endl;
				grrp->Draw("A*");
				grrm->SetMarkerColor(46);
				grrm->Draw("*same");
				sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
				c.SaveAs(tstr);
			}
		}
	}

	//return;

	//
			/*****************************************************
	 							Vertex Chi2
			 *****************************************************/
	//
	//		c.SetLogy(kTRUE);
	//		cout<<"Vertex Chi2 ci="<<ci<<endl;
	//		sprintf(tstr,"VtxChi2_%d_all", ci);
	//		TH1F* variable_plus  = histloader.GetTH1( tstr, 1 );
	//		TH1F* variable_minus = histloader.GetTH1( tstr, -1 );
	//		//variable_plus->Rebin(20);
	//		//variable_minus->Rebin(20);
	//		variable_plus->GetXaxis()->SetRangeUser(0,20);
	//		variable_minus->GetXaxis()->SetRangeUser(0,20);
	//		//variable_minus->Rebin(20);
	//		variable_plus->Scale(1/(flux_p));
	//		variable_minus->Scale(1/(flux_m));
	//
	//		variable_plus->SetLineColor(4); //blue
	//		variable_minus->SetLineColor(2); //red
	//
	//		variable_minus->Draw();
	//		variable_plus->Draw("same");
	//
	//		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	//		c.SaveAs(tstr);
	//		c.SetLogy(kFALSE);
	//
	//		TH1F* hrapport_plusoverminus = variable_plus;
	//		sprintf(tstr,"Vertex Chi2, #mu+ / #mu-");
	//		hrapport_plusoverminus->SetTitle(tstr);
	//		hrapport_plusoverminus->SetLineColor(8);
	//		hrapport_plusoverminus->Divide(variable_minus);
	//		hrapport_plusoverminus->SetMinimum(0.5);
	//		hrapport_plusoverminus->SetMaximum(2.0);
	//		hrapport_plusoverminus->Draw();
	//
	//		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	//		c.SaveAs(tstr);


	/*****************************************************
  	  	  	  	  	  	  Q2 distribution
	 *****************************************************/

	cout<<"Q2 distribution ci="<<ci<<endl;
	sprintf(tstr,"Q2");
	TH1F* variable_plus  = hlp.GetTH1( tstr, 1 );
	TH1F* variable_minus = hlm.GetTH1( tstr, -1 );
	//variable_plus->Rebin(20);
	//variable_minus->Rebin(20);
	variable_plus->Scale(1/(flux_p));
	variable_minus->Scale(1/(flux_m));

	variable_plus->SetLineColor(4); //blue
	variable_minus->SetLineColor(2); //red
	variable_plus->GetXaxis()->SetRangeUser(0.5,100);
	variable_minus->GetXaxis()->SetRangeUser(0.5,100);


	variable_minus->Draw();
	variable_plus->Draw("same");


	TPaveText *pt = new TPaveText(.8,.8,.95,.95, "NDC");
	double Np = variable_plus->Integral();
	double Nm = variable_minus->Integral();
	pt->AddText(TString::Format("Integral+: %.0f", Np));
	pt->AddText(TString::Format("Integral-: %.0f", Nm));
	pt->Draw();

	c.SetLogx(kTRUE);

	sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	c.SaveAs(tstr);

	c.SetLogy(kTRUE);
	sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	c.SaveAs(tstr);

	c.SetLogy(kFALSE);
	TH1F* hrapport_plusoverminus = variable_plus;
	sprintf(tstr,"Q^{2} distribution, #mu+ / #mu-");
	hrapport_plusoverminus->SetTitle(tstr);
	hrapport_plusoverminus->SetLineColor(8);
	hrapport_plusoverminus->Divide(variable_minus);
	hrapport_plusoverminus->SetMinimum(0.5);
	hrapport_plusoverminus->SetMaximum(1.5);

	sprintf(tstr,"fit-r");
	TF1* ffit_r = new TF1(tstr, "pol0(1)", -100, 100);
	ffit_r->SetParameter(0,1);
	ffit_r->SetParLimits(0,0.5,1.5);
	hrapport_plusoverminus->Fit(tstr, "N");
	hrapport_plusoverminus->Fit(tstr, "N");
	hrapport_plusoverminus->Fit(tstr, "N");
	hrapport_plusoverminus->Fit(tstr, "");
	hrapport_plusoverminus->Draw();
	hrapport_plusoverminus->Draw();
	TLine line(hrapport_plusoverminus->GetXaxis()->GetXmin(), 1, hrapport_plusoverminus->GetXaxis()->GetXmax(), 1);
	line.Draw("same");

	sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	c.SaveAs(tstr);
	c.SetLogx(kFALSE);
	cout<<"End of Q2 distribution ci="<<ci<<endl;

	/*
    sprintf(tstr,"hNGoodBeamEvt");
    TH1F* variable_plus  = histloader.GetTH1( tstr, 1 );
    TH1F* variable_minus = histloader.GetTH1( tstr, -1 );
    variable_plus->Scale(1/(flux_p));
    variable_minus->Scale(1/(flux_m));

    variable_plus->SetLineColor(4); //blue
    variable_minus->SetLineColor(2); //red

    variable_plus->Draw();
    variable_minus->Draw("same");

    sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
    c.SaveAs(tstr);

    TH1F* hrapport_plusoverminus = variable_plus;
    sprintf(tstr,"Vertex distribution, #mu+ / #mu-");
    hrapport_plusoverminus->SetTitle(tstr);
    hrapport_plusoverminus->SetLineColor(8);
    hrapport_plusoverminus->Divide(variable_minus);
    hrapport_plusoverminus->SetMinimum(0.5);
    hrapport_plusoverminus->SetMaximum(2.0);
    hrapport_plusoverminus->Draw();

    sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
    c.SaveAs(tstr);
	 */
	int imax = 4;

	//		cout<<"After trignames"<<endl;


	/*****************************************************
	  	  	  	      HIts in hodoscopes
	 *****************************************************/

	cout<<"hodoscopes"<<endl;
	imax = 4;
	for(int i = 1; i <= imax; i++) {

		for (int zi=0; zi<2; zi++){
			c.SetLogz(true);
			cout<<"trigger"<< i <<endl;
			sprintf(tstr,"MuXY_%s_%i", trignames[i].c_str(), zi);

			TH2F* h2_p = hlp.GetTH2( tstr, 1 );
			TH2F* h2_m = hlm.GetTH2( tstr, -1 );
			int r = 2;
			//			rebin2(h2_p,r,r);
			//			rebin2(h2_m,r,r);
			gStyle->SetPalette(51);
			h2_p->Scale(1/(flux_p));
			h2_m->Scale(1/(flux_m));
			double maxplus = h2_p->GetMaximum();
			double maxmins = h2_m->GetMaximum();
			double max = maxplus;
			if(maxmins > maxplus) max = maxmins;
			h2_p->SetMaximum(max);
			h2_p->SetMinimum(max/1000000);
			h2_m->SetMaximum(max);
			h2_m->SetMinimum(max/1000000);
			//		h2_p->SetMinimum(h2_p->GetMaximum()/100000);
			//		h2_m->SetMinimum(h2_m->GetMaximum()/100000);
			h2_p->Draw("colz");
			if(i==1 && zi==0)  plot_HM04_2016();
			if(i==1 && zi==1) plot_HM05_2016();
			if(i==2 && zi==0) plot_HL04_2016();
			if(i==2 && zi==1) plot_HL05_2016();
			if(i==3 && zi==0) plot_HO03_2016();
			if(i==3 && zi==1)  plot_HO04_2016();
			if(i==4 && zi==0) plot_HG01_2016();
			if(i==4 && zi==1)  plot_HG02_2016();

			sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
			c.SaveAs(tstr);
			h2_m->Draw("colz");
			if(i==1 && zi==0)  plot_HM04_2016();
			if(i==1 && zi==1) plot_HM05_2016();
			if(i==2 && zi==0) plot_HL04_2016();
			if(i==2 && zi==1) plot_HL05_2016();
			if(i==3 && zi==0) plot_HO03_2016();
			if(i==3 && zi==1)  plot_HO04_2016();
			if(i==4 && zi==0) plot_HG01_2016();
			if(i==4 && zi==1)  plot_HG02_2016();
			sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
			c.SaveAs(tstr);

			c.SetLogz(false);
			gStyle->SetPalette(nc, MyPalette);
			h2_p->Divide(h2_m);
			h2_p->SetMinimum(0.8);
			h2_p->SetMaximum(1.2);
			h2_p->Draw("colz");
			if(i==1 && zi==0)  plot_HM04_2016();
			if(i==1 && zi==1) plot_HM05_2016();
			if(i==2 && zi==0) plot_HL04_2016();
			if(i==2 && zi==1) plot_HL05_2016();
			if(i==3 && zi==0) plot_HO03_2016();
			if(i==3 && zi==1)  plot_HO04_2016();
			if(i==4 && zi==0) plot_HG01_2016();
			if(i==4 && zi==1)  plot_HG02_2016();
			sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
			c.SaveAs(tstr);
		}
	}
	cout<<"end of hodoscopes"<<endl;





	/*****************************************************
	  	  	  	      HIts in hodoscopes all beam
	 *****************************************************/
	/*
	cout<<"hodoscopes"<<endl;

	for(int i = 0; i <= imax; i++) {
		c.SetLogz(true);
		cout<<"trigger"<< i <<endl;
		sprintf(tstr,"MuXYAll_%s", trignames[i].c_str());
		// à rechanger "MuXY_%s" voir user event 1
		TH2F* h2_p = histloader.GetTH2( tstr, 1 );
		TH2F* h2_m = histloader.GetTH2( tstr, -1 );
		int r = 5;
		rebin2(h2_p,r,r);
		rebin2(h2_m,r,r);
		gStyle->SetPalette(51);
		h2_p->Scale(1/(flux_p));
		h2_m->Scale(1/(flux_m));
		double maxplus = h2_p->GetMaximum();
		double maxmins = h2_m->GetMaximum();
		double max = maxplus;
		if(maxmins > maxplus) max = maxmins;
		h2_p->SetMaximum(max);
		h2_p->SetMinimum(max/1000000);
		h2_m->SetMaximum(max);
		h2_m->SetMinimum(max/1000000);
		//		h2_p->SetMinimum(h2_p->GetMaximum()/100000);
		//		h2_m->SetMinimum(h2_m->GetMaximum()/100000);
		h2_p->Draw("colz");
		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
		h2_m->Draw("colz");
		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);

		c.SetLogz(false);
		gStyle->SetPalette(nc, MyPalette);
		h2_p->Divide(h2_m);
		h2_p->SetMinimum(0.8);
		h2_p->SetMaximum(1.2);
		h2_p->Draw("colz");
		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
	}
	cout<<"end of hodoscopes"<<endl;
	/*****************************************************
			  	  nu distribution
	 *****************************************************/

	for(int i = 0; i <= imax; i++) {
		cout<<"nu distribution"<<"  i="<<i<<endl;
		c.SetLogx(kFALSE);
		/**/
		sprintf(tstr,"nu_%s_%i", trignames[i].c_str(),cut);
		TH1F* variable_plus  = hlp.GetTH1( tstr, 1 );
		TH1F* variable_minus = hlm.GetTH1( tstr, -1 );
		cout<<tstr<<" "<<variable_plus<<endl;
		//		variable_plus->Rebin(20);
		//		variable_minus->Rebin(20);
		variable_plus->Scale(1/(flux_p));
		variable_minus->Scale(1/(flux_m));

		variable_plus->SetLineColor(4); //blue
		variable_minus->SetLineColor(2); //red

		variable_plus->Draw();
		variable_minus->Draw("same");


		TPaveText *pt = new TPaveText(.8,.8,.95,.95, "NDC");
		double Np = variable_plus->Integral();
		double Nm = variable_minus->Integral();
		pt->AddText(TString::Format("Integral+: %.0f", Np));
		pt->AddText(TString::Format("Integral-: %.0f", Nm));
		pt->Draw();

		//if( i== 0 )
		//  sprintf(tstr,"plots/dis-%d-%d.pdf(", run1, run2);
		//else
		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);

		TH1F* hrapport_plusoverminus = variable_plus;
		sprintf(tstr,"#nu distribution (%s), #mu+ / #mu-", trignames[i].c_str());
		hrapport_plusoverminus->SetTitle(tstr);
		hrapport_plusoverminus->SetLineColor(8);
		hrapport_plusoverminus->Divide(variable_minus);
		hrapport_plusoverminus->SetMinimum(0.9);
		hrapport_plusoverminus->SetMaximum(1.1);
		sprintf(tstr,"fit-r");
		TF1* ffit_r = new TF1(tstr, "pol0(0)", -100, 100);
		ffit_r->SetParameter(0,1);
		ffit_r->SetParLimits(0,0.5,1.5);
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "");
		hrapport_plusoverminus->Draw();
		hrapport_plusoverminus->Draw();
		TLine line(hrapport_plusoverminus->GetXaxis()->GetXmin(), 1, hrapport_plusoverminus->GetXaxis()->GetXmax(), 1);
		line.Draw("same");
		double meanvalue = ffit_r->GetParameter(0);
		double sigma = ffit_r->GetParError(0);
		TLine lineup(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue+sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue+sigma);
		lineup.SetLineStyle(2);
		lineup.Draw("same");
		TLine linedw(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue-sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue-sigma);
		linedw.SetLineStyle(2);
		linedw.Draw("same");

		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
	}

	/*****************************************************
			  	xb distribution direct
	 *****************************************************/
	for(int i = 0; i <= imax; i++) {

		cout<<"xBj distribution"<<"  i="<<i<<endl;
		c.SetLogx(kTRUE);
		sprintf(tstr,"xBj_%s_%i", trignames[i].c_str(),cut);

		TH1F* variable_plus  = hlp.GetTH1( tstr, 1 );
		TH1F* variable_minus = hlm.GetTH1( tstr, -1 );
		//		variable_plus->Rebin(5);
		//		variable_minus->Rebin(5);
		variable_plus->Scale(1/(flux_p));
		variable_minus->Scale(1/(flux_m));


		variable_plus->SetLineColor(4); //blue
		variable_minus->SetLineColor(2); //red


		variable_plus->Draw();
		variable_minus->Draw("same");

		TPaveText *pt = new TPaveText(.8,.8,.95,.95, "NDC");
		double Np = variable_plus->Integral();
		double Nm = variable_minus->Integral();
		pt->AddText(TString::Format("Integral+: %.0f", Np));
		pt->AddText(TString::Format("Integral-: %.0f", Nm));
		pt->Draw();


		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);

		TH1F* hrapport_plusoverminus = variable_plus;
		sprintf(tstr,"x_{Bj} distribution (%s), #mu+ / #mu-", trignames[i].c_str());
		hrapport_plusoverminus->SetTitle(tstr);
		hrapport_plusoverminus->SetLineColor(8);
		hrapport_plusoverminus->Divide(variable_minus);
		hrapport_plusoverminus->SetMinimum(0.9);
		hrapport_plusoverminus->SetMaximum(1.1);
		sprintf(tstr,"fit-r");
		TF1* ffit_r = new TF1(tstr, "pol0(0)", -100, 100);
		ffit_r->SetParameter(0,1);
		ffit_r->SetParLimits(0,0.5,1.5);
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "");
		hrapport_plusoverminus->Draw();
		hrapport_plusoverminus->Draw();
		TLine line(hrapport_plusoverminus->GetXaxis()->GetXmin(), 1, hrapport_plusoverminus->GetXaxis()->GetXmax(), 1);
		line.Draw("same");
		double meanvalue = ffit_r->GetParameter(0);
		double sigma = ffit_r->GetParError(0);
		TLine lineup(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue+sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue+sigma);
		lineup.SetLineStyle(2);
		lineup.Draw("same");
		TLine linedw(hrapport_plusoverminus->GetXaxis()->GetXmin(), meanvalue-sigma, hrapport_plusoverminus->GetXaxis()->GetXmax(), meanvalue-sigma);
		linedw.SetLineStyle(2);
		linedw.Draw("same");
		//if( i == 3 )
		//  sprintf(tstr,"plots/dis-%d-%d.pdf)", run1, run2);
		//else
		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
		c.SetLogx(kFALSE);
	}
	/**/
	/*
	c.SetLogx(kTRUE);
	sprintf(tstr,"xBj_%s", trignames[i].c_str());
	TH1F* variable_plus  = histloader.GetTH1( tstr );
	sprintf(tstr,"xBj2_%s", trignames[i].c_str());
	TH1F* variable_minus = histloader.GetTH1( tstr );
	variable_plus->Rebin(5);
	variable_minus->Rebin(5);
	//variable_plus->Scale(1/(1.02935e+10));
	//variable_minus->Scale(1/(4.56566e+09));

	variable_plus->SetLineColor(4); //blue
	variable_minus->SetLineColor(2); //red

	variable_plus->Draw();
	variable_minus->Draw("same");

	if( i == 0 )
	sprintf(tstr,"plots/dis-%d-%d.pdf(", run1, run2);
	else if( i == 3 )
	sprintf(tstr,"plots/dis-%d-%d.pdf)", run1, run2);
	else
	sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
	c.SaveAs(tstr);
	 */
	/**/


	/*****************************************************
			            	  Mass K0
	 *****************************************************/

	for(int i = 0; i <= imax; i++) {
		cout<<"MK0 distribution"<<"  i="<<i<<endl;
		sprintf(tstr,"MK0_%s", trignames[i].c_str());

		TH1F* variable_plus  = hlp.GetTH1( tstr, 1 );
		sprintf(tstr,"MK0_%s", trignames[i].c_str());
		TH1F* variable_minus = hlm.GetTH1( tstr, -1 );
		variable_plus->Rebin(5);
		variable_minus->Rebin(5);
		variable_plus->Scale(1/(flux_p));
		variable_minus->Scale(1/(flux_m));

		sprintf(tstr,"fit_MK0_%d_%s_p", ci, trignames[i].c_str());
		TF1* fit_p = new TF1(tstr, "gausn + pol1(3)", 420, 600);
		fit_p->SetParameter(0,100);
		fit_p->SetParameter(1,490);
		fit_p->SetParLimits(1,450,500);
		fit_p->SetParameter(2,10);
		fit_p->SetParLimits(2,5,100);
		variable_plus->Fit(tstr, "QRN");
		fit_p->SetParLimits(1,400,600);
		fit_p->SetParLimits(2,1,100);
		variable_plus->Fit(tstr, "QRN");
		fit_p->SetParLimits(2,0.01,100);
		variable_plus->Fit(tstr, "QR");

		sprintf(tstr,"fit_MK0_%d_%s_m", ci, trignames[i].c_str());
		TF1* fit_m = new TF1(tstr, "gausn + pol1(3)", 400, 600);
		fit_m->SetParameter(0,100);
		fit_m->SetParameter(1,490);
		fit_m->SetParLimits(1,450,500);
		fit_m->SetParameter(2,10);
		fit_m->SetParLimits(2,5,100);
		variable_minus->Fit(tstr, "QRN");
		fit_m->SetParLimits(1,400,600);
		fit_m->SetParLimits(2,1,100);
		variable_minus->Fit(tstr, "QRN");
		fit_m->SetParLimits(2,0.01,100);
		variable_minus->Fit(tstr, "QR");

		variable_plus->SetLineColor(4); //blue
		variable_minus->SetLineColor(2); //red

		variable_minus->SetMinimum(0);
		variable_minus->Draw();
		variable_plus->Draw("same");
		TLine line(497.614,0,497.614,variable_minus->GetMaximum());
		line.Draw("same");



		TPaveText *pt = new TPaveText(.65,.7,.95,.9, "NDC");
		double Np = fit_p->GetParameter(0);
		double Nm = fit_m->GetParameter(0);
		pt->AddText(TString::Format("N+: %.2f", Np/variable_plus->GetXaxis()->GetBinWidth(1)));
		pt->AddText(TString::Format("N-: %.2f", Nm/variable_minus->GetXaxis()->GetBinWidth(1)));
		pt->AddText(TString::Format("N+/N-: %.2f", Np/Nm));
		pt->AddText(TString::Format("M+: %.1f #pm %.1f", fit_p->GetParameter(1), fit_p->GetParError(1)));
		pt->AddText(TString::Format("M-: %.1f #pm %.1f", fit_m->GetParameter(1), fit_m->GetParError(1)));
		pt->AddText(TString::Format("#sigma+: %.1f #pm %.1f", fit_p->GetParameter(2), fit_p->GetParError(2)));
		pt->AddText(TString::Format("#sigma-: %.1f #pm %.1f", fit_m->GetParameter(2), fit_m->GetParError(2)));
		pt->Draw();


		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);

		cout<<"N K0 +: "<<fit_p->GetParameter(0)<<" N K0 -: "<<fit_m->GetParameter(0)	 <<"     ratio: "<<fit_p->GetParameter(0)/fit_m->GetParameter(0)<<endl;

		TH1F* hrapport_plusoverminus = variable_plus;
		sprintf(tstr,"M_{K^{0}} distribution (%s), #mu+ / #mu-", trignames[i].c_str());
		hrapport_plusoverminus->SetTitle(tstr);
		hrapport_plusoverminus->SetLineColor(8);
		hrapport_plusoverminus->Divide(variable_minus);
		hrapport_plusoverminus->SetMinimum(0.5);
		hrapport_plusoverminus->SetMaximum(1.5);
		sprintf(tstr,"fit-r");
		TF1* ffit_r = new TF1(tstr, "pol0(0)", -100, 100);
		ffit_r->SetParameter(0,1);
		ffit_r->SetParLimits(0,0.5,1.5);
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "N");
		hrapport_plusoverminus->Fit(tstr, "");
		hrapport_plusoverminus->Draw();
		hrapport_plusoverminus->Draw();
		TLine line2(hrapport_plusoverminus->GetXaxis()->GetXmin(), 1, hrapport_plusoverminus->GetXaxis()->GetXmax(), 1);
		line2.Draw("same");

		cout<<"ci="<<ci<<"  i="<<i<<endl;

		sprintf(tstr,"plots/dis-%d-%d.pdf", run1, run2);
		c.SaveAs(tstr);
	}

	sprintf(tstr,"plots/dis-%d-%d.pdf)", run1, run2);
	c.SaveAs(tstr);

	return 0;

}
