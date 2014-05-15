#if !defined(AFX_INDUSTRYDETAILDIALOG_CARS_H__7A473B14_18E2_4BDA_8307_AEE5995AA461__INCLUDED_)
#define AFX_INDUSTRYDETAILDIALOG_CARS_H__7A473B14_18E2_4BDA_8307_AEE5995AA461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryDetailDialog_Cars.h : header file
//
#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog_Cars dialog

class CIndustryDetailDialog_Cars : public CDialog
{
// Construction
public:
enum
{
    IDD_C_TYPE=0,
    IDD_C_TEXT,
    IDD_C_TIME,
    IDD_C_HIGH,
    IDD_C_LOW,
    IDD_C_PCT,
    IDD_C_PASS,
};

	CIndustryDetailDialog_Cars(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryDetailDialog_Cars)
	enum { IDD = IDD_INDUSTRYDETAILDIALOG_CARS };
	CTOListCtrl	m_ctlCarListOut;
	CTOListCtrl	m_ctlCarListIn;
	//}}AFX_DATA
    void SetIndustry(int iIndustryID) { m_iIndustryID = iIndustryID; } ;

    void Load();
    void Save(int iIndustryFK);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryDetailDialog_Cars)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iIndustryID;
	// Generated message map functions
	//{{AFX_MSG(CIndustryDetailDialog_Cars)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedCarlistin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCarlistout(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCarlistin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCarlistout(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILDIALOG_CARS_H__7A473B14_18E2_4BDA_8307_AEE5995AA461__INCLUDED_)
