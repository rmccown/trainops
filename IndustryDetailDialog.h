#if !defined(AFX_INDUSTRYDETAILDIALOG_H__AF94A8C6_D021_4F68_9A0D_6507C45A019B__INCLUDED_)
#define AFX_INDUSTRYDETAILDIALOG_H__AF94A8C6_D021_4F68_9A0D_6507C45A019B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndustryDetailDialog.h : header file
//

#include "IndustryDetailTabCtrl.h"

//#include "CheckableGroupBox.h"
#include "ColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialog dialog

class CIndustryDetailDialog : public CDialog
{
// Construction
public:
	CIndustryDetailDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndustryDetailDialog)
	enum { IDD = IDD_INDUSTRYDETAILDIALOG };
	CColorStatic	m_ctlRatioError;
	CindustryDetailTabCtrl	m_ctlTab;
	CEdit	m_ctlIndustryName;
	CComboBox	m_ctlSidingCombo;
	CString	m_sIndustryName;
	CString	m_sSidingName;
	CString	m_sLocalInstructions;
	CString	m_sRatioText;
	CString	m_sRatioError;
	CString	m_sUsageText;
	//}}AFX_DATA

    void SetIndustryID(int iSidingID);
    void SetIndustryDefault();
//    void SetIndustrySiding(CString sIndustrySiding) { m_sIndustrySiding = sIndustrySiding; } ;
    void SetIndustrySiding(int iSiding) { m_iSiding = iSiding; } ;

    void RecalcRatios();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndustryDetailDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_iIndustryID;
    //CString m_sIndustrySiding;
    int m_iSiding;
	// Generated message map functions
	//{{AFX_MSG(CIndustryDetailDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnItemchangedCarlistIn(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnItemchangedCarlistOut(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnDblclkCarlistin(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnDblclkCarlistout(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnItemchangedCommoditylistIn(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnItemchangedCommoditylistOut(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnDblclkCommoditylistin(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnDblclkCommoditylistout(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDUSTRYDETAILDIALOG_H__AF94A8C6_D021_4F68_9A0D_6507C45A019B__INCLUDED_)
