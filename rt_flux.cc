//#define pi 3.141592653589793238
//#include HistLoader.h
#include <string>
#include "include/rt_flux.h"

Double_t pi = TMath::Pi();

// #ifdef __CLING__
// // Tell  ROOT where to find AliRoot headers
// //R__ADD_INCLUDE_PATH("/afs/cern.ch/compass/dvcs/Production/Analysis/Students/avidon/rootscripts")
// #include "include/HistLoader.h"
// #include "include/rt_flux.h"
//
// #endif


int iTHW = 3;
Float_t THW[8] = {0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0};
Float_t TWO[8] = {0,0,0,0,0,0,0,0};

void* hl;

Double_t Bestt_1[400];
Double_t BestN_RT_1[400];
Double_t Bestt_2[400];
Double_t BestN_RT_2[400];
Double_t Bestt_0[400];
Double_t BestN_RT_0[400];
Double_t Bestt_end[400];
Double_t BestN_RT_end[400];

Double_t BestN_VDT_num_1[400];
Double_t BestN_VDT_num_2[400];
Double_t BestN_VDT_den_1[400];
Double_t BestN_VDT_den_2[400];

Double_t BestN_trig_1[400];
Double_t BestN_trig_2[400];

Float_t Flux[8][400];
Float_t eFlux[8][400];

Float_t Flux_nodaqdt[8][400];
Float_t eFlux_nodaqdt[8][400];

Float_t Flux_withvtdt[8][400];
Float_t eFlux_withvtdt[8][400];

TH1F* hRTAccepted;
TH1F* hRTAttempts;
TH1F* hTBeam;

int beam_charge = 0;
float spill_tmin = 0;
float spill_tmax = 0;
//float spill_tmin = 1.5;
//float spill_tmax = 5.0;


// E. Fuchey, 21/09/2015:
//
// This file contains the routine FluxAnaOfficial_Slot3(const Int_t RunNum, const Int_t Charge,
//		     const Float_t OffMin = 0.2, const Float_t OffMax = 0.8,
//		     const char* Opt = "Official", const Int_t slot=2, Bool_t Save=kTRUE)
// where RunNum is the run number one wants to calculate the flux;
//       Charge is the beam charge for the run;
//       OffMin and OffMax correspond to the resp. first and last analysis windows (delta t)
//       defined by a different apertures, 1, 2, 3,... ns, centered on 0
//       The flux is calculated for many analysis windows, and fitted over a stability interval,
//       which is fixed between OffMin and OffMax.
//       Values are set by default to select 2ns only (for a 5ns coral window),
//       and shall be changed for a wider coral window.
//       (see presentation on flux at Analysis Meeting from 2013/11/29 for more detail on this)
//       Opt is the name of the production, "Official" by default;
//       Slot is the slot number: 3 by default
//       Save is a boolean which allows to decides if the result plot is saved or not.
// This macro calculates the flux, spill by spill, for the selected run, for 'slot3' (=t15,t11,t12,t13,t14) production.
// Output gives: spill number, flux, flux_error, qualityflag
// Then it gives the mean flux per spill.
// The routine 'FluxAnaOfficial(...)' has many dependances:
// * GetFluxesFromHistos(...) which calculates the flux from the root files, spill by spill, for each delta t,
//  and write them in a text file;
// * PlotFlux_Run_fSpillNb_AWW(...) which uses the text file created by GetFluxesFromHistos(...),
// and plots the flux for each spill and each window, and evaluates the total mean flux per spill for the run.
//
// This file also contains useful 2 routines that are not directly used for flux calcuation:
// * PlotNRTAtt(const Int_t RunNum, const Int_t slot=2)
//   which plots for the selected run the number of attempted trigger as a function of the spill number,
//   for t_0 (0.2s), t_1 (1.0ns), t_2 (10.4ns), t_end (10.488ns).
// * PlotNRTAcc_Slot2Vs1(...) which compares the number of accepted random triggers for two productions

// CombineFiles(const int RunNum, const Int_t Nsplits = 100, const Int_t Nspills = 200)
//       RunNum is the number of the run you want to combine the file;
//       Nsplits the number of files in which the processed run has been splitted;
//       Nspills the number of spills in the run -> always 200.
// As indicated in the README file in this directory,
// this routine combines the rt_attempt-{$Runnum}.dat
// with the rt_attempts-{$RunNum}-{i_split}.dat produced by user event 31
// The reuse, the paths on line 61 and 105 have to be modified

