// NewVersionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "NewVersionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewVersionDialog dialog


CNewVersionDialog::CNewVersionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewVersionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewVersionDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewVersionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewVersionDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewVersionDialog, CDialog)
	//{{AFX_MSG_MAP(CNewVersionDialog)
	ON_BN_CLICKED(IDC_VIEWREADMEBUTTON, OnViewreadmebutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewVersionDialog message handlers

void CNewVersionDialog::OnViewreadmebutton() 
{
	// TODO: Add your control notification handler code here
    ShellExecute(m_hWnd, 0, "http://www.largescalecentral.com/TrainOps/releasenotes.txt", 0, 0, SW_SHOWDEFAULT);

}

void CNewVersionDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CNewVersionDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
    EndDialog(IDYES);
}
