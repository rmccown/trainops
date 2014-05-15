#if !defined(AFX_CABOOSEDIALOG_H__9A9AC843_A0B8_4D3B_A5FA_4E10A4E693B6__INCLUDED_)
#define AFX_CABOOSEDIALOG_H__9A9AC843_A0B8_4D3B_A5FA_4E10A4E693B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CabooseDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCabooseDialog dialog

class CCabooseDialog : public CDialog
{
// Construction
public:
	CCabooseDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCabooseDialog)
	enum { IDD = IDD_CABOOSEDIALOG };
	CTOListCtrl	m_ctlCabooseList;
	CButton	m_ctlAddButton;
	CButton	m_ctlEditButton;
	CButton	m_ctlDeleteButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCabooseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();

	// Generated message map functions
	//{{AFX_MSG(CCabooseDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditbutton();
	afx_msg void OnDeletebutton();
	afx_msg void OnAddbutton();
	afx_msg void OnClickCabooselist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCabooselist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CABOOSEDIALOG_H__9A9AC843_A0B8_4D3B_A5FA_4E10A4E693B6__INCLUDED_)
