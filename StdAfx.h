// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__58D46EFB_F4B3_4CFF_8894_9A130B30F4A8__INCLUDED_)
#define AFX_STDAFX_H__58D46EFB_F4B3_4CFF_8894_9A130B30F4A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#include "TBTreeStateMgr.h" //  CTreeCtrl state manager

#include "amsEdit.h"        //  Validating edit control

#include "VisualStylesXP.h"

// if you don't include shlwapi.h include next two lines
#include "shlwapi.h" 
#pragma comment (lib, "shlwapi.lib")
// This line needed
#include "RestartAPI.h"
#define IDC_RESTART 1001

//  database schema XML
//#include "dbschema.h"

extern int g_iCarLength;
extern int g_iLoadEmptyDays;
extern CString g_sReportingMarks;
extern CString g_sRRName;
extern int g_iMethod;
extern int g_iOffLayoutTime;
extern CString g_sSchemaPath;
extern CString g_sLogo;
extern BOOL g_bRealisticReports;

bool FileExists(LPCSTR dirName);

#define METHOD_CARS 0
#define METHOD_COMMODITIES 1

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__58D46EFB_F4B3_4CFF_8894_9A130B30F4A8__INCLUDED_)
