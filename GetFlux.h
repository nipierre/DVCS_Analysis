#ifndef __GETFLUX_H_INCLUDED__
#define __GETFLUX_H_INCLUDED__

#include <TMath.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string>

static int bsl_select(const struct dirent *entry);
void fill_badspills();
int get_flux(int run_min, int run_max, int charge, float half_win,
    Double_t& flux_tot, Double_t& eflux_tot);

#endif
