#if !defined(AFX_EDITCOMMODITYDIALOG_H__44177157_1F12_42A5_9E37_E6C360CE118F__INCLUDED_)
#define AFX_EDITCOMMODITYDIALOG_H__44177157_1F12_42A5_9E37_E6C360CE118F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCommodityDialog.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CEditCommodityDialog dialog

class CEditCommodityDialog : public CDialog
{
// Construction
public:
	CEditCommodityDialog(CWnd* pParent = NULL);   // standard constructor

    void SetCommodityID(int iCommodityID) { m_iCommodityID = iCommodityID; } ;
// Dialog Data
	//{{AFX_DATA(CEditCommodityDialog)
	enum { IDD = IDD_EDITCOMMODITYDIALOG };
	CTOListCtrl	m_ctlCarTypes;
	CString	m_sCommodityName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCommodityDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iCommodityID;
	// Generated message map functions
	//{{AFX_MSG(CEditCommodityDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOMMODITYDIALOG_H__44177157_1F12_42A5_9E37_E6C360CE118F__INCLUDED_)
