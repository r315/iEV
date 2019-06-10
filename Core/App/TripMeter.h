#ifndef _tripmeter_h
#define _tripmeter_h

/**
	\defgroup tripmeter


	\brief	This module defines the structures and functions which allows the computation of tripmeter data

	\author David Pereira Coutinho (C)
	\version 1.0
	\date 21 Mar 2019
*/

#pragma once

//
// TripMeter struct definition
//
struct TM_struct
{
	double drpm_Ts;
	int irpm_ref;
	int ispeedKPH_ref;
	double ddistpermin;
	double ddistperrev;
};


// struct TM_struct tm;

//
// TripMeter function prototypes
//
void TM_Init(double drpm_Ts, int irpm_ref, int ispeedKPH_ref, struct TM_struct *ptm) ;
double TM_ComputeDistance(int irpm, double drpm_Ts, struct TM_struct tm) ;
int TM_EstimateSpeed(double dElapsed_distance, double drpm_Ts, struct TM_struct tm) ;

#endif
