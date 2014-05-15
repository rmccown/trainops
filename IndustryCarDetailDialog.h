#if !defined(AFX_INDUSTRYCARDETAILDIALOG_H__EDADBB27_B89C_4E79_8D6D_F9BF98815ADE__INCLUDED_)
#define AFX_INDUSTRYCARDETAILDIALOG_H__EDADBB27_B89C_4E79_8D6D_F9BF98815ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryCarDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndustryCarDetailDialog dialog

class CIndustryCarDetailDialog : public CDialog
{
// Construction
public:
	CIndustryCarDetailDialog(CWnd* pParent = NULL);   // standard constructor

    void SetIn() { m_bIn = TRUE;};
    void SetOut() { m_bIn = FALSE;};

    void SetLoadEmpty(int iLoadEmpty) { m_iLoadEmpty = iLoadEmpty; };
    void SetPercent(int iPercent) { m_iPercent = iPercent; };
    void SetQuanHigh(int iQuanHigh) { m_iQuanHigh = iQuanHigh; };
    void SetQuanLow(int iQuanLow) { m_iQuanLow = iQuanLow; };
    void SetPassenger(bool bPassenger) { m_bPassenger = bPassenger;};

    int GetLoadEmpty() { return m_iLoadEmpty; } ;
    int GetPercent() { return m_iPercent; } ;
    int GetQuanHigh() { return m_iQuanHigh; } ;
    int GetQuanLow() { return m_iQuanLow; } ;

// Dialog Data
	//{{AFX_DATA(CIndustryCarDetailDialog)
	enum { IDD = IDD_INDUSTRYCARDETAILDIALOG };
	CEdit	m_ctlPercent;
	CEdit	m_ctlQuanHigh;
	CButton	m_ctlOK;
	int	    m_iLoadEmpty;
	int		m_iPercent;
	int		m_iQuanHigh;
	int		m_iQuanLow;
	CString	m_sLoadEmptyText;
	//}}AFX_DATA
    BOOL m_bPassenger;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryCarDetailDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bIn;
	// Generated message map functions
	//{{AFX_MSG(CIndustryCarDetailDialog)
	virtual void OnOK();
	afx_msg void OnChangeLoadempty();
	afx_msg void OnChangePercent();
	afx_msg void OnChangeQuanhigh();
	afx_msg void OnChangeQuanlow();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYCARDETAILDIALOG_H__EDADBB27_B89C_4E79_8D6D_F9BF98815ADE__INCLUDED_)