void CombineFiles(const int RunNum, const Int_t Nsplits = 1000, const Int_t Nspills = 200)
{
	ifstream *in[Nsplits];

	Int_t i_split = 0;

	Int_t SpillNbr = 0;
	Double_t t_1 = 0.0;
	Double_t N_RT_1 = 0.0;
	Double_t t_2 = 0.0;
	Double_t N_RT_2 = 0.0;
	Double_t t_0 = 0.0;
	Double_t N_RT_0 = 0.0;
	Double_t t_end = 0.0;
	Double_t N_RT_end = 0.0;
	Double_t N_VDT_num_1 = 0.0;
	Double_t N_VDT_num_2 = 0.0;
	Double_t N_VDT_den_1 = 0.0;
	Double_t N_VDT_den_2 = 0.0;

	Double_t N_trig_1 = 0.0;
	Double_t N_trig_2 = 0.0;

	//Double_t t1Ref = 2.0;
	//Double_t t2Ref = 10.0;
	//Double_t t1Ref = 1.0;
	//Double_t t2Ref = 10.4;
	Double_t t1Ref = spill_tmin;
	Double_t t2Ref = spill_tmax;

	for(Int_t j = 0; j<Nspills; j++) {
		Bestt_1[j] = 0.0;
		BestN_RT_1[j] = 0.0;
		Bestt_2[j] = 100.0;
		BestN_RT_2[j] = 0.0;
		Bestt_0[j] = 100.0;
		BestN_RT_0[j] = 0.0;
		Bestt_end[j] = 0.0;
		BestN_RT_end[j] = 0.0;
	}

	for(Int_t i = 0; i<Nsplits; i++) {
		i_split = i+1;
		//cout << i << " " << i_split << endl;

		in[i] = new ifstream(Form("/afs/cern.ch/compass/dvcs/Production/Analysis/Students/avidon/outputs/2016/phast/%d/rt_attempts-%d.dat",RunNum,i_split));
//		    in[i] = new ifstream(Form("/afs/cern.ch/work/a/avidon/outputs/%d/rt_attempts-%d.dat",RunNum,i_split));
		//    in[i] = new ifstream(Form("/afs/cern.ch/work/a/avidon/outputs/P09t4_QcutTcutCameracalib5cm/%d/rt_attempts-%d.dat",RunNum,i_split));



		if(!in[i]->good()) {
			//cout << "file does not exist" << endl;
			continue;
		}
		//cout<<"Reading RT attempts from "<<Form("../outputs/2016/phast/%d/rt_attempts-%d.dat",RunNum,i_split)<<endl;

		for(Int_t j = 0; j<Nspills; j++) {
			char line[1001];
			in[i]->getline(line, 1000);
			istringstream instr(line);
			/**in[i]*/ instr >> SpillNbr >> t_1 >> N_RT_1 >> t_2 >> N_RT_2 >> t_0 >> N_RT_0 >> t_end >> N_RT_end >> N_VDT_num_1 >> N_VDT_num_2 >> N_VDT_den_1 >> N_VDT_den_2;// >> N_trig_1 >> N_trig_2;

			//cout << j << " " << SpillNbr << endl;

			if(fabs(t_1-t1Ref)<fabs(Bestt_1[j]-t1Ref)) {
				//cout << j << " " << SpillNbr << " " << Bestt_1[j] << " " << t_1 << " " << BestN_RT_1[j]  << " " << N_RT_1 << endl;

				Bestt_1[j] = t_1;
				BestN_RT_1[j] = N_RT_1;
				BestN_VDT_num_1[j] = N_VDT_num_1;
				BestN_VDT_den_1[j] = N_VDT_den_1;
				BestN_trig_1[j] = N_trig_1;
			}

			//if(j==29) cout<<"Bestt_2="<<Bestt_2[j]<<"  t2Ref="<<t2Ref<<"  t_2="<<t_2<<endl;
			if(fabs(t_2-t2Ref)<fabs(Bestt_2[j]-t2Ref)) {
				Bestt_2[j] = t_2;
				BestN_RT_2[j] = N_RT_2;
				BestN_VDT_num_2[j] = N_VDT_num_2;
				BestN_VDT_den_2[j] = N_VDT_den_2;
				BestN_trig_2[j] = N_trig_2;
			}

			if(t_0<Bestt_0[j]) {
				Bestt_0[j] = t_0;
				BestN_RT_0[j] = N_RT_0;
			}

			if(t_end>Bestt_end[j]) {
				Bestt_end[j] = t_end;
				BestN_RT_end[j] = N_RT_end;
			}
			//if(j==29)cout<<"spill: "<< j << "  BestN_RT_1="<<BestN_RT_1[j]<<"  BestN_RT_2="<<BestN_RT_2[j]<< "  diff="<<(BestN_RT_2[j]-BestN_RT_1[j])<<"  t_1="<<Bestt_1[j]<<"  t_2="<<Bestt_2[j]<< endl;
		}

		in[i]->close();
	}
}


