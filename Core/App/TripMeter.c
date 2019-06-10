#include<math.h>
#include"TripMeter.h"

//
// TM_Init
// 
void TM_Init(double drpm_Ts, int irpm_ref, int ispeedKPH_ref, struct TM_struct *ptm)
{
	(*ptm).irpm_ref = irpm_ref ;
	(*ptm).ispeedKPH_ref = ispeedKPH_ref ;
	(*ptm).drpm_Ts = drpm_Ts ;
	(*ptm).ddistpermin = ispeedKPH_ref * 1000 / 60 ;
	(*ptm).ddistperrev = (*ptm).ddistpermin / irpm_ref ;
}

//
// TM_ComputeDistance
// 
//    returns the elapsed distance in meter
//
double TM_ComputeDistance(int irpm, double drpm_Ts, struct TM_struct tm)
{
	double fdist = (((1.0 * irpm * drpm_Ts) / 60) * tm.ddistperrev) / 1000 ;
	return(fdist) ;
}

//
// TM_EstimateSpeed
//
//   returns the estimated speed in km/h
//
int TM_EstimateSpeed(double dElapsed_distance, double drpm_Ts, struct TM_struct tm)
{
	double dBuf = (dElapsed_distance * 3600) / drpm_Ts ;
	return(round(dBuf)) ;
}