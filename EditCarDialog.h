#if !defined(AFX_EDITCARDIALOG_H__9D732E0D_5C3B_4E56_81B0_993641A0A09C__INCLUDED_)
#define AFX_EDITCARDIALOG_H__9D732E0D_5C3B_4E56_81B0_993641A0A09C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCarDialog.h : header file
//

#include "CheckableGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEditCarDialog dialog

class CEditCarDialog : public CDialog
{
// Construction
public:
	CEditCarDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditCarDialog)
	enum { IDD = IDD_EDITCARDIALOG };
	CStatic	m_ctlInterchangeViaText;
	CComboBox	m_ctlInterchangeVia;
	CComboBox	m_ctlReturnTo;
	CDateTimeCtrl	m_ctlMaintDate;
	CComboBox	m_ctlCommodity;
	CString	m_sCarNumber;
	CString	m_sReportingMarks;
	CString	m_sCarLength;
	CString	m_sWeight;
	BOOL	m_bForeign;
	CTime	m_pMaintDate;
	CString	m_sNotes;
	BOOL	m_bOutOfService;
	BOOL	m_bChangeLastMaint;
	CString	m_sReturnTo;
	CString	m_sInterchangeVia;
	//}}AFX_DATA
    void SetCarID(int iID) { m_iCarID = iID; } ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCarDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iCarID;

	// Generated message map functions
	//{{AFX_MSG(CEditCarDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLastmaint();
	afx_msg void OnForeign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCARDIALOG_H__9D732E0D_5C3B_4E56_81B0_993641A0A09C__INCLUDED_)
