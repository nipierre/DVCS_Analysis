#include "include/TargetCoordinates.h"
#include "TMath.h"


/*
const float target_z_survey = -327.28;

const float target_survey_offset = 0.71;
const float target_z0 = target_z_survey - target_survey_offset;
const float target_cell_length = 254;
const float target_cell_half_length = target_cell_length/2;

const float target_cell_center =
  target_z0 + 4 + 3.2 + target_cell_half_length;

const float target_cell_zmin =
  target_cell_center - target_cell_half_length;
const float target_cell_zmax =
  target_cell_center + target_cell_half_length;

const float target_zmin = target_cell_zmin + 4;
const float target_zmax = target_cell_zmax - 4;

const float target_inlet_length = 9.5;
const float target_inlet_end = target_z0 + 4 + 3.2 + 1;
const float target_inlet_start = target_inlet_end - target_inlet_length;

const float target_zstart =
  target_inlet_start - 1.5;
const float target_zend =
  target_cell_center + target_cell_half_length + 1.5;



const float camera_survey_u = -328.99;
const float camera_survey_d = 77.9;
const float camera_survey_offset_u = -2;
const float camera_survey_offset_d = 25;

const float camera_ringb_center =
  (camera_survey_u-camera_survey_offset_u +
   camera_survey_d-camera_survey_offset_d)/2;

const float camera_ringb_zmin = camera_ringb_center - 360.0f/2.0f;

const float camera_ringb_center_expected =
  target_cell_zmin + 5 + 360.0f/2.0f;
 */

float target_z_survey;

float target_survey_offset;
float target_z0;
float target_cell_length;
float target_cell_half_length;

float target_cell_center;

float target_cell_zmin;
float target_cell_zmax;

float target_zmin;
float target_zmax;

float target_inlet_length;
float target_inlet_end;
float target_inlet_start;

float target_zstart;
float target_zend;

float target_cryostat_window_z;

float target_inlet_zmax ;
float target_inlet_zmin ;

float camera_survey_u;
float camera_survey_d;
float camera_survey_offset_u;
float camera_survey_offset_d;

float camera_ringb_center;
float camera_ringb_center2;

float camera_ringb_zmin;
float camera_ringb_zmin2;

float camera_ringb_center_expected;


static void Init2012() {
	float global_offset = 2.6;

	target_z_survey = -327.28 + global_offset;

	target_survey_offset = 0.71;
	target_z0 = target_z_survey - target_survey_offset;
	target_cell_length = 254;
	target_cell_half_length = target_cell_length/2;

	target_cell_center =
			target_z0 + 4 + 3.2 + target_cell_half_length;

	target_cell_zmin = target_z0 + 4 + 3.2;
	//target_cell_center - target_cell_half_length;
	target_cell_zmax = target_cell_zmin + target_cell_length;
	//target_cell_center + target_cell_half_length;

	target_zmin = target_cell_zmin + 4;
	target_zmax = target_cell_zmax - 4;

	target_inlet_length = 9.5;
	target_inlet_zmax = target_z0 + 4 + 3.2 + 1;
	target_inlet_zmin = target_inlet_zmax - target_inlet_length;

	float target_lh2_zmin = target_inlet_zmin - 1.5;
	float target_lh2_zmax = target_cell_zmax + 1.5;
	target_cryostat_window_z =
			target_cell_zmin + 261 + 3.5;

	target_zstart =
			target_inlet_zmin - 1.5;
	target_zend =
			target_cell_center + target_cell_half_length + 1.5;



	camera_survey_u = -328.99 + global_offset;
	camera_survey_d = 77.9 + global_offset;
	camera_survey_offset_u = -2;
	camera_survey_offset_d = 25;


	camera_ringb_zmin = camera_survey_u-camera_survey_offset_u+10.45;
	camera_ringb_center = camera_ringb_zmin + 360.0f/2.0f;

	camera_ringb_center2 =
			(camera_survey_u-camera_survey_offset_u +
					camera_survey_d-camera_survey_offset_d)/2;
	camera_ringb_zmin2 = camera_ringb_center2 - 360.0f/2.0f;

	camera_ringb_center_expected =
			target_cell_zmin + 5 + 360.0f/2.0f;
}


static void Init2016() {
	float global_offset = 0.0;

	target_z_survey = -327.28 + global_offset;

	target_survey_offset = 0.71;
	target_z0 = target_z_survey - target_survey_offset;
	target_cell_length = 254;
	target_cell_half_length = target_cell_length/2;

	target_cell_center =
			target_z0 + 4 + 3.2 + target_cell_half_length;

	target_cell_zmin = target_z0 + 4 + 3.2;
	//target_cell_center - target_cell_half_length;
	target_cell_zmax = target_cell_zmin + target_cell_length;
	//target_cell_center + target_cell_half_length;

	target_zmin = target_cell_zmin + 4;
	target_zmax = target_cell_zmax - 4;

	target_inlet_length = 9.5;
	target_inlet_zmax = target_z0 + 4 + 3.2 + 1;
	target_inlet_zmin = target_inlet_zmax - target_inlet_length;

	float target_lh2_zmin = target_inlet_zmin - 1.5;
	float target_lh2_zmax = target_cell_zmax + 1.5;
	target_cryostat_window_z =
			target_cell_zmin + 261 + 3.5;

	target_zstart =
			target_inlet_zmin - 1.5;
	target_zend =
			target_cell_center + target_cell_half_length + 1.5;



	camera_survey_u = -333.45 + global_offset;
	camera_survey_d = 56.30 + global_offset;
	camera_survey_offset_u = -2;
	camera_survey_offset_d = 7;


	camera_ringb_zmin = camera_survey_u-camera_survey_offset_u+10.45;
	camera_ringb_center = camera_ringb_zmin + 360.0f/2.0f;

	camera_ringb_center2 =
			(camera_survey_u-camera_survey_offset_u +
					camera_survey_d-camera_survey_offset_d)/2;
	camera_ringb_zmin2 = camera_ringb_center2 - 360.0f/2.0f;

	camera_ringb_center_expected =
			target_cell_zmin + 5 + 360.0f/2.0f;
}


void InitTargetCoordinates(int year)
{
	switch(year) {
	case 2012: Init2012(); break;
  case 2016: Init2016(); break;
  case 2017: Init2016(); break;
	default: break;
	}
}


TVector3 GetDvcsTargetCenter()
{
	return TVector3(0,0,target_cell_center);
}

bool InDvcsTarget(TVector3 vtx)
{
	float z = vtx.Z();
	float x = vtx.X();
	float y = vtx.Y();
	float r = TMath::Sqrt(x*x+y*y);
	if(z < (target_cell_zmin+4)) return false;
	if(z > (target_cell_zmax-4)) return false;
	if(r > 1.6) return false;
	return true;
}