void PlotFluxHistos(const Int_t RunNum)
{
	char tstr[500];
	TCanvas c("c","c",800,600);

	HistLoader* histloader = (HistLoader*)hl;

	TH1F* hChunks = histloader->GetTH1("hChunks");
	hChunks->Draw("col");
	sprintf(tstr,"plots/rt_flux-%d.pdf(",(int)RunNum);
	c.SaveAs(tstr);

	TH1F* hNevtVsChunk = histloader->GetTH1("hNevtVsChunk");
	hNevtVsChunk->SetTitle("Chunk-wise number of RT events");
	hNevtVsChunk->Draw("colz");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	TH1F* hTimeSpill = histloader->GetTH1("TimeSpill");
	hTimeSpill->Draw();
	TLine l1(spill_tmin, 0, spill_tmin, hTimeSpill->GetMaximum());
	TLine l2(spill_tmax, 0, spill_tmax, hTimeSpill->GetMaximum());
	l1.SetLineColor(2); l2.SetLineColor(2);
	l1.Draw("same"); l2.Draw("same");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	TH1F* hNTrigAccepted;
	hNTrigAccepted = histloader->GetTH1("NTrigAccepted");
	hNTrigAccepted->Draw();
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	TH1F* hNRTAccepted = histloader->GetTH1("NRTAccepted");
	TH1F*	hNRTAtempts = new TH1F("NRTAtempts", "RT attempts",
			hNRTAccepted->GetXaxis()->GetNbins(),
			hNRTAccepted->GetXaxis()->GetXmin(),
			hNRTAccepted->GetXaxis()->GetXmax());
	TH1F* hProcessedEvents = histloader->GetTH1("ProcessedEvents");
	//TH1F* hNRTAttempted = new TH1F("NRTAttempted","Rescaled accepted random triggers",151,-0.5,150.5);
	TH2F* hBeamMeanTimeVsSpillNum = histloader->GetTH2("BeamMeanTimeVsSpillNum");

	TH1F* hBeamMeanTime = (TH1F*)hBeamMeanTimeVsSpillNum->ProjectionY("BeamMeanTime");
	hBeamMeanTime->SetTitle("Mean beam time (all spills)");

	hNRTAccepted->Draw();
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	hBeamMeanTime->Draw();
	TLine l3(-THW[iTHW], 0, -THW[iTHW], hBeamMeanTime->GetMaximum());
	TLine l4(THW[iTHW], 0, THW[iTHW], hBeamMeanTime->GetMaximum());
	l3.SetLineColor(2); l4.SetLineColor(2);
	l3.Draw("same"); l4.Draw("same");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	const Int_t NSpill = 200;

	Float_t spill_vec[NSpill];
	Float_t att_vec[NSpill];
	Float_t acc_vec[NSpill];
	for(Int_t iSpill = 0; iSpill<NSpill; iSpill++) {
		spill_vec[iSpill] = iSpill + 1;
		att_vec[iSpill] = BestN_RT_2[iSpill] - BestN_RT_1[iSpill];
		int SpillNb_Bin = hNRTAccepted->GetXaxis()->FindBin(iSpill+1);
		acc_vec[iSpill] = hNRTAccepted->GetBinContent(SpillNb_Bin);
		cout<<"spill="<<iSpill<<"  attempted="<<att_vec[iSpill]<<"  accepted="<<acc_vec[iSpill]<<endl;
	}

	TGraph gr1(NSpill, spill_vec, att_vec);
	TGraph gr2(NSpill, spill_vec, acc_vec);

	//cout<<"att_vec[0]: "<<att_vec[0]<<"    acc_vec[0]: "<<acc_vec[0]<<endl;

	gr1.SetMarkerColor(2);
	gr2.SetMarkerColor(4);
	gr1.Draw("A*");
	gr2.Draw("*same");
	gr1.SetMinimum(0);
	gr1.SetTitle("Number of attempted(red) and accepted(blue) random triggers per spill");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	Float_t daq_lt[NSpill];
	Float_t daq_lt2[NSpill];
	Float_t edaq_lt[NSpill];
	for(int i = 0; i < NSpill; i++) {
		if(Flux_nodaqdt[iTHW][i] > 0) {
			daq_lt[i] = Flux[iTHW][i]/Flux_nodaqdt[iTHW][i];
			/**/
			int SpillNb_Bin = hNTrigAccepted->GetXaxis()->FindBin(i+1);
			float N_Trigacc = hNTrigAccepted->GetBinContent(SpillNb_Bin);
			float diff = BestN_trig_2[i] - BestN_trig_1[i];
			daq_lt2[i] = N_Trigacc / diff;
			/**//*
      int SpillNb_Bin = hNRTAccepted->GetXaxis()->FindBin(i+1);
      float N_RTacc = hNRTAccepted->GetBinContent(SpillNb_Bin);
      float diff = BestN_RT_2[i] - BestN_RT_1[i];
      daq_lt2[i] = N_RTacc / diff;
			 */
			//cout<<"BestN_trig_1["<<i<<"]="<<BestN_trig_1[i];
			//cout<<"    BestN_trig_2["<<i<<"]="<<BestN_trig_2[i]<<endl;
		} else {
			daq_lt[i] = 1;
			daq_lt2[i] = 1;
			edaq_lt[i] = 0;
		}
	}
	TGraphErrors gr4(NSpill, spill_vec, daq_lt, NULL, NULL);
	TGraphErrors gr42(NSpill, spill_vec, daq_lt2, NULL, NULL);
	gr4.Draw("A*");
	gr42.SetMarkerColor(2);
	gr42.SetLineColor(2);
	gr42.Draw("*same");
	//gr4.SetMinimum(0);
	//gr4.SetMaximum(1);
	gr4.SetTitle("DAQ live-time");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);



	Float_t veto_lt[NSpill];
	Float_t eveto_lt[NSpill];
	for(int i = 0; i < NSpill; i++) {
		float num = BestN_VDT_num_2[i] - BestN_VDT_num_1[i];
		float den = BestN_VDT_den_2[i] - BestN_VDT_den_1[i];
		if(den > 0) {
			veto_lt[i] = num/den;
		} else {
			veto_lt[i] = 1;
			eveto_lt[i] = 0;
		}
	}
	TGraphErrors gr4b(NSpill, spill_vec, veto_lt, NULL, NULL);
	gr4b.Draw("A*");
	//gr4b.SetMinimum(0);
	//gr4b.SetMaximum(1);
	gr4b.SetTitle("Veto live-time");
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);



	TGraphErrors gr31(NSpill, spill_vec, Flux_nodaqdt[iTHW], NULL, eFlux_nodaqdt[iTHW]);
	TGraphErrors gr32(NSpill, spill_vec, Flux[iTHW], NULL, eFlux[iTHW]);
	TGraphErrors gr33(NSpill, spill_vec, Flux_withvtdt[iTHW], NULL, eFlux_withvtdt[iTHW]);
	gr31.SetMarkerColor(2);
	gr32.SetMarkerColor(4);
	gr33.SetMarkerColor(3);
	gr31.SetLineColor(2);
	gr32.SetLineColor(4);
	gr33.SetLineColor(3);
	gr31.Draw("A*");
	gr32.Draw("*same");
	gr33.Draw("*same");
	gr31.SetMinimum(0);
	gr31.SetTitle(TString::Format("Spill-by-spill flux values - window #pm %0.1f ns", THW[iTHW]));
	sprintf(tstr,"plots/rt_flux-%d.pdf",(int)RunNum);
	c.SaveAs(tstr);

	Float_t flux_tot[8];
	Float_t eflux_tot[8];
	Float_t flux_max = 0;
	for(int k = 0; k < 8; k++) {
		flux_tot[k] = 0;
		eflux_tot[k] = 0;
		for(int i = 0; i < NSpill; i++) {
			flux_tot[k] += Flux[k][i];
			eflux_tot[k] += eFlux[k][i] * eFlux[k][i];
			//flux_tot[k] += Flux_withvtdt[k][i];
			//eflux_tot[k] += eFlux_withvtdt[k][i] * eFlux_withvtdt[k][i];
			if( k == 3 ) cout<<"spill "<<i+1<<"  flux: "<<Flux_withvtdt[k][i]<<" +/- "<<eFlux_withvtdt[k][i]<<" ("<<eFlux_withvtdt[k][i]/Flux_withvtdt[k][i]<<")  flux (no DAQ DT): "<<Flux_nodaqdt[k][i]<<" +/- "<<eFlux_nodaqdt[k][i]<<endl;
		}
		if( flux_tot[k] > flux_max) flux_max = flux_tot[k];
		eflux_tot[k] = TMath::Sqrt( eflux_tot[k] );
		cout<<"THW="<<THW[k]<<"  flux: "<<flux_tot[k]<<" +/- "<<eflux_tot[k]<<endl;
	}

	TGraphErrors gr5(8, THW, flux_tot, NULL, eflux_tot);
	gr5.Draw("AP");
	gr5.SetMinimum(flux_max/2);
	gr5.SetTitle("Measured flux vs. half widow size");
	sprintf(tstr,"plots/rt_flux-%d.pdf)",(int)RunNum);
	c.SaveAs(tstr);

}





