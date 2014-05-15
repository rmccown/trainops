#if !defined(AFX_INDUSTRYDETAILDIALOGTAB_H__21CB808C_4C74_4E61_BD80_AF4F215EB290__INCLUDED_)
#define AFX_INDUSTRYDETAILDIALOGTAB_H__21CB808C_4C74_4E61_BD80_AF4F215EB290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryDetailDialogTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogTab window

class CIndustryDetailDialogTab : public CTabCtrl
{
// Construction
public:
	CIndustryDetailDialogTab();

// Attributes
public:

//Array to hold the list of dialog boxes/tab pages for CTabCtrl
int m_DialogID[2];

int m_nPageCount;

//CDialog Array Variable to hold the dialogs 
CDialog *m_Dialog[2];

//Function to Create the dialog boxes during startup
void InitDialogs();

//Function to activate the tab dialog boxes
void ActivateTabDialogs();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryDetailDialogTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndustryDetailDialogTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndustryDetailDialogTab)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILDIALOGTAB_H__21CB808C_4C74_4E61_BD80_AF4F215EB290__INCLUDED_)
