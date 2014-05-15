#if !defined(AFX_LOCOMOTIVEDETAILDIALOG_H__161EDC49_E17B_47EB_AFE7_5CDBD1561B51__INCLUDED_)
#define AFX_LOCOMOTIVEDETAILDIALOG_H__161EDC49_E17B_47EB_AFE7_5CDBD1561B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocomotiveDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLocomotiveDetailDialog dialog

class CLocomotiveDetailDialog : public CDialog
{
// Construction
public:
	CLocomotiveDetailDialog(CWnd* pParent = NULL);   // standard constructor

    void SetLocoFK(int iLocoFK) { m_iLocoFK = iLocoFK; } ;
    void CheckButtons();

// Dialog Data
	//{{AFX_DATA(CLocomotiveDetailDialog)
	enum { IDD = IDD_LOCOMOTIVEDETAILDIALOG };
	CButton	m_ctlOKButton;
	CDateTimeCtrl	m_ctlMaintDate;
	CString	m_sRoadName;
	CString	m_sRoadNumber;
	int		m_iCarCapacity;
	CString	m_sDescription;
	BOOL	m_bActive;
	CString	m_sNotes;
	CTime	m_pMaintDate;
	BOOL	m_bChangeLastMaint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocomotiveDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iLocoFK;
	// Generated message map functions
	//{{AFX_MSG(CLocomotiveDetailDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeCarcapacity();
	afx_msg void OnChangeDescription();
	afx_msg void OnChangeNumber();
	afx_msg void OnChangeRoadname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCOMOTIVEDETAILDIALOG_H__161EDC49_E17B_47EB_AFE7_5CDBD1561B51__INCLUDED_)
