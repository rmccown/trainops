#if !defined(AFX_CTOListCtrl_H__4F457B40_61AE_4076_A528_B4BD10DD9147__INCLUDED_)
#define AFX_CTOListCtrl_H__4F457B40_61AE_4076_A528_B4BD10DD9147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TOListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTOListCtrl window

class CTOListCtrl : public CListCtrl
{
// Construction
public:
	CTOListCtrl();

// Attributes
public:
	COLORREF m_colRow1;
	COLORREF m_colRow2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTOListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTOListCtrl();

    BOOL SetState(LPBYTE pState, int nStateLen);
    BOOL GetState(LPBYTE* ppState, int* pnStateLen);
    BOOL RestoreState(LPCTSTR pszSection, LPCTSTR pszEntry);
    BOOL SaveState(LPCTSTR pszSection, LPCTSTR pszEntry);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTOListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTOListCtrl_H__4F457B40_61AE_4076_A528_B4BD10DD9147__INCLUDED_)
