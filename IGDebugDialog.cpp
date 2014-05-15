// IGDebugDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrainOps.h"
#include "IGDebugDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIGDebugDialog dialog


CIGDebugDialog::CIGDebugDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIGDebugDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIGDebugDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIGDebugDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIGDebugDialog)
	DDX_Control(pDX, IDC_DEBUGLIST, m_ctlDebugList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIGDebugDialog, CDialog)
	//{{AFX_MSG_MAP(CIGDebugDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIGDebugDialog message handlers

void CIGDebugDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CIGDebugDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST );
	
    m_ctlDebugList.InsertColumn(0,"Debug Message");
    m_ctlDebugList.SetColumnWidth(0,260);

    ClearDebug();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIGDebugDialog::ClearDebug()
{
    m_ctlDebugList.DeleteAllItems();
    m_ctlDebugList.InsertItem(1,"Debug Start");
}

void CIGDebugDialog::AddDebug(CString sMsg)
{
    m_ctlDebugList.InsertItem(m_ctlDebugList.GetItemCount()+1,sMsg);
}