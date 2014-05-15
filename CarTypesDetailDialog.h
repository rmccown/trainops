#if !defined(AFX_CARTYPESDETAILDIALOG_H__DB8E02FD_19C0_441A_BC06_1CD94A535425__INCLUDED_)
#define AFX_CARTYPESDETAILDIALOG_H__DB8E02FD_19C0_441A_BC06_1CD94A535425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarTypesDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCarTypesDetailDialog dialog

class CCarTypesDetailDialog : public CDialog
{
// Construction
public:
	CCarTypesDetailDialog(CWnd* pParent = NULL);   // standard constructor
    void SetCarType(int iCarTypeFK) { m_iCarTypeFK = iCarTypeFK; } ;
// Dialog Data
	//{{AFX_DATA(CCarTypesDetailDialog)
	enum { IDD = IDD_CARTYPE_DETAIL_DIALOG };
	BOOL	m_bPassenger;
	CString	m_sCarTypeDescription;
	CString	m_sCarTypeID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarTypesDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iCarTypeFK;

	// Generated message map functions
	//{{AFX_MSG(CCarTypesDetailDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARTYPESDETAILDIALOG_H__DB8E02FD_19C0_441A_BC06_1CD94A535425__INCLUDED_)
