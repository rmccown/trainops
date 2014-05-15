#if !defined(AFX_TERMINATETRAINDIALOG_H__60D2B2E0_6CB5_4837_B514_82AF78C6830E__INCLUDED_)
#define AFX_TERMINATETRAINDIALOG_H__60D2B2E0_6CB5_4837_B514_82AF78C6830E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerminateTrainDialog.h : header file
//

#include "ResizableDialog.h"

#include "TOListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTerminateTrainDialog dialog

class CTerminateTrainDialog : public CResizableDialog
{
// Construction
public:
	CTerminateTrainDialog(CWnd* pParent = NULL);   // standard constructor

    void SetTrainID(int iTrainID) { m_iTrainID = iTrainID;};

// Dialog Data
	//{{AFX_DATA(CTerminateTrainDialog)
	enum { IDD = IDD_TERMINATETRAINDIALOG };
	CTOListCtrl	m_ctlTrainList;
	CString	m_sTrainDescription;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerminateTrainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
    int m_iTrainID;
    //CString m_sSelectedIndustry;
    //CString m_sSelectedSiding;
	// Generated message map functions
	//{{AFX_MSG(CTerminateTrainDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRclickTrainlist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
    afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMINATETRAINDIALOG_H__60D2B2E0_6CB5_4837_B514_82AF78C6830E__INCLUDED_)
