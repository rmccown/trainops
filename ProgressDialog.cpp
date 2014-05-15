// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog


CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	m_sStatusText = _T("");
	//}}AFX_DATA_INIT
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	DDX_Text(pDX, IDC_STATUSTEXT, m_sStatusText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers

void CProgressDialog::OnOK()
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination\n");
        // The UpdateData routine will set focus to correct item
        return;
    }
    DestroyWindow();
}

void CProgressDialog::OnCancel()
{
    DestroyWindow();
}

void CProgressDialog::PostNcDestroy()
{
    delete this;
}

BOOL CProgressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_bUp = TRUE;
    m_bUp = 1;
    m_ctlProgress.SetRange(1,10);
    m_ctlProgress.SetPos(1);

    SetTimer(1,500,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDialog::OnTimer(UINT nIDEvent) 
{
    m_ctlProgress.SetPos(m_iCount);
    UpdateData(FALSE);
    //
    if( m_bUp )
        m_iCount++;
    else
        m_iCount--;

    if( m_iCount == 10 )
        m_bUp = FALSE;
    if( m_iCount == 1 )
        m_bUp = TRUE;

	CDialog::OnTimer(nIDEvent);
}
