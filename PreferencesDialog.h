#if !defined(AFX_PREFERENCESDIALOG_H__030111E1_B11E_4392_9E2F_062473FF7E83__INCLUDED_)
#define AFX_PREFERENCESDIALOG_H__030111E1_B11E_4392_9E2F_062473FF7E83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesDialog.h : header file
//

#include "amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog dialog

class CPreferencesDialog : public CDialog
{
// Construction
public:
	CPreferencesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreferencesDialog)
	enum { IDD = IDD_PREFDIALOG };
	CAMSDateEdit	m_ctlRRDate;
	CButton	m_ctlOK;
	int		m_iCarLength;
	int		m_iLoadEmptyDays;
	CString	m_sReportingMarks;
	CString	m_sRRName;
	int		m_iMethod;
	int		m_iOffLayoutTime;
	CString	m_sLogoFile;
	BOOL	m_bRealisticPrintout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferencesDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESDIALOG_H__030111E1_B11E_4392_9E2F_062473FF7E83__INCLUDED_)
