#if !defined(AFX_IGDEBUGDIALOG_H__EC35BF29_7AF1_467B_A4FB_B07BA04AC17A__INCLUDED_)
#define AFX_IGDEBUGDIALOG_H__EC35BF29_7AF1_467B_A4FB_B07BA04AC17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IGDebugDialog.h : header file
//
#include "resource.h"
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CIGDebugDialog dialog

class CIGDebugDialog : public CDialog
{
// Construction
public:
	CIGDebugDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIGDebugDialog)
	enum { IDD = IDD_DEBUGDIALOG };
	CTOListCtrl	m_ctlDebugList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIGDebugDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
    void AddDebug(CString sMsg);
    void ClearDebug();

protected:

	// Generated message map functions
	//{{AFX_MSG(CIGDebugDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IGDEBUGDIALOG_H__EC35BF29_7AF1_467B_A4FB_B07BA04AC17A__INCLUDED_)
