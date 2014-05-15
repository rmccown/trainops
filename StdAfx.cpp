// stdafx.cpp : source file that includes just the standard includes
//	TrainOps.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

/*
 *  Global preferences
 */

int		g_iCarLength;
int		g_iLoadEmptyDays;
CString	g_sReportingMarks;
CString	g_sRRName;
int		g_iMethod;
int     g_iOffLayoutTime;
CString g_sLogo;
BOOL    g_bRealisticReports;
CString g_sSchemaPath;

/*
 *  Helper functions
 */

/*
 *  Checks if a file exists.
 */
bool FileExists(LPCSTR dirName)
{
    return (::PathFileExists(dirName));
}



