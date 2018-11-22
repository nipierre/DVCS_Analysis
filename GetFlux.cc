#include "include/GetFlux.h"

using namespace std;

//char* flux_db = ".";
//static char* flux_db = "/afs/cern.ch/compass/dvcs/Production/Analysis/FluxDB-20170216";
#define flux_db "/afs/cern.ch/compass/dvcs/Production/Analysis/Students/avidon/rootscripts/flux" //pour lire le flux en local
//static char* flux_db = "/afs/cern.ch/compass/dvcs/Production/Analysis/Students/avidon/rootscripts/fluxOld";
//static char* flux_db = "/afs/cern.ch/compass/dvcs/Production/Analysis/Students/phast-analysis-test/rootscripts";
//static char* flux_db = "/afs/cern.ch/work/a/avidon/outputs/P09t4_QcutTcutCameracalib5cm/flux";
#define cam_db  "/afs/cern.ch/compass/dvcs/Camera/Analysis/calibrations"

static std::set<unsigned int> badspills;

void fill_badspills()
{
  struct dirent **namelist;
  int n;
  //bool found = false;
  n = scandir("/afs/cern.ch/compass/dvcs/Production/Analysis/BadSpills/2012/badspilllists2012", &namelist, bsl_select, alphasort);
  for(int i = 0; i < n; i++) {
    char tstr[500];
    sprintf(tstr,"/afs/cern.ch/compass/dvcs/Production/Analysis/BadSpills/2012/badspilllists2012/%s",namelist[i]->d_name);
    std::ifstream bs;
    bs.open(tstr);
    if( !bs.eof() ) std::cout<<"Loading BSL "<<namelist[i]->d_name<<std::endl;
    while(bs.is_open() && !bs.eof()) {
      unsigned int run, spill;
      bs >> run >> spill;
      //std::cout<<"  added "<<run<<","<<spill<<std::endl;
      unsigned int uspill = run*1000+spill;
      badspills.insert(uspill);
    }
  }
}


int get_flux(int run_min, int run_max, int charge, float half_win,
    Double_t& flux_tot, Double_t& eflux_tot)
{
  flux_tot = 0;
  eflux_tot = 0;
  for(int run = run_min; run <= run_max; run++) {
    char tstr[500];

//    sprintf(tstr, "/afs/cern.ch/work/a/avidon/outputs/P09t4_QcutTcutCameracalib5cm/flux/rt_flux-%d.txt", run);
    sprintf(tstr, "%s/rt_flux-%d.txt", flux_db, run);
    //sprintf(tstr, "Output_EFF_%d.txt", run);
    FILE* fp = fopen(tstr,"r");
    if( !fp ) continue;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while( (read = getline(&line, &len, fp)) != -1 ) {
      int spill, c = 0;
      float thw, two, flux, eflux, flux_vdt, eflux_vdt;
      //int n = sscanf(line, "%d %f %f %f %f %f %f",
      //       &spill, &thw, &two,
      //       &flux, &eflux, &flux_vdt, &eflux_vdt);
      //if( n != 7 ) continue;
      int n = sscanf(line, "%d %d %f %f %f %f %f %f",
             &spill, &c, &thw, &two,
             &flux, &eflux, &flux_vdt, &eflux_vdt);
      if( n != 8 ) continue;
      //printf("line: %s", line);

      //printf("spill: %d, charge: %d, thw: %f\n", spill, c, thw);
      if( charge != 0 && charge != c ) continue;
      if( thw != half_win ) continue;
      unsigned int uspill = run*1000+spill;
      std::set<unsigned int>::iterator bsi = badspills.find( uspill );
      if( bsi != badspills.end() ) {
        //cout<<"get_flux(): "<<run<<" spill "<<spill<<" excluded"<<endl;
        continue;
      }
 //      printf("run %d  spill %d  flux = %0.1f +/- %0.1f  charge = %i \n",
//      	     run, spill, flux_vdt, eflux_vdt, charge);
      flux_tot += flux_vdt;
      eflux_tot += eflux_vdt * eflux_vdt;
      //flux_tot += flux;
      //eflux_tot += eflux * eflux;
    }

//    cout<<"run "<<run<<"("<<charge<<")  flux tot. = "<<flux_tot<<" +/- "<<eflux_tot<<endl;

    if (line) free(line);
    fclose(fp);
  }
  eflux_tot = TMath::Sqrt( eflux_tot );
  printf("run %d-%d(%d)  flux tot. = %0.1f +/- %0.1f\n",
   run_min, run_max, charge, flux_tot, eflux_tot);
  cout<<"run "<<run_min<<"-"<<run_max<<"("<<charge<<")  flux tot. = "<<flux_tot<<" +/- "<<eflux_tot<<endl;
  return 0;
}


