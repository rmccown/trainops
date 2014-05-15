// TrainOpsView.cpp : implementation of the CTrainOpsView class
//

#include "stdafx.h"
#include "TrainOps.h"

#include "TrainOpsDoc.h"
#include "TrainOpsView.h"

#include "Tools.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView

IMPLEMENT_DYNCREATE(CTrainOpsView, CView)

BEGIN_MESSAGE_MAP(CTrainOpsView, CView)
	//{{AFX_MSG_MAP(CTrainOpsView)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView construction/destruction

CTrainOpsView::CTrainOpsView()
{
	// TODO: add construction code here

}

CTrainOpsView::~CTrainOpsView()
{
}

BOOL CTrainOpsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView drawing

void CTrainOpsView::OnDraw(CDC* pDC)
{
	CTrainOpsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

/*
    if( ((CTrainOpsApp*)AfxGetApp())->m_pActiveLockMfc )
    {
        if( ((CTrainOpsApp*)AfxGetApp())->m_pActiveLockMfc->IsTrialLicense() )
        {
            CFont Font;
            Font.CreatePointFont(140,"Arial");
            CFont *pOldFont = pDC->SelectObject(&Font);
            CRect rect;          // Create a CRect object
    	    GetClientRect(rect); // Retrieve the view's rectangle into the rect object
	        pDC->DrawText("TrainOps Evaluation Version - Visit http://www.largescalecentral.com/TrainOps", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); 
            pDC->SelectObject(pOldFont);
        }
    }
*/
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView printing

BOOL CTrainOpsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTrainOpsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTrainOpsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView diagnostics

#ifdef _DEBUG
void CTrainOpsView::AssertValid() const
{
	CView::AssertValid();
}

void CTrainOpsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTrainOpsDoc* CTrainOpsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTrainOpsDoc)));
	return (CTrainOpsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsView message handlers

void CTrainOpsView::OnNcPaint ()
{
    Default();

    CWindowDC cDC (this);
    CWindowRect rc (this);
    CPenDC pen (cDC, ::GetSysColor (COLOR_3DSHADOW));
    CBrushDC brush (cDC);
    cDC.Rectangle (0, 0, rc.Width(), rc.Height());
}

void CTrainOpsView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// Set flat border
    //ModifyStyle (0, WS_BORDER);
    //ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
    //GetParentFrame()->RecalcLayout();	
}
