#if !defined(AFX_CARTYPESDIALOG_H__86B1DA4B_5F7A_46A4_AB6F_034749AC3CBA__INCLUDED_)
#define AFX_CARTYPESDIALOG_H__86B1DA4B_5F7A_46A4_AB6F_034749AC3CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarTypesDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCarTypesDialog dialog

class CCarTypesDialog : public CDialog
{
// Construction
public:
	CCarTypesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCarTypesDialog)
	enum { IDD = IDD_CARTYPES_DIALOG };
	CButton	m_ctlEditButton;
	CButton	m_ctlAddButton;
	CButton	m_ctlDeleteButton;
	CTOListCtrl	m_ctlCarTypes;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarTypesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CCarTypesDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddButton();
	afx_msg void OnDeleteButton();
	virtual void OnOK();
	afx_msg void OnEditButton();
	afx_msg void OnClickCartypeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCartypeList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARTYPESDIALOG_H__86B1DA4B_5F7A_46A4_AB6F_034749AC3CBA__INCLUDED_)
