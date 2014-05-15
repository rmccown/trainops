// TrainOpsDoc.cpp : implementation of the CTrainOpsDoc class
//

#include "stdafx.h"
#include "TrainOps.h"

#include "TrainOpsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsDoc

IMPLEMENT_DYNCREATE(CTrainOpsDoc, CDocument)

BEGIN_MESSAGE_MAP(CTrainOpsDoc, CDocument)
	//{{AFX_MSG_MAP(CTrainOpsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsDoc construction/destruction

CTrainOpsDoc::CTrainOpsDoc()
{
	// TODO: add one-time construction code here

}

CTrainOpsDoc::~CTrainOpsDoc()
{
}

BOOL CTrainOpsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    SetTitle(g_sRRName);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTrainOpsDoc serialization

void CTrainOpsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsDoc diagnostics

#ifdef _DEBUG
void CTrainOpsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTrainOpsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrainOpsDoc commands
