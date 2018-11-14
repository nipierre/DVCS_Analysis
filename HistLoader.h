#ifndef HIST_LOADER__H
#define HIST_LOADER__H

#include <string>
#include <vector>

#include <TFile.h>
#include <TList.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

using namespace std;

class HistLoader
{
  vector<string> fnamevec[3];
  vector<TFile*> filevec[3];
  vector<int>    runvec[3];
  TFile* fdummy;
  char dummyname[500];

public:
  HistLoader(const char* base, int runmin, int runmax);
  ~HistLoader() {
    if( fdummy ) {
      delete fdummy;
      unlink(dummyname);
      cout<<dummyname<<" deleted"<<endl;
    }
  }

  void CloseAll();

  TH1F* GetTH1(char* name, int charge, int runmin, int runmax);
  TH2F* GetTH2(char* name, int charge, int runmin, int runmax);
  TH3F* GetTH3(char* name, int charge, int runmin, int runmax);

  TH1F* GetTH1(char* name) {return GetTH1(name,0,0,10000000);}
  TH2F* GetTH2(char* name) {return GetTH2(name,0,0,10000000);}
  TH3F* GetTH3(char* name) {return GetTH3(name,0,0,10000000);}

  TH1F* GetTH1(char* name, int charge) {return GetTH1(name,charge,0,10000000);}
  TH2F* GetTH2(char* name, int charge) {return GetTH2(name,charge,0,10000000);}
  TH3F* GetTH3(char* name, int charge) {return GetTH3(name,charge,0,10000000);}

  TH1F* GetTH1(char* name, int runmin, int runmax) {return GetTH1(name,0,runmin,runmax);}
  TH2F* GetTH2(char* name, int runmin, int runmax) {return GetTH2(name,0,runmin,runmax);}
  TH3F* GetTH3(char* name, int runmin, int runmax) {return GetTH3(name,0,runmin,runmax);}

  void DrawTH1(char* name);
  void DrawTH2(char* name);

  TChain* GetChain(char* name, int charge, int runmin, int runmax);
  TChain* GetChain(char* name) {return GetChain(name,0,0,10000000);}
  TChain* GetChain(char* name, int charge) {return GetChain(name,charge,0,10000000);}
  TChain* GetChain(char* name, int runmin, int runmax) {return GetChain(name,0,runmin,runmax);}

  TTree* GetTree(char* name, int charge, int runmin, int runmax);
  TTree* GetTree(char* name) {return GetTree(name,0,0,10000000);}
  TTree* GetTree(char* name, int charge) {return GetTree(name,charge,0,10000000);}
  TTree* GetTree(char* name, int runmin, int runmax) {return GetTree(name,0,runmin,runmax);}

  TList* GetTreeList(char* name, int charge, int runmin, int runmax);
  TList* GetTreeList(char* name) {return GetTreeList(name,0,0,10000000);}
  TList* GetTreeList(char* name, int charge) {return GetTreeList(name,charge,0,10000000);}
  TList* GetTreeList(char* name, int runmin, int runmax) {return GetTreeList(name,0,runmin,runmax);}

  vector<TTree*> GetTreeVector(char* name, int charge, int runmin, int runmax);
  vector<TTree*> GetTreeVector(char* name) {return GetTreeVector(name,0,0,10000000);}
  vector<TTree*> GetTreeVector(char* name, int charge) {return GetTreeVector(name,charge,0,10000000);}
  vector<TTree*> GetTreeVector(char* name, int runmin, int runmax) {return GetTreeVector(name,0,runmin,runmax);}
};

#endif
