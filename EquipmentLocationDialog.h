#if !defined(AFX_EQUIPMENTLOCATIONDIALOG_H__62519324_70F5_44D3_9A90_1119164FD604__INCLUDED_)
#define AFX_EQUIPMENTLOCATIONDIALOG_H__62519324_70F5_44D3_9A90_1119164FD604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EquipmentLocationDialog.h : header file
//

#include "TreeCtrlEx.h"
#include "TOListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEquipmentLocationDialog dialog

class CEquipmentLocationDialog : public CDialog
{
// Construction
public:
	CEquipmentLocationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEquipmentLocationDialog)
	enum { IDD = IDD_EQUIPMENTLOCATIONDIALOG };
	CTOListCtrl	m_ctlCarsList;
	CTreeCtrlEx	m_ctlEquipmentTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEquipmentLocationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
    void ReadDB();
    void CleanupTree();
    void GetCars(int iType, int iKey);
    CString GetCarType(int iCarTypeFK);

protected:

	// Generated message map functions
	//{{AFX_MSG(CEquipmentLocationDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangedEquipmenttree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTLOCATIONDIALOG_H__62519324_70F5_44D3_9A90_1119164FD604__INCLUDED_)
