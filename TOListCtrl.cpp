// TOListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "trainops.h"
#include "TOListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTOListCtrl

CTOListCtrl::CTOListCtrl()
{
    m_colRow1 = RGB(220,240,220);
    m_colRow2 = RGB(247,247,247);
}

CTOListCtrl::~CTOListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTOListCtrl, CListCtrl)
//{{AFX_MSG_MAP(CTOListCtrl)
ON_WM_ERASEBKGND()
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTOListCtrl message handlers


void CTOListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
    
    LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
    int iRow = lplvcd->nmcd.dwItemSpec;
    
    switch(lplvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT :
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            return;
        }
        
        // Modify item text and or background
    case CDDS_ITEMPREPAINT:
        {
            lplvcd->clrText = RGB(0,0,0);
            // If you want the sub items the same as the item,
            // set *pResult to CDRF_NEWFONT
            *pResult = CDRF_NOTIFYSUBITEMDRAW;
            return;
        }
        
        // Modify sub item text and/or background
    case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
        {
            
            if(iRow %2){
                lplvcd->clrTextBk = m_colRow2;
            }
            else{
                lplvcd->clrTextBk = m_colRow1;
            }
            
            
            *pResult = CDRF_DODEFAULT;
            return;
        }
    }
    
}

BOOL CTOListCtrl::OnEraseBkgnd(CDC* pDC) 
{
    // TODO: Add your message handler code here and/or call default
    
    CRect rect;
    CTOListCtrl::GetClientRect(rect);
    
    
    POINT mypoint;  
    
    CBrush brush0(m_colRow1);
    CBrush brush1(m_colRow2);
    
    
    
    int chunk_height=GetCountPerPage();
    pDC->FillRect(&rect,&brush1);
    
    for (int i=0;i<=chunk_height;i++)
    {
        
        
        GetItemPosition(i,&mypoint);
        rect.top=mypoint.y ;
        GetItemPosition(i+1,&mypoint);
        rect.bottom =mypoint.y;
        pDC->FillRect(&rect,i %2 ? &brush1 : &brush0);
        
        
    }
    
    brush0.DeleteObject();
    brush1.DeleteObject();
    
    return FALSE;
}


///////////
//  state management
///////////


/*** Restore attributes of this list view control ****************************/
BOOL CTOListCtrl::RestoreState(LPCTSTR pszSection, LPCTSTR pszEntry)
{
    BOOL bSuccess = FALSE;
    
    LPBYTE pState;
    UINT   nStateLen;
    
    if (AfxGetApp()->GetProfileBinary(
        pszSection, pszEntry, &pState, &nStateLen))
    {
        bSuccess = SetState(pState, nStateLen);
        delete[] pState;
    }
    
    return bSuccess;
}

/*** Save attributes of this list view control *******************************/
BOOL CTOListCtrl::SaveState(LPCTSTR pszSection, LPCTSTR pszEntry)
{
    LPBYTE pState;
    int   nStateLen;
    
    if (GetState(&pState, &nStateLen))           // get current state
    {
        BOOL bSuccess =
            AfxGetApp()->WriteProfileBinary(pszSection, pszEntry, pState, nStateLen);
        
        delete[] pState;
        return bSuccess;
    }
    else
        return FALSE;
}

BOOL CTOListCtrl::GetState(LPBYTE* ppState, int* pnStateLen) 
{
    int nColumnCount = GetHeaderCtrl()->GetItemCount();
    
    if (nColumnCount > 0 && ppState)
    {
        UINT nStateLen =
            sizeof(int) + ( nColumnCount * (sizeof(int)));
        
        *ppState = new BYTE[nStateLen];
        LPBYTE p = *ppState;
        
        *reinterpret_cast<LPINT>(p) = nColumnCount;
        p                          += sizeof(int);
        
        for (int nColumn = 0; nColumn < nColumnCount; ++nColumn)
        {
            *reinterpret_cast<LPINT>(p) = GetColumnWidth(nColumn);
            p                          += sizeof(int);
        }
        
        if (pnStateLen) 
            *pnStateLen = nStateLen;
        return TRUE;
    }
    else
        return FALSE;
}

/*** Set attributes of this list view control ********************************/
BOOL CTOListCtrl::SetState(LPBYTE pState, int nStateLen)
{
    int nColumnCount = GetHeaderCtrl()->GetItemCount();
    pState += sizeof(int);
    int nColumn = *reinterpret_cast<LPINT>(pState);
    for (nColumn = 0; nColumn < nColumnCount; ++nColumn)
    {
        SetColumnWidth(nColumn, *reinterpret_cast<LPINT>(pState));
        pState += sizeof(int);
    }
    return TRUE;
}

