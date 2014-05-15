#if !defined(AFX_INDUSTRYDETAILDIALOGCARTYPES_H__A31A4B2E_AA0E_41BD_8AF3_5D170F7E7083__INCLUDED_)
#define AFX_INDUSTRYDETAILDIALOGCARTYPES_H__A31A4B2E_AA0E_41BD_8AF3_5D170F7E7083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryDetailDialogCartypes.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogCartypes dialog

class CIndustryDetailDialogCartypes : public CDialog
{
// Construction
public:
	CIndustryDetailDialogCartypes(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryDetailDialogCartypes)
	enum { IDD = IDD_INDUSTRYDETAILDIALOG_CARTYPES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryDetailDialogCartypes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIndustryDetailDialogCartypes)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILDIALOGCARTYPES_H__A31A4B2E_AA0E_41BD_8AF3_5D170F7E7083__INCLUDED_)
