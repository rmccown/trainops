#if !defined(AFX_SIDINGDETAILDIALOG_H__DDE0D338_647D_4E76_868B_8B2DF8C038F7__INCLUDED_)
#define AFX_SIDINGDETAILDIALOG_H__DDE0D338_647D_4E76_868B_8B2DF8C038F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SidingDetailDialog.h : header file
//

#include "CheckableGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSidingDetailDialog dialog

class CSidingDetailDialog : public CDialog
{
// Construction
public:
	CSidingDetailDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSidingDetailDialog)
	enum { IDD = IDD_SIDINGDETAILDIALOG };
	CCheckableGroupBox	m_ctlIsYard;
	CComboBox	m_ctlLocationCombo;
	int		m_iCapacity;
	CString	m_sLocation;
	CString	m_sSidingName;
	BOOL	m_bGetEmpties;
    BOOL    m_bIsYard;
	BOOL	m_bInterchange;
	//}}AFX_DATA

    void SetSidingID(int iSidingID);
    void SetSidingDefault();
    void SetSidingLocation(CString sSidingLocation) { m_sSidingLocation = sSidingLocation; } ;
    CString GetSidingLocation() { return m_sSidingLocation;};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSidingDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iSidingID;
    CString m_sSidingLocation;

	// Generated message map functions
	//{{AFX_MSG(CSidingDetailDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDINGDETAILDIALOG_H__DDE0D338_647D_4E76_868B_8B2DF8C038F7__INCLUDED_)
