// PDFViewerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "PDFViewerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPDFViewerDialog dialog


CPDFViewerDialog::CPDFViewerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPDFViewerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPDFViewerDialog)
	//}}AFX_DATA_INIT
}


void CPDFViewerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDFViewerDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPDFViewerDialog, CDialog)
	//{{AFX_MSG_MAP(CPDFViewerDialog)
	ON_BN_CLICKED(ID_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDFViewerDialog message handlers

void CPDFViewerDialog::OnClose() 
{
    EndDialog(0);	
}

BEGIN_EVENTSINK_MAP(CPDFViewerDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CPDFViewerDialog)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CPDFViewerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
