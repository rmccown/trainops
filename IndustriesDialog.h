#if !defined(AFX_INDUSTRIESDIALOG_H__05241B4A_C910_4C45_B873_F3EDBA30DE1C__INCLUDED_)
#define AFX_INDUSTRIESDIALOG_H__05241B4A_C910_4C45_B873_F3EDBA30DE1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustriesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndustriesDialog dialog

class CIndustriesDialog : public CDialog
{
// Construction
public:
	CIndustriesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustriesDialog)
	enum { IDD = IDD_INDUSTRYDIALOG };
	CButton	m_ctlEditButton;
	CButton	m_ctlDeleteButton;
	CButton	m_ctlAddButton;
	CTreeCtrl	m_ctlIndustryTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustriesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CIndustriesDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbutton();
	afx_msg void OnEditbutton();
	afx_msg void OnDeletebutton();
	virtual void OnOK();
	afx_msg void OnSelchangedIndustrytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkIndustrytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRIESDIALOG_H__05241B4A_C910_4C45_B873_F3EDBA30DE1C__INCLUDED_)
