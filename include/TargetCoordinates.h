#ifndef TARGET_COORDINATES__H
#define TARGET_COORDINATES__H

#include <TVector3.h>



extern float target_z_survey;

extern float target_survey_offset;
extern float target_z0;
extern float target_cell_length;
extern float target_cell_half_length;

extern float target_cell_center;

extern float target_cell_zmin;
extern float target_cell_zmax;

extern float target_zmin;
extern float target_zmax;

extern float target_inlet_length;
extern float target_inlet_end;
extern float target_inlet_start;

extern float target_zstart;
extern float target_zend;



extern float camera_survey_u;
extern float camera_survey_d;
extern float camera_survey_offset_u;
extern float camera_survey_offset_d;

extern float camera_ringb_center;
extern float camera_ringb_center2;

extern float camera_ringb_zmin;
extern float camera_ringb_zmin2;
extern float target_cryostat_window_z;
extern float camera_ringb_center_expected;

extern float target_inlet_zmax;
extern float target_inlet_zmin;


void InitTargetCoordinates(int year);

TVector3 GetDvcsTargetCenter();

bool InDvcsTarget(TVector3 vtx);

#endif
