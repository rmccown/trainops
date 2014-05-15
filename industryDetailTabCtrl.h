#if !defined(AFX_INDUSTRYDETAILTABCTRL_H__42AFDDB0_F233_4D95_BA95_18B279451614__INCLUDED_)
#define AFX_INDUSTRYDETAILTABCTRL_H__42AFDDB0_F233_4D95_BA95_18B279451614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// industryDetailTabCtrl.h : header file
//

#include "IndustryDetailDialog_Cars.h"
#include "IndustryDetailDialog_Commodities.h"

/////////////////////////////////////////////////////////////////////////////
// CindustryDetailTabCtrl window

class CindustryDetailTabCtrl : public CTabCtrl
{
// Construction
public:
	CindustryDetailTabCtrl();

// Attributes
public:

//Array to hold the list of dialog boxes/tab pages for CTabCtrl
int m_DialogID[2];

//CDialog Array Variable to hold the dialogs 
CDialog* m_Dialog[2];
CIndustryDetailDialog_Cars* m_Dialog0;
CIndustryDetailDialog_Commodities* m_Dialog1;

//Function to Create the dialog boxes during startup
void InitDialogs();

//Function to activate the tab dialog boxes
void ActivateTabDialogs();

int m_nPageCount;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CindustryDetailTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CindustryDetailTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CindustryDetailTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILTABCTRL_H__42AFDDB0_F233_4D95_BA95_18B279451614__INCLUDED_)
