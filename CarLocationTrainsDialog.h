#if !defined(AFX_CARLOCATIONTRAINSDIALOG_H__DB8754A5_CCDE_4D28_9595_4BB8C4C7C423__INCLUDED_)
#define AFX_CARLOCATIONTRAINSDIALOG_H__DB8754A5_CCDE_4D28_9595_4BB8C4C7C423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarLocationTrainsDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCarLocationTrainsDialog dialog

class CCarLocationTrainsDialog : public CDialog
{
// Construction
public:
	CCarLocationTrainsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCarLocationTrainsDialog)
	enum { IDD = IDD_CARLOCATIONTRAINSDIALOG };
	CButton	m_ctlReportSortedButton;
	CButton	m_ctlReportButton;
	CTOListCtrl	m_ctlTrainList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarLocationTrainsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCarLocationTrainsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnReportbutton();
	afx_msg void OnItemchangedTrainlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportSortedButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARLOCATIONTRAINSDIALOG_H__DB8754A5_CCDE_4D28_9595_4BB8C4C7C423__INCLUDED_)