Int_t GetFluxesFromHistos(ofstream &fout, const Int_t RunNum, const char* Opt = "Official", Int_t k=0,
		Float_t SpillStartTime = 2.0,  Float_t SpillStopTime = 10.0, const Int_t slot=2)
{
	HistLoader* histloader = (HistLoader*)hl;

	SpillStartTime = spill_tmin;
	SpillStopTime = spill_tmax;
	Float_t TimeHalfWindow = THW[k];
	Float_t TimeWindowOffset = TWO[k];

	TH1F* hNRTAccepted = histloader->GetTH1("NRTAccepted");
	TH1F* hProcessedEvents = histloader->GetTH1("ProcessedEvents");
	//TH1F* hNRTAttempted = new TH1F("NRTAttempted","Rescaled accepted random triggers",151,-0.5,150.5);
	TH2F* hBeamMeanTimeVsSpillNum = histloader->GetTH2("BeamMeanTimeVsSpillNum");

	const Int_t NSpill = 200;
	Int_t SpillNb = 0;
	Int_t SpillNb_Bin = 0;
	Int_t NSpill_eff = NSpill;
	Float_t N_RTacc = 0;
	Float_t N_Procs = 0;
	//Int_t N_RTatt = 0;
	Float_t N_RTatt = 0;

	Int_t WindowMin_Bin = 0;
	Int_t WindowMax_Bin = 0;

	Float_t N_Trk_std = 0.0;
	Float_t F_std = 0.0;
	Float_t ErrF_std = 0.0;
	Float_t F_nodt = 0.0;
	Float_t ErrF_nodt = 0.0;

	TH1F* hBeamMeanTime_Spill;

	Float_t N_RTatt_array[NSpill];
	Float_t t_1 = 0.0;
	Float_t N_RT_1 = 0.0;
	Float_t t_2 = 0.0;
	Float_t N_RT_2 = 0.0;
	Float_t t_0 = 0.0;
	Float_t N_RT_0 = 0.0;
	Float_t t_end = 0.0;
	Float_t N_RT_end = 0.0;
	ifstream in_RT;

	for(Int_t iSpill = 0; iSpill<NSpill; iSpill++) {
		Flux[k][iSpill] = 0.0;
		Flux_nodaqdt[k][iSpill] = 0.0;
		Flux_withvtdt[k][iSpill] = 0.0;
		N_RTatt_array[iSpill] = 0.0;
		// in_RT >> SpillNb >> t_1 >> N_RT_1 >> t_2 >> N_RT_2 >> t_0 >> N_RT_0 >> t_end >> N_RT_end;
		SpillNb = iSpill+1;
		t_1 = Bestt_1[iSpill];
		N_RT_1 = BestN_RT_1[iSpill];
		t_2 = Bestt_2[iSpill];
		N_RT_2 = BestN_RT_2[iSpill];
		t_0 = Bestt_0[iSpill];
		N_RT_0 = BestN_RT_0[iSpill];
		t_end = Bestt_end[iSpill];
		N_RT_end = BestN_RT_end[iSpill];

		N_RTatt_array[iSpill] = (N_RT_2-N_RT_1)/(t_2-t_1);
		//N_RTatt_array[iSpill] = (N_RT_end-N_RT_0)*(t_2-t_1)/(t_end-t_0);
		//N_RTatt_array[iSpill] = (N_RT_end-N_RT_0)/(t_end-t_0);//NRTAtt/s!!!
		//if(t_end<5.0)N_RTatt_array[iSpill] = 1.0e6;
		if(t_0==100.0 || t_0==10.0)N_RTatt_array[iSpill] = 0.0;
		//if(N_RT_0>10000.0 || N_RT_1>10000.0)N_RTatt_array[iSpill] = N_RTatt_array[iSpill]*5.0e4;
		if(N_RT_0>100000.0 || N_RT_1>1000000.0)N_RTatt_array[iSpill] = N_RTatt_array[iSpill]*5.0e7;

		//cout<<"spill: "<< SpillNb << "  N_RT_1="<<N_RT_1<<"  N_RT_2="<<N_RT_2<< "  diff="<<(N_RT_2-N_RT_1)<<"  t_1="<<t_1<<"  t_2="<<t_2<<"  N_RTatt_array[iSpill]=" << N_RTatt_array[iSpill] << endl;
		//hNRTAttempted->Fill(iSpill+1, t_end);
	}

	SpillNb = 0;

	WindowMin_Bin = hBeamMeanTimeVsSpillNum->GetYaxis()->FindBin(-TimeHalfWindow+TimeWindowOffset);
	WindowMax_Bin = hBeamMeanTimeVsSpillNum->GetYaxis()->FindBin(+TimeHalfWindow+TimeWindowOffset);

	WindowMax_Bin = WindowMax_Bin-1;//for
	//   cout << WindowMin_Bin <<  " "  << WindowMax_Bin << endl;
	//   cout << WindowMin_Bin <<  " "  << WindowMax_Bin << endl;

	double F_std_tot = 0;

	for(Int_t iSpill = 0; iSpill<NSpill; iSpill++) {
		SpillNb = iSpill+1;
		SpillNb_Bin = hNRTAccepted->GetXaxis()->FindBin(SpillNb);
		N_RTacc = hNRTAccepted->GetBinContent(SpillNb_Bin)/(SpillStopTime-SpillStartTime);
		N_Procs = hProcessedEvents->GetBinContent(SpillNb_Bin)/(SpillStopTime-SpillStartTime);
		N_RTatt = N_RTatt_array[iSpill];
		//N_RTatt = N_RTacc;
		//    cout << SpillNb << " " << N_Procs << " / " << N_RTacc << " = " << N_Procs/N_RTacc << endl;
		//    cout << SpillNb << " " << SpillStartTime << " " << SpillStopTime << " " << hNRTAccepted->GetBinContent(SpillNb_Bin) << " " << N_RTacc << " " << N_RTatt << endl;
		//if(N_RTacc == 0.0 || N_RTatt==0) {
		if(N_RTatt==0) {
			NSpill_eff--;
			//cout << SpillNb << " " << N_RTatt << " " << NSpill_eff << endl;
			//cout << SpillNb << " " << SpillNb_Bin << " " << N_RTacc << " " << N_RTatt << " " << "Dummy spill " << iSpill << endl;
			continue;
		}

		if(N_RTatt < N_RTacc) {
			NSpill_eff--;
			//cout << SpillNb << " " << N_RTatt << " " << NSpill_eff << endl;
			//cout << SpillNb << " " << SpillNb_Bin << " " << N_RTacc << " " << N_RTatt << " " << "Dummy spill " << iSpill << endl;
			continue;
		}

		hBeamMeanTime_Spill = (TH1F*)hBeamMeanTimeVsSpillNum->ProjectionY(Form("BeamMeanTime_Spill%d",SpillNb),SpillNb_Bin, SpillNb_Bin);

		N_Trk_std = hBeamMeanTime_Spill->Integral(WindowMin_Bin,WindowMax_Bin,"");

		// TEMPORARY FIX!!!
		//N_Trk_std = hBeamMeanTime_Spill->GetEntries();
		//cout << "Reconstructed tracks: " << SpillNb << " " << N_Trk_std << " " << endl;

		if(N_Trk_std==0) {
			NSpill_eff--;
			//cout << SpillNb << " " << N_RTatt << " " << NSpill_eff << endl;
			//cout << SpillNb << " " << SpillNb_Bin << " " << N_RTacc << " " << N_RTatt << " " << "Dummy spill " << iSpill << endl;
			continue;
		}
		//if(k==1) cout << SpillNb << " " << SpillNb_Bin << " " << N_RTacc << " " << N_RTatt << " " << "Dummy spill " << iSpill << endl;

		// F_std = N_Trk_std/(N_RTacc*2*TimeHalfWindow*1.0e-9)*(SpillStopTime-SpillStartTime);
		// ErrF_std = F_std*sqrt(1.0/N_Trk_std+1.0/N_RTacc);

		// F_std = N_Trk_std/(N_RTatt*2*TimeHalfWindow*1.0e-9)*(SpillStopTime-SpillStartTime);
		F_std = N_Trk_std/(N_RTatt*2*TimeHalfWindow*1.0e-9);
		ErrF_std = F_std*sqrt(1.0/N_Trk_std+1.0/N_RTatt);

		Flux[k][iSpill] = F_std;
		eFlux[k][iSpill] = ErrF_std;

		if(N_RTacc>0) {

			F_nodt = N_Trk_std/(N_RTacc*2*TimeHalfWindow*1.0e-9);
			ErrF_nodt = F_nodt*sqrt(1.0/N_Trk_std+1.0/N_RTacc);
		}

		Flux_nodaqdt[k][iSpill] = F_nodt;
		eFlux_nodaqdt[k][iSpill] = ErrF_nodt;

		float num = BestN_VDT_num_2[iSpill] - BestN_VDT_num_1[iSpill];
		float den = BestN_VDT_den_2[iSpill] - BestN_VDT_den_1[iSpill];
		if(den > 0) {
			Flux_withvtdt[k][iSpill]  = Flux[k][iSpill] * num/den;
			eFlux_withvtdt[k][iSpill] = eFlux[k][iSpill] * num/den;
		}

		//F_std_tot += F_std;
		F_std_tot += Flux_withvtdt[k][iSpill];

		//       if(N_Trk_std!=0){
		// 	F_std = 100.0/(N_RTatt*2*TimeHalfWindow*1.0e-9);
		// 	ErrF_std = F_std*sqrt(1.0/100.0+1.0/N_RTatt);
		//       }else{
		// 	F_std = N_Trk_std/(N_RTatt*2*TimeHalfWindow*1.0e-9);
		// 	ErrF_std = F_std*sqrt(1.0/N_Trk_std+1.0/N_RTatt);
		//       }

		//       if(N_RTatt>1.0e5)
		// 	{
		// 	  N_Trk_std = 0.0;
		// 	  F_std = 0.0;
		// 	  ErrF_std = 1.0;
		// 	}


		if(TMath::IsNaN(ErrF_std))ErrF_std = 1.0;

		if(false && N_Procs/N_RTacc<1.2 && N_Trk_std>0.0) {
			cout << SpillNb << " " << N_Procs/N_RTacc << " " << N_Trk_std << endl;
			N_Trk_std = 0.0;
		}

		//if(fabs(N_RTatt_array[iSpill])>5.0e4) {
		if(fabs(N_RTatt_array[iSpill])>=5.0e7) {
			cout << SpillNb << " : problem in scalers... " << N_RTatt/5.0e4 << " " << N_RTacc << endl;
			F_std = F_std*5.0e4;
			ErrF_std = F_std*5.0e4;

			N_Trk_std = 0.0;
		}



		//cout << SpillNb << "    N_RTatt=" << N_RTatt << "    N_Trk_std=" << N_Trk_std << "    F_std="
		//	 << F_std << "    ErrF_std=" << ErrF_std << endl;
		//cout << SpillNb << "    N_RTacc=" << N_RTacc << "    N_Trk_std=" << N_Trk_std << "    F_nodt="
		//     << F_nodt << "    ErrF_nodt=" << ErrF_nodt << endl;

		//fout << SpillNb << "    " << TimeHalfWindow << "    " << TimeWindowOffset << "     " << N_RTacc << "      "
		fout << SpillNb << " " << beam_charge << " " << TimeHalfWindow << " " << TimeWindowOffset
				<< " " << Flux[k][iSpill] << " " << eFlux[k][iSpill]
															 << " " << Flux_withvtdt[k][iSpill] << " " << eFlux_withvtdt[k][iSpill]
																															<< " " << Flux_nodaqdt[k][iSpill] << " " << eFlux_nodaqdt[k][iSpill]
																																														 << " " << N_RTatt << " " << N_Trk_std
																																														 << endl;
	}

	//printf("total flux: %f    time window: +/-%0.1f\n",(float)F_std_tot, (float)TimeHalfWindow);
	cout<</*std::fixed<<std::setprecision(1)<<*/"total flux: "<<F_std_tot<<"    time window: +/-"<<TimeHalfWindow<<" ns"<<endl;

	hBeamMeanTime_Spill = (TH1F*)hBeamMeanTimeVsSpillNum->ProjectionY(Form("BeamMeanTime_Spill%d",SpillNb),1,150);

	hBeamMeanTime_Spill->SetLineColor(2);
	hBeamMeanTime_Spill->SetMaximum(3.0e4);
	hBeamMeanTime_Spill->SetTitle(Form("Run %d, Official production, 2FI3SI, target crossing; Beam mean time (ns)",RunNum));
	hBeamMeanTime_Spill->Draw();

	return NSpill_eff;
}



