#if !defined(AFX_CABOOSEDETAILDIALOG_H__C35809C3_8905_450F_9640_3B13E12A6C5D__INCLUDED_)
#define AFX_CABOOSEDETAILDIALOG_H__C35809C3_8905_450F_9640_3B13E12A6C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CabooseDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCabooseDetailDialog dialog

class CCabooseDetailDialog : public CDialog
{
// Construction
public:
	CCabooseDetailDialog(CWnd* pParent = NULL);   // standard constructor
    void SetCabooseFK(int iCabooseFK) { m_iCabooseFK = iCabooseFK;};
// Dialog Data
	//{{AFX_DATA(CCabooseDetailDialog)
	enum { IDD = IDD_CABOOSEDETAILDIALOG };
	CDateTimeCtrl	m_ctlMaintDate;
	CString	m_sCarNumber;
	CString	m_sReportingMarks;
	int		m_iLength;
	CString	m_sNotes;
	BOOL	m_bOutOfService;
	BOOL	m_bChangeLastMaint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCabooseDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iCabooseFK;
	// Generated message map functions
	//{{AFX_MSG(CCabooseDetailDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CABOOSEDETAILDIALOG_H__C35809C3_8905_450F_9640_3B13E12A6C5D__INCLUDED_)
