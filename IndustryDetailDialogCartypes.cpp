// IndustryDetailDialogCartypes.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IndustryDetailDialogCartypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogCartypes dialog


CIndustryDetailDialogCartypes::CIndustryDetailDialogCartypes(CWnd* pParent /*=NULL*/)
	: CDialog(CIndustryDetailDialogCartypes::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndustryDetailDialogCartypes)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndustryDetailDialogCartypes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndustryDetailDialogCartypes)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndustryDetailDialogCartypes, CDialog)
	//{{AFX_MSG_MAP(CIndustryDetailDialogCartypes)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndustryDetailDialogCartypes message handlers
