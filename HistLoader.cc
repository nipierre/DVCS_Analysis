#include <iostream>
#include <string>

#include "include/HistLoader.h"
#include "TLorentzVector.h"

using namespace std;

HistLoader::HistLoader(const char* base, int runmin, int runmax)
{
//	cout<<"HistLoader started"<<endl;
  char fname[500];
  int fd = open("/dev/urandom", O_RDONLY, 0);
  int key = 0;
  if (fd >= 0) {
    read(fd, &key, sizeof(int));
    (void)close(fd);
  }
  snprintf(dummyname, 499, "/tmp/___temp_%d.root",key);
  fdummy = new TFile(dummyname,"RECREATE");
//	cout<<"dummy file created"<<endl;
 for (int run = runmin; run <= runmax; run++) {
    bool first = true;
    for(int i = 1; i <= 500; i++) {
      sprintf(fname,base,run,i);
      FILE* testf = fopen(fname,"r");
      //cout<<"Testing "<<fname<<endl;
      if(!testf) continue;
      fclose(testf);
      //cout<<"Loading file "<<fname<<endl;
      TFile* f = new TFile(fname);
      //cout<<"f: "<<f<<endl;
      if(!f) continue;
      if(!f->IsOpen()) {delete f; continue;}
      //cout<<"Opened"<<endl;
      TH1F* hcharge = (TH1F*)f->Get("hBeamCharge");
      int nminus = 0;
      int nplus = 0;
      if(hcharge) {
        nminus = hcharge->GetBinContent(1);
        nplus = hcharge->GetBinContent(2);
        //cout<<"nminus: "<<nminus<<"    nplus: "<<nplus<<endl;
        delete hcharge;
      }
      //cout<<"closing "<<f->GetName()<<endl;
      f->Close("R");
      delete f; f=NULL;
      if(nminus>0 && nplus>0) {delete f; continue;}
      int fid;
      if(nminus>0) fid = 0;
      if(nplus>0) fid = 1;
      if(nminus<1 && nplus<1) fid = 2;
      if(first) {
        cout<<run<<" ";
        if(fid==0) cout<<"-";
        if(fid==1) cout<<"+";
        if(fid==2) cout<<"?";
        cout<<endl;
        first = false;
      }
      fnamevec[fid].push_back(std::string(fname));
      filevec[fid].push_back(f);
      runvec[fid].push_back(run);
    }
  }
  cout<<"Total number of opened files: "
      <<filevec[0].size()+filevec[1].size()+filevec[2].size()<<endl;
}


void HistLoader::CloseAll()
{
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < int(filevec[i].size()); j++) {
      if(!filevec[i][j]) continue;
      filevec[i][j]->Close("R");
      delete filevec[i][j]; filevec[i][j]=NULL;
      //cout<<fnamevec[i][j]<<" closed."<<endl;
    }
  }
  cout<<"HistLoader: file cache cleared."<<endl;
}


TH1F* HistLoader::GetTH1(char* name, int charge, int runmin, int runmax)
{
  TH1F* h = NULL;
  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    //cout<<"runvec["<<ci<<"].size(): "<<runvec[ci].size()<<endl;
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        //cout<<"Opening "<<fnamevec[ci][fi]<<"..."<<endl;
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        //f->Open(fnamevec[ci][fi].c_str());
        //f->Init(kFALSE);
        //cout<<"Calling f->Init(kFALSE)..."<<endl;
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TH1F* h2 = (TH1F*)f->Get(name);
      //cout<<"h: "<<h<<"    h2: "<<h2<<endl;
      if(!h2) {
        filevec[ci][fi]->Close("R");
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      if(!h) {
        sprintf(tstr,"%s_clone",name);
        fdummy->cd();
        h = (TH1F*)h2->Clone(tstr);
        //h = new TH1F(name,h2->GetTitle(),
        //	     h2->GetXaxis()->GetNbins(),
        //	     h2->GetXaxis()->GetXmin(),
        //	     h2->GetXaxis()->GetXmax());
        //h->Add(h2);
      } else {
        //cout<<"Before adding to "<<h->GetName()<<"..."<<endl;
        h->Add(h2);
        //cout<<"Histogram "<<name<<" added."<<endl;
      }
      //if( h2->GetBinContent(4)/h2->GetBinContent(1)< 0.99 )
      //cout << fi << ": "<< h2->GetBinContent(4) << "/" << h2->GetBinContent(1)
      //     << "=" << h2->GetBinContent(4)/h2->GetBinContent(1) << endl;
      delete h2;
      //cout<<"closing "<<filevec[ci][fi]->GetName()<<endl;
      filevec[ci][fi]->Close("R");
      //cout<<"h2 deleted."<<endl;
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
    }
  }
  return h;
}



