#if !defined(AFX_ABOUTDIALOG_H__7A3C682A_8048_46AA_A07D_541EF569C8B6__INCLUDED_)
#define AFX_ABOUTDIALOG_H__7A3C682A_8048_46AA_A07D_541EF569C8B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDialog.h : header file
//
#include "TextScroller.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog

class CAboutDialog : public CDialog
{
// Construction
public:
	CAboutDialog();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDialog)
	enum { IDD = IDD_ABOUTBOX };
	CTextScroller	m_ctlScrollText;
	CString	m_sLicense;
	CString	m_sVersionText;
	CString	m_sScrollText;
	CString	m_sCopyrightText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDIALOG_H__7A3C682A_8048_46AA_A07D_541EF569C8B6__INCLUDED_)
