#if !defined(AFX_LOCATIONDIALOG_H__0295C11F_D212_43B6_94F1_1B649FFD5122__INCLUDED_)
#define AFX_LOCATIONDIALOG_H__0295C11F_D212_43B6_94F1_1B649FFD5122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocationDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CLocationDialog dialog

class CLocationDialog : public CDialog
{
// Construction
public:
	CLocationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLocationDialog)
	enum { IDD = IDD_LOCATIONDIALOG };
	CButton	m_ctlEditButton;
	CButton	m_ctlDeleteButton;
	CTOListCtrl	m_ctlLocationList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CLocationDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbutton();
	afx_msg void OnDelbutton();
	afx_msg void OnEndlabeleditLocationlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLocationlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditbutton();
	afx_msg void OnDblclkLocationlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATIONDIALOG_H__0295C11F_D212_43B6_94F1_1B649FFD5122__INCLUDED_)