TH2F* HistLoader::GetTH2(char* name, int charge, int runmin, int runmax)
{
  TH2F* h = NULL;
  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }

    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      //cout<<"nopen: "<<nopen<<endl;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TH2F* h2 = (TH2F*)f->Get(name);
      if(!h2) {
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      if(!h) {
        sprintf(tstr,"%s_clone",name);
        fdummy->cd();
        h = (TH2F*)h2->Clone(tstr);
      } else {
        h->Add(h2);
      }
      delete h2;
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
    }
  }
  return h;
}

TH3F* HistLoader::GetTH3(char* name, int charge, int runmin, int runmax)
{
  TH3F* h = NULL;
  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 2; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      //cout<<"nopen: "<<nopen<<endl;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TH3F* h2 = (TH3F*)f->Get(name);
      if(!h2) {
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      if(!h) {
        sprintf(tstr,"%s_clone",name);
        fdummy->cd();
        h = (TH3F*)h2->Clone(tstr);
      } else {
        h->Add(h2);
      }
      delete h2;
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
    }
  }
  return h;
}


void HistLoader::DrawTH1(char* name)
{
  TH1F* h = GetTH1(name);
  h->Draw();
}


void HistLoader::DrawTH2(char* name)
{
  TH2F* h = GetTH2(name);
  h->Draw("col");
}

TChain* HistLoader::GetChain(char* name, int charge, int runmin, int runmax)
{
  // char tstr[500];

  if(fdummy) fdummy->cd();
  TChain* Chain = new TChain(name,"MyChain");

  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    //cout<<"runvec["<<ci<<"].size(): "<<runvec[ci].size()<<endl;
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      //if( fi > 1 ) continue;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      //cout << run << endl;
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}

        if(!f->IsOpen()) continue;
      }

      nopen++;

      TTree* T2 = (TTree*)f->Get(name);
      if(!T2) {
        cout<<"HistLoader::GetChain(): T2=NULL, file="<<fnamevec[ci][fi]<<endl;
        f->Close();
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      //if(T2->GetEntries()<=0)continue;
      Long64_t Nentries = T2->GetEntries();
      if(Nentries<=0) {
        cout<<"HistLoader::GetChain(): Nentries<=0, file="<<fnamevec[ci][fi]<<endl;
        f->Close();
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }

      //sprintf(tstr,"Chain_%d_%d",run,fi);
      //TChain* Chain2 = new TChain(tstr,tstr);

      delete T2;
      f->Close();
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
      //fdummy->cd();

      cout << "adding file "<<fnamevec[ci][fi].c_str() << " to TChain, entries = " << Nentries << endl;

      //Chain2->AddFile(fnamevec[ci][fi].c_str(),-1,name);
      //Chain->AddFile(fnamevec[ci][fi].c_str(),-1,name);
      Chain->Add(fnamevec[ci][fi].c_str());

      //sprintf(tstr,"%s/%s",fnamevec[ci][fi].c_str(),name);
      //Chain->Add(tstr,-1);

      //Chain->AddFriend(T2,tstr);

      //Chain->Add(Chain2);

      //       TLorentzVector* p4beam = new TLorentzVector();
      //       TBranch* bbeam = Chain2->GetBranch("par_4_0");
      //       bbeam->SetAddress(&p4beam);bbeam->SetStatus(1);

      //       for(int i = 0; i<Chain2->GetEntries(); i++)
      //  	{
      //  	  Chain2->GetEntry(i);
      //  	  cout << i << " " << p4beam->E() << endl;
      //  	}

     //delete Chain2;
    }
  }
  //sprintf(tstr,"%s_all",name);
  //Chain->Merge("NewChain.root");

  //   TLorentzVector* p4beam = new TLorentzVector();
  //   TBranch* bbeam = Chain->GetBranch("par_4_0");
  //   bbeam->SetAddress(&p4beam);bbeam->SetStatus(1);
  //   for(int i = 0; i<Chain->GetEntries(); i++)
  //     {
  //       Chain->GetEntry(i);
  //       cout << i << " " << p4beam->E() << endl;
  //     }

  return Chain;
}

