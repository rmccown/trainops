#if !defined(AFX_INDUSTRYUSAGEDIALOG_H__CC9EDC69_AD8C_49F5_9682_A9FE99916FB8__INCLUDED_)
#define AFX_INDUSTRYUSAGEDIALOG_H__CC9EDC69_AD8C_49F5_9682_A9FE99916FB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryUsageDialog.h : header file
//

#include "TOListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CIndustryUsageDialog dialog

class CIndustryUsageDialog : public CDialog
{
// Construction
public:
	CIndustryUsageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryUsageDialog)
	enum { IDD = IDD_INDUSTRYUSAGEDIALOG };
	CTreeCtrl	m_ctlIndustryTree;
	CTOListCtrl	m_ctlIndustryDetail;
	//}}AFX_DATA

    void ReadDB();
    void GetCars(int iIndustryFK);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryUsageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIndustryUsageDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangedIndustrytree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYUSAGEDIALOG_H__CC9EDC69_AD8C_49F5_9682_A9FE99916FB8__INCLUDED_)
