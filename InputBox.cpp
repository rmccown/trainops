#include "stdafx.h"
#include "InputBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CInputBox, CFrameWnd)

CInputBox::CInputBox(CWnd *pParent)
{
	this->pParent=pParent;
}

CInputBox::~CInputBox()
{	
}

CInputBox::CInputBox()
{
}

BEGIN_MESSAGE_MAP(CInputBox, CFrameWnd)
	//{{AFX_MSG_MAP(CInputBox)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(5000, OnOk)
	ON_BN_CLICKED(5050, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CInputBox::ShowInputBox(CString strPrompt,CString strTitle,CString strDefault)
{
	this->strDefault=strDefault;
	this->strPrompt=strPrompt;
	this->strTitle=strTitle;
	Create(NULL,strTitle,WS_CAPTION|WS_SYSMENU,
		CRect(0,0,0,0),pParent);	
	pParent->EnableWindow(false);	
	MoveWindow(100,100,300,130);
	ShowWindow(SW_SHOW);
	UpdateWindow();	

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) 
	{		
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
		if (msg.message==WM_USER+1)
			break; 
	}	
	 
	ShowWindow(SW_HIDE);
	return retvalue; 

}

void CInputBox::OnClose() 
{
	pParent->SetForegroundWindow();
	pParent->EnableWindow(true);
	
	CFrameWnd::OnClose();
}


int CInputBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	m_dummy.Create("",WS_VISIBLE,CRect(0,0,300,130),this);

	m_prompt.Create(strPrompt,WS_VISIBLE,
		CRect(10,5,280,40),this,3000);	
	m_edit.Create(WS_VISIBLE|WS_BORDER,CRect(10,50,280,70),this,4000);
	m_ok.Create("OK",WS_VISIBLE,CRect(80,75,150,95),this,5000);
	m_cancel.Create("Cancel",WS_VISIBLE,CRect(170,75,250,95),this,5050);
	
	m_edit.SetWindowText(strDefault);
	m_edit.SetFocus();
	
	return 0;
}

void CInputBox::OnCancel()
{
	retvalue=IDCANCEL;
	InputText="";
	PostMessage(WM_USER+1);
	
}


void CInputBox::OnOk()
{
	retvalue=IDOK;
	m_edit.GetWindowText(InputText);
	PostMessage(WM_USER+1);	
}

void CInputBox::CloseBox()
{
	PostMessage(WM_CLOSE);
}