TTree* HistLoader::GetTree(char* name, int charge, int runmin, int runmax)
{
  TTree* T = NULL;
  //TList* list = new TList();
  //TCollection* list = new TCollection();

  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    //cout<<"runvec["<<ci<<"].size(): "<<runvec[ci].size()<<endl;
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      //cout << run << endl;
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        //f->Open(fnamevec[ci][fi].c_str());
        //f->Init(kFALSE);
        //cout<<"Calling f->Init(kFALSE)..."<<endl;
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TTree* T2 = (TTree*)f->Get(name);
      //cout<<"h: "<<h<<"    h2: "<<h2<<endl;
      if(!T2) {
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      cout << run << " " << T << " " << T2 << " "  << T2->GetEntries() << endl;
      cout << fnamevec[ci][fi] << endl;

      if(!T) {
        sprintf(tstr,"%s_all",name);
        fdummy->cd();
        T = (TTree*)T2->Clone(tstr);

        //list->Add(T);
      } else {
        sprintf(tstr,"%s_%d_%d",name,run,fi);
        //cout << tstr << endl;
        fdummy->cd();
        TTree *T1 = (TTree*)T2->Clone(tstr);

        T->AddFriend(T1);
        //list->Add(T1);

        delete T2;
        //cout<<"T2 deleted."<<endl;
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        //cout << "file deleted" << endl;
      }
    }
  }

  //TTree* T3 = (TTree*)list->At(0);
  //cout << T3 << " " << T3->GetName() << " " << T3->GetEntries() << endl;

  //   int i_ls = 0;
  //   while(1){
  //     TTree* T_ = (TTree*)list->At(i_ls);
  //     cout << i_ls << " " << T_ << " " << T_->GetName() << " " << T_->GetEntries() << endl;
  //     if(T_==list->Last())break;
  //     i_ls++;
  //   }

  //cout << "Merging trees " << endl;
  //fdummy->cd();
  //TTree* T3 = T->MergeTrees(list);
  //Long64_t NEntries = T->Merge(list);
  //cout << "Trees merged" << endl;
  cout << T << " " << T->GetName() << " " << T->GetEntries() << endl;
  fdummy->Write();
  //cout << T3 << " " << T3->GetName() << " " << T3->GetEntries() << endl;

  //cout << "Trees merged, total " << NEntries << " entries" << endl;
  //T3->Draw("");

  //return T3;
  return T;
}


TList* HistLoader::GetTreeList(char* name, int charge, int runmin, int runmax)
{
  //TTree* T = NULL;
  //TCollection;
  TList* list = new TList();

  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    //cout<<"runvec["<<ci<<"].size(): "<<runvec[ci].size()<<endl;
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      //cout << run << endl;
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        //f->Open(fnamevec[ci][fi].c_str());
        //f->Init(kFALSE);
        //cout<<"Calling f->Init(kFALSE)..."<<endl;
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TTree* T2 = (TTree*)f->Get(name);
      //cout<<"h: "<<h<<"    h2: "<<h2<<endl;
      if(!T2) {
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      //cout << run << " " << T2 << " "  << T2->GetEntries() << endl;

      //       if(!T) {
      // 	sprintf(tstr,"%s_all",name);
      //   	fdummy->cd();
      //  	T = (TTree*)T2->Clone(tstr);

      // 	cout << T << " " << T->GetName() << " " << T->GetEntries() << endl;

      // 	list->Add(T);
      //       } else {
      // 	sprintf(tstr,"%s_clone",name);
      // 	TTree *T1 = (TTree*)T->Clone(tstr);
      // 	list->Add(T1);
      // 	list->Add(T2);
      // 	//cout<<"Before adding to "<<h->GetName()<<"..."<<endl;
      // //  	sprintf(tstr,"%s_clone",name);
      // // 	fdummy->cd();
      // 	T->MergeTrees(list);

      // 	//T = T_;
      // // 	T = (TTree*)T2->Clone(tstr);
      // 	//T->Add(h2);
      // 	//cout << run << "  " << T->GetEntries() << endl;

      sprintf(tstr,"%s_%d_%d",name,run,fi);
      //cout << tstr << endl;
      fdummy->cd();
      TTree *T = (TTree*)T2->Clone(tstr);

      list->Add(T);

      delete T2;
      //cout<<"T2 deleted."<<endl;
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
      //cout << "file deleted" << endl;
      //}

    }
  }

  //TTree* T3 = (TTree*)list->At(0);
  //cout << T3 << " " << T3->GetName() << " " << T3->GetEntries() << endl;

  //cout << "Merging trees " << endl;
  //fdummy->cd();
  //TTree* T3 = T->MergeTrees(list);
  //cout << "Trees merged" << endl;
  //cout << T3 << " " << T3->GetName() << " " << T3->GetEntries() << endl;

  //T3->Draw("");

  //return T3;
  return list;
}

