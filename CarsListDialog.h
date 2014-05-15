#if !defined(AFX_CARSLISTDIALOG_H__63A4BD87_23CE_4D12_93A5_23E213472378__INCLUDED_)
#define AFX_CARSLISTDIALOG_H__63A4BD87_23CE_4D12_93A5_23E213472378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarsListDialog.h : header file
//

#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCarsListDialog dialog

class CCarsListDialog : public CDialog
{
// Construction
public:
	CCarsListDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCarsListDialog)
	enum { IDD = IDD_CARSLIST_DIALOG };
	CButton	m_ctlEditCarButton;
	CButton	m_ctlDeleteCarButton;
	CTOListCtrl	m_ctlCarList;
	//}}AFX_DATA

    void SetCarType(int iCarTypeFK) { m_iCarTypeFK = iCarTypeFK;};
    void ReadDB();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarsListDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iCarTypeFK;
	// Generated message map functions
	//{{AFX_MSG(CCarsListDialog)
	afx_msg void OnDeletecarbutton();
	afx_msg void OnEditcarbutton();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCarslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCarslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddCarButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARSLISTDIALOG_H__63A4BD87_23CE_4D12_93A5_23E213472378__INCLUDED_)
