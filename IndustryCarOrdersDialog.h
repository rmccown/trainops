#if !defined(AFX_INDUSTRYCARORDERSDIALOG_H__1371D2E2_8E53_474C_9C92_AF0BA38C7D61__INCLUDED_)
#define AFX_INDUSTRYCARORDERSDIALOG_H__1371D2E2_8E53_474C_9C92_AF0BA38C7D61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryCarOrdersDialog.h : header file
//

#include "ResizableDialog.h"
#include "TreeCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarOrdersDialog dialog

class CIndustryCarOrdersDialog : public CResizableDialog
{
// Construction
public:
	CIndustryCarOrdersDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryCarOrdersDialog)
	enum { IDD = IDD_INDUSTRYCARORDERSDIALOG };
	CTreeCtrlEx	m_ctlIndustryCarOrderTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryCarOrdersDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIndustryCarOrdersDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYCARORDERSDIALOG_H__1371D2E2_8E53_474C_9C92_AF0BA38C7D61__INCLUDED_)