std::vector<TTree*> HistLoader::GetTreeVector(char* name, int charge, int runmin, int runmax)
{
  std::vector<TTree*> list;

  char tstr[500];
  int nopen = 0;
  for(int ci = 0; ci < 3; ci++) {
    if(charge != 0) {
      if(charge < 0 && ci != 0) continue;
      if(charge > 0 && ci != 1) continue;
    }
    //cout<<"runvec["<<ci<<"].size(): "<<runvec[ci].size()<<endl;
    for(int fi = 0; fi < int(runvec[ci].size()); fi++) {
      int run = runvec[ci][fi];
      if(run<runmin || run>runmax) continue;
      if(nopen == 500) {
        //CloseAll();
        nopen = 0;
      }
      //cout << run << endl;
      TFile* f = filevec[ci][fi];
      if(!f || !f->IsOpen()) {
        if(!f) {f = new TFile(fnamevec[ci][fi].c_str()); filevec[ci][fi]=f;}
        //f->Open(fnamevec[ci][fi].c_str());
        //f->Init(kFALSE);
        //cout<<"Calling f->Init(kFALSE)..."<<endl;
        if(!f->IsOpen()) continue;
        //cout<<fnamevec[ci][fi]<<" opened, f: "<<f<<endl;
      }

      nopen++;
      TTree* T2 = (TTree*)f->Get(name);
      //cout<<"h: "<<h<<"    h2: "<<h2<<endl;
      if(!T2) {
        delete filevec[ci][fi]; filevec[ci][fi]=NULL;
        continue;
      }
      //cout << run << " " << T2 << " "  << T2->GetEntries() << endl;

      //       if(!T) {
      // 	sprintf(tstr,"%s_all",name);
      //   	fdummy->cd();
      //  	T = (TTree*)T2->Clone(tstr);

      // 	cout << T << " " << T->GetName() << " " << T->GetEntries() << endl;

      // 	list->Add(T);
      //       } else {
      // 	sprintf(tstr,"%s_clone",name);
      // 	TTree *T1 = (TTree*)T->Clone(tstr);
      // 	list->Add(T1);
      // 	list->Add(T2);
      // 	//cout<<"Before adding to "<<h->GetName()<<"..."<<endl;
      // //  	sprintf(tstr,"%s_clone",name);
      // // 	fdummy->cd();
      // 	T->MergeTrees(list);

      // 	//T = T_;
      // // 	T = (TTree*)T2->Clone(tstr);
      // 	//T->Add(h2);
      // 	//cout << run << "  " << T->GetEntries() << endl;

      sprintf(tstr,"%s_%d_%d",name,run,fi);
      //cout << tstr << endl;
      fdummy->cd();
      // TTree* T = (TTree*)T2->Clone(tstr);

      //list.push_back(T);

      delete T2;
      //cout<<"T2 deleted."<<endl;
      delete filevec[ci][fi]; filevec[ci][fi]=NULL;
      //cout << "file deleted" << endl;
      //}

    }
  }

  //return T3;
  return list;
}
