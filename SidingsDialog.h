#if !defined(AFX_SIDINGSDIALOG_H__001D8526_C061_4EFB_B5E0_5909D0375CC2__INCLUDED_)
#define AFX_SIDINGSDIALOG_H__001D8526_C061_4EFB_B5E0_5909D0375CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SidingsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSidingsDialog dialog

class CSidingsDialog : public CDialog
{
// Construction
public:
	CSidingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSidingsDialog)
	enum { IDD = IDD_SIDINGSDIALOG };
	CButton	m_ctlEditButton;
	CButton	m_ctlDeleteButton;
	CButton	m_ctlAddButton;
	CTreeCtrl	m_ctlSidingsTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSidingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CSidingsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbutton();
	afx_msg void OnEditbutton();
	afx_msg void OnDeletebutton();
	virtual void OnOK();
	afx_msg void OnSelchangedSidingstree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSidingstree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDINGSDIALOG_H__001D8526_C061_4EFB_B5E0_5909D0375CC2__INCLUDED_)