int get_flux_camera(int run_min, int run_max, int charge, float half_win,
    Double_t* flux_tot, Double_t* eflux_tot, Double_t* flux_AB_tot, Double_t* eflux_AB_tot)
{
  for(int i = 0; i < 24; i++) {
    flux_tot[i] = 0;
    eflux_tot[i] = 0;
  }
  for(int i = 0; i < 48; i++) {
    flux_AB_tot[i] = 0;
    eflux_AB_tot[i] = 0;
  }
  bool is_bad_run[2][24];
  for(int run = run_min; run <= run_max; run++) {
    char tstr[500];
    for(int i = 0; i < 24; i++) {
      for(int j = 0; j < 2; j++) {
        is_bad_run[j][i] = false;
      }
    }
    sprintf(tstr,"%s/A_quality-%d.dat",cam_db,run);
    std::ifstream fAqual;
    fAqual.open(tstr);
    int n=0, Q;
    while(fAqual.is_open() && !fAqual.eof()) {
      fAqual>>Q;
      is_bad_run[0][n] = (Q == 1);
      n++;
    }
    sprintf(tstr,"%s/B_quality-%d.dat",cam_db,run);
    std::ifstream fBqual;
    fBqual.open(tstr);
    n=0;
    while(fBqual.is_open() && !fBqual.eof()) {
      fBqual>>Q;
      is_bad_run[1][n] = (Q == 1);
      n++;
    }

    sprintf(tstr, "%s/rt_flux-%d.txt", flux_db, run);
    //sprintf(tstr, "Output_EFF_%d.txt", run);
    FILE* fp = fopen(tstr,"r");
    if( !fp ) continue;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while( (read = getline(&line, &len, fp)) != -1 ) {
      int spill, c = 0;
      float thw, two, flux, eflux, flux_vdt, eflux_vdt;
      //int n = sscanf(line, "%d %f %f %f %f %f %f",
      //       &spill, &thw, &two,
      //       &flux, &eflux, &flux_vdt, &eflux_vdt);
      //if( n != 7 ) continue;
      n = sscanf(line, "%d %d %f %f %f %f %f %f",
          &spill, &c, &thw, &two,
          &flux, &eflux, &flux_vdt, &eflux_vdt);
      if( n != 8 ) continue;
      //printf("line: %s", line);

      //printf("spill: %d, charge: %d, thw: %f\n", spill, c, thw);
      if( charge != 0 && charge != c ) continue;
      if( thw != half_win ) continue;
      unsigned int uspill = run*1000+spill;
      std::set<unsigned int>::iterator bsi = badspills.find( uspill );
      if( bsi != badspills.end() ) {
        //cout<<"get_flux(): "<<run<<" spill "<<spill<<" excluded"<<endl;
        continue;
      }
      //printf("run %d  spill %d  flux = %0.1f +/- %0.1f\n",
      //       run, spill, flux_vdt, eflux_vdt);
      for( int bid = 0; bid < 24; bid++ ) {
        if( !is_bad_run[1][bid] ) {
          //cout<<"run: "<<run<<"  aid2: "<<aid2<<"  flux_tot: "<<(void*)(&flux_tot[0])<<endl;
          flux_tot[bid] += flux_vdt;
          eflux_tot[bid] += eflux_vdt * eflux_vdt;
        }
      }
      for( int aid = 0; aid < 24; aid++ ) {
        int bid1 = aid;
        int bid2 = aid+1;
        if(bid2 > 23) bid2 = 0;
        int aid2;
        aid2 = aid*2;
        if( !is_bad_run[0][aid] && !is_bad_run[1][bid1] ) {
          //cout<<"run: "<<run<<"  aid2: "<<aid2<<"  flux_tot: "<<(void*)(&flux_tot[0])<<endl;
          flux_AB_tot[aid2] += flux_vdt;
          eflux_AB_tot[aid2] += eflux_vdt * eflux_vdt;
        }
        //if(aid == 9 || aid == 10) cout<<"is_bad_run[0]["<<aid<<"]: "<<is_bad_run[0][aid]<<"    "
        aid2 = aid*2 + 1;
        if( !is_bad_run[0][aid] && !is_bad_run[1][bid2] ) {
          flux_AB_tot[aid2] += flux_vdt;
          eflux_AB_tot[aid2] += eflux_vdt * eflux_vdt;
        }
      }
      //flux_tot += flux;
      //eflux_tot += eflux * eflux;
    }
    if (line) free(line);
    fclose(fp);
  }
  for( int aid = 0; aid < 24; aid++ ) {
    eflux_tot[aid] = TMath::Sqrt( eflux_tot[aid] );
  }
  for( int aid = 0; aid < 48; aid++ ) {
    eflux_AB_tot[aid] = TMath::Sqrt( eflux_AB_tot[aid] );
  }
  //printf("run %d-%d(%d)  flux tot. = %0.1f +/- %0.1f\n",
  // run_min, run_max, charge, flux_tot, eflux_tot);
  //cout<<"run "<<run_min<<"-"<<run_max<<"("<<charge<<")  flux tot. = "<<flux_tot<<" +/- "<<eflux_tot<<endl;
  return 0;
}
