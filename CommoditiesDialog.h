#if !defined(AFX_COMMODITIESDIALOG_H__A1A7B30E_9D47_4A2F_BD99_83E0EB5D71EF__INCLUDED_)
#define AFX_COMMODITIESDIALOG_H__A1A7B30E_9D47_4A2F_BD99_83E0EB5D71EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommoditiesDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCommoditiesDialog dialog

class CCommoditiesDialog : public CDialog
{
// Construction
public:
	CCommoditiesDialog(CWnd* pParent = NULL);   // standard constructor

    void ReadDB();
// Dialog Data
	//{{AFX_DATA(CCommoditiesDialog)
	enum { IDD = IDD_COMMODITIESDIALOG };
	CTOListCtrl	m_ctlCommodityList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommoditiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommoditiesDialog)
	afx_msg void OnDblclkCommoditylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditbutton();
	afx_msg void OnAddbutton();
	afx_msg void OnDeletebutton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMODITIESDIALOG_H__A1A7B30E_9D47_4A2F_BD99_83E0EB5D71EF__INCLUDED_)
