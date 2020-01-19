/* ---------- ---------- ---------- ---------- ---------- ---------- ----------
OMX2GPX - A simple Geonaute OnMove 220 GPS watch OMH/OMD to GPX converter.
Code: Copyright (C) 2016 Clement CORDE - c1702@yahoo.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
---------- ---------- ---------- ---------- ---------- ---------- ---------- */


// *** Header: Fixed size, 60 bytes.

#ifndef FORMAT_H
#define FORMAT_H

#pragma pack(1)
struct SHeader
{
	// Session statistics.
	u32	nDistance;		// In meters.
	u16	nTime;			// In seconds.
	u16	nSpeedAvg;		// In decameters/h.
	u16	nSpeedMax;		// In decameters/h.
	u16	nKCal;
	u8	nHeartRateAvg;	// In bpm.
	u8	nHeartRateMax;	// In bpm.

	// Date and time of session.
	u8	nYear;			// +2000
	u8	nMonth;
	u8	nDay;
	u8	nHours;
	u8	nMinutes;

	u8	nFileNumber0;	// Fn0 == Fn1 == Fn2.

	u16	nGPSPoints;		// Number of GPS points recorded in OMD. The count does NOT include 'curve' records!
	u8	nGPSOff;		// 0 = GPS on. / *** off? to test: non zero assumed ***

	u8	pReserved0[15];			// *** t[0] = 1 when cardio belt present. (bit or byte ?).

	u8	nFileNumber1;	// Fn0 == Fn1 == Fn2.
	u8	nMagicNumber1;	// Always 0xFA.

	// Target data.
	u16	nTargetTimeBelow;		// Time below min. In seconds.
	u16	nTargetTimeIn;			// Time within min and max. In seconds.
	u16	nTargetTimeAbove;		// Time above max. In seconds.
	u16	nTargetSpeedMin;		// If target is Speed, in decameters/h. / If target is Pace, in seconds.
	u16	nTargetSpeedMax;		// If target is Speed, in decameters/h. / If target is Pace, in seconds.
	u8	nTargetHeartRateMin;	// When target is HR, in bpm.
	u8	nTargetHeartRateMax;	// When target is HR, in bpm.
	u8	nTargetMode;			// 0 = Not set, 1 = Speed, 2 = HR, 3 = Pace.

	u8	pReserved1[5];

	u8	nFileNumber2;	// Fn0 == Fn1 == Fn2.
	u8	nMagicNumber2;	// Always 0xF0.

};
#pragma pack()

enum
{
	e_OMH_TARGET_SPEED	=	1,
	e_OMH_TARGET_HR		=	2,
	e_OMH_TARGET_PACE	=	3,

	e_OMH_TARGET_Max,
};


// *** Data: List of 20 bytes chunks.

// From what I understand, there are two F1 records (let's say g0 and g1) followed by one F2 record.
// The first part of the F2 record contents additional info for g0, the second part is the same for g1.
// On the last iteration, if there is only one F1 record, all data from the part 2 of the following F2 record is set to 0 and should be ignored.

// Blank struct, used to check type.
#pragma pack(1)
struct SDataRecordType
{
	u8	pPad[19];
	u8	nType;			// 0xF1 = GPS record / 0xF2 = Curve record.
};
#pragma pack()

enum
{
	e_OMD_GPS_RECORD	=	0xF1,
	e_OMD_CURVE_RECORD	=	0xF2,
};

// e_OMD_GPS_RECORD - 0xF1. (Maps SDataRecordType).
#pragma pack(1)
struct SDataRecordGPS
{
	s32	nLatitude;		// to / by 1000000.
	s32	nLongitude;		// to / by 1000000.
	u32	nDistance;		// Distance ran from start, in meters.
	u16	nTime;			// Time from start, in seconds.

		u8	nGPSStatus;	// *** ??? Always 3 in my files. Number of satellites maybe? ***

	u8	pReserved0[4];

	u8	nType;			// 0xF1: GPS record.
};
#pragma pack()

#pragma pack(1)
struct SHalfCurve
{
	u16	nTime;			// In seconds.
	u16	nSpeed;			// In decameters/h.
	u16	nKCal;
	u8	nHeartRate;		// In bpm.

		u8	nLap;		// *** ??? ***
		u8	nCad;		// *** Cadence ??? *** => ??? 0 without cardio belt, 2 with cardio belt. Except on last F2 record. ???

	u8	nPad0;			// Padding on first half, 0xF2 on second half.
};
#pragma pack()

// e_OMD_CURVE_RECORD - 0xF2.  (Maps SDataRecordType).
#pragma pack(1)
struct SDataRecordCurve
{
	struct SHalfCurve	sHalf0;
	struct SHalfCurve	sHalf1;		// Last byte == 0xF2: Curve record.
};
#pragma pack()

struct SOptions
{
	u8	nOptions;

	// Zero point data, if provided.
	struct SDataRecordGPS	sZeroGPS;
	struct SHalfCurve		sZeroCurve;
};
// Awful, global variable
struct SOptions	gOptions;


enum
{
	e_OPT_HeaderOnly		= 1,		// Display header only.
	e_OPT_GPXFile			= 1 << 1,	// Generate GPX file.
	e_OPT_GPSDataDisplay	= 1 << 2,	// Display GPS data to screen.
	e_OPT_Elevation			= 1 << 3,	// Retrieve elevation.
	e_OPT_ZeroPt			= 1 << 4,	// 1 = Zero point information provided.
	e_OPT_Verbose			= 1 << 5,	// Verbose output
	
	e_OPT_DefaultOptions	= (e_OPT_GPSDataDisplay | e_OPT_GPXFile)
};

enum {
  NO_ERROR			= 0,
  COMMANDLINE_ERROR = 1,
  HEADER_ERROR		= 2,
  DATA_ERROR		= 3,
};

#endif
