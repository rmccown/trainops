//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_EQUIPMENTNOTFOUNDDIALOG_H__A9722544_4259_4F98_AF45_8890144289C9__INCLUDED_)
#define AFX_EQUIPMENTNOTFOUNDDIALOG_H__A9722544_4259_4F98_AF45_8890144289C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EquipmentNotFoundDialog.h : header file
//

#include "TOListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CEquipmentNotFoundDialog dialog

class CEquipmentNotFoundDialog : public CDialog
{
// Construction
public:
	CEquipmentNotFoundDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEquipmentNotFoundDialog)
	enum { IDD = IDD_EQUIPMENTNOTFOUNDDIALOG };
	CTreeCtrl	m_ctlNotFoundTree;
	CTreeCtrl	m_ctlEquipmentTree;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEquipmentNotFoundDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ReadDB();
    void CleanupTree();
    void GetCars(int iCarTypeFK);

    int m_iCarTypeFK;

	// Generated message map functions
	//{{AFX_MSG(CEquipmentNotFoundDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickNotfoundlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPrunetohere();
	afx_msg void OnRclickNotfoundtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTNOTFOUNDDIALOG_H__A9722544_4259_4F98_AF45_8890144289C9__INCLUDED_)
