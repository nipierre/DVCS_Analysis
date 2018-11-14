#ifndef __GETFLUX_H_INCLUDED__
#define __GETFLUX_H_INCLUDED__

#include <TMath.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string>

void fill_badspills();
int get_flux(int run_min, int run_max, int charge, float half_win,
    Double_t& flux_tot, Double_t& eflux_tot);

static int bsl_select(const struct dirent *entry)
{
  std::string str = entry->d_name;
  std::size_t pos = str.find(".list");
  if( pos != std::string::npos)
    return 1;
  return 0;
};

#endif
