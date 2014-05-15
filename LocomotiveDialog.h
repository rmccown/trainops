#if !defined(AFX_LOCOMOTIVEDIALOG_H__F831D028_FC23_43BA_A5E9_72CFA5C91BBE__INCLUDED_)
#define AFX_LOCOMOTIVEDIALOG_H__F831D028_FC23_43BA_A5E9_72CFA5C91BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocomotiveDialog.h : header file
//
#include "TOListCtrl.h"
//#include "amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDialog dialog

class CLocomotiveDialog : public CDialog
{
// Construction
public:
	CLocomotiveDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLocomotiveDialog)
	enum { IDD = IDD_LOCOMOTIVEDIALOG };
	CButton	m_ctlEditButton;
	CButton	m_ctlDeleteButton;
	CTOListCtrl	m_ctlLocoList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocomotiveDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();
    void CheckControls();

    int m_iLocoID;

	// Generated message map functions
	//{{AFX_MSG(CLocomotiveDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickLocolist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeletebutton();
	afx_msg void OnEditbutton();
	afx_msg void OnNewbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCOMOTIVEDIALOG_H__F831D028_FC23_43BA_A5E9_72CFA5C91BBE__INCLUDED_)