//void rt_flux(const Int_t RunNum, const Int_t Charge = 1,
//		const Float_t OffMin = 1.8, const Float_t OffMax = 2.2,
//		const char* Opt = "Official", const Int_t slot=3, Bool_t Save=kFALSE)

int main(int argc, char **argv)
{
	if(argc < 1)
	{
		cout << FRED("ERROR : Not enough arguments.") << endl;
		cout << FRED("Asked : 1 *** Received : ") << argc-1 << endl;
		cout << FRED("./plot_target [run1] [run2]") << endl;
		cout << FRED("Use ./plot_target -h for further help") << endl;

		return 1;
	}

	Int_t RunNum;
	const Int_t Charge = 1;
	const Float_t OffMin = 1.8;
	const Float_t OffMax = 2.2;
	const char* Opt = "Official";
	const Int_t slot=3;
	Bool_t Save=kFALSE;

	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "-h")
		{
			cout << FCYN("HELP : available flags :") << endl;
			cout << FCYN("-pm") << endl;
			cout << FCYN("./rt_flux  [RunNum] [Charge = 1] [OffMin = 1.8] [OffMax = 2.2] [Opt = \"Official\"] [slot=3] [Save=kFALSE]")  << endl;
			return 0;
		}

		if(argc < 1)
		{
			cout << FRED("ERROR : Not enough arguments.") << endl;
			cout << FRED("Asked : 1 *** Received : ") << argc-1 << endl;
			cout << FRED("./rt_flux  [RunNum] [Charge = 1] [OffMin = 1.8] [OffMax = 2.2] [Opt = \"Official\"] [slot=3] [Save=kFALSE]")  << endl;


			return 1;
		}

		if (string(argv[i]) == "-pm")
		{
			RunNum = atoi(argv[2]);

		}
		else
		{
			RunNum = atoi(argv[1]);
		}
	}






	gROOT->ProcessLine(".L HistLoader.cc+");
	HistLoader *histloader;

	/////////////////////////////////////////////////////////////////////////////////
	///  BEWARE!             													  ///
	///  If you change repository remember to switch it for rt_attempts too !!!!  ///
	/////////////////////////////////////////////////////////////////////////////////

	histloader = new HistLoader("../outputs/2016/phast/%d/hist-1000-%d.root",RunNum,RunNum);
	//histloader = new HistLoader("/afs/cern.ch/work/a/avidon/outputs/%d/hist-1000-%d.root",RunNum,RunNum);

	//  histloader = new HistLoader("/afs/cern.ch/work/a/avidon/outputs/P09t4_QcutTcutCameracalib5cm/%d/hist-1000-%d.root",RunNum,RunNum);
	//TFile *f0 = TFile::Open(Form("Slot%d/RootFiles/hist-%d-%s.root",slot,RunNum,Opt));

	hl = histloader;




	TH1F* hSpillWindow = histloader->GetTH1("hSpillWindow");
	if( !hSpillWindow ) {
		cout<<"hSpillWindow not found."<<endl;
		delete histloader;
		return 0;
	}
	spill_tmin = hSpillWindow->GetBinContent(2)/hSpillWindow->GetBinContent(1);
	spill_tmax = hSpillWindow->GetBinContent(3)/hSpillWindow->GetBinContent(1);

	TH1F* hBeamCharge = histloader->GetTH1("hBeamCharge");
	if( !hBeamCharge ) {
		cout<<"hBeamCharge not found."<<endl;
		delete histloader;
		return 0;
	}
	beam_charge = 0;
	if( hBeamCharge->GetBinContent(1) > hBeamCharge->GetBinContent(2) )
		beam_charge = -1;
	if( hBeamCharge->GetBinContent(1) < hBeamCharge->GetBinContent(2) )
		beam_charge = 1;

	Float_t TimeHalfWindow = 0.5;
	Float_t TimeWindowOffset = 1.5;
	// 2012
	//Float_t SpillStartTime = 1.0;
	//Float_t SpillStopTime = 10.4;
	// 2016
	Float_t SpillStartTime = spill_tmin;
	Float_t SpillStopTime = spill_tmax;

	cout << "Spill start time : " << SpillStartTime << " s;  Spill stop time : " << SpillStopTime << "s"
			<< "; Beam time half window : " << TimeHalfWindow << " ns; Time window offset : " << TimeWindowOffset << " ns."
			<< endl;

	CombineFiles(RunNum, 1000, 200);

	ofstream fout(Form("flux/rt_flux-%d.txt",RunNum));
	//ofstream fout(Form("Output_TOT_WO_%d_%s.txt",RunNum,Opt));

	Int_t Nspills_Rec;
	Int_t Nspills_Eff;

	TCanvas *C0 = new TCanvas("C0","C0",700,500);
	C0->cd();

	//Float_t THW = 1.0;
	for(Int_t k = 0; k<8; k++)
	{
		//THW[k] = (Float_t)k*0.25-1.25;
		Nspills_Rec = GetFluxesFromHistos(fout, RunNum, Opt, k, SpillStartTime, SpillStopTime, slot);
		fout << endl << endl;
	}
	cout << "Run number: " << RunNum << "; Spills recorded: " << Nspills_Rec << endl;

	PlotFluxHistos(RunNum);

	//Nspills_Eff = PlotFlux_Run_fSpillNb_AWW(RunNum, Charge, Opt, Nspills_Rec, 8, OffMin, OffMax, slot);

	cout << Nspills_Eff << " useful spills." << endl;

	if(Save)C0->SaveAs(Form("Flux_Run%d.pdf",RunNum));
	delete histloader;

	return 0;
}
