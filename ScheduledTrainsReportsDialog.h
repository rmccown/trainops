#if !defined(AFX_SCHEDULEDTRAINSREPORTSDIALOG_H__C6CB089E_EF7C_4294_A649_E204FCB12776__INCLUDED_)
#define AFX_SCHEDULEDTRAINSREPORTSDIALOG_H__C6CB089E_EF7C_4294_A649_E204FCB12776__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScheduledTrainsReportsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScheduledTrainsReportsDialog dialog

class CScheduledTrainsReportsDialog : public CDialog
{
// Construction
public:
	CScheduledTrainsReportsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScheduledTrainsReportsDialog)
	enum { IDD = IDD_SCHEDULEDTRAINSREPORTSDIALOG };
	CButton	m_ctlPrintButton;
	CTreeCtrl	m_ctlTrainsTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduledTrainsReportsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    LRESULT OnCheckStateChanged(WPARAM pParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CScheduledTrainsReportsDialog)
	afx_msg void OnPrint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTrainsTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHEDULEDTRAINSREPORTSDIALOG_H__C6CB089E_EF7C_4294_A649_E204FCB12776__INCLUDED_)
