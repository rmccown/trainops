#if !defined(AFX_INDUSTRYDETAILDIALOG_COMMODITIES_H__BDD014B1_9C72_4714_BCC3_5CCD3259632C__INCLUDED_)
#define AFX_INDUSTRYDETAILDIALOG_COMMODITIES_H__BDD014B1_9C72_4714_BCC3_5CCD3259632C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryDetailDialog_Commodities.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Commodities dialog

class CIndustryDetailDialog_Commodities : public CDialog
{
// Construction
public:
	CIndustryDetailDialog_Commodities(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryDetailDialog_Commodities)
	enum { IDD = IDD_INDUSTRYDETAILDIALOG_COMMODITIES };
	CTOListCtrl	m_ctlCommodityListOut;
	CTOListCtrl	m_ctlCommodityListIn;
	//}}AFX_DATA
    void SetIndustry(int iIndustryID) { m_iIndustryID = iIndustryID; } ;

    void Load();
    void Save();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryDetailDialog_Commodities)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iIndustryID;
	// Generated message map functions
	//{{AFX_MSG(CIndustryDetailDialog_Commodities)
	afx_msg void OnItemchangedCommoditiesinlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCommoditiesoutlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCommoditiesinlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCommoditiesoutlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILDIALOG_COMMODITIES_H__BDD014B1_9C72_4714_BCC3_5CCD3259632C__INCLUDED_)
