// TrainOpsView.h : interface of the CTrainOpsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAINOPSVIEW_H__D4635310_DB0D_491F_92E1_434C93DFCC57__INCLUDED_)
#define AFX_TRAINOPSVIEW_H__D4635310_DB0D_491F_92E1_434C93DFCC57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTrainOpsView : public CView
{
protected: // create from serialization only
	CTrainOpsView();
	DECLARE_DYNCREATE(CTrainOpsView)

// Attributes
public:
	CTrainOpsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainOpsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrainOpsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTrainOpsView)
	//}}AFX_MSG
	afx_msg void OnNcPaint();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TrainOpsView.cpp
inline CTrainOpsDoc* CTrainOpsView::GetDocument()
   { return (CTrainOpsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINOPSVIEW_H__D4635310_DB0D_491F_92E1_434C93DFCC57__INCLUDED_)
