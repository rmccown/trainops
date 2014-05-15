#ifndef __SENDFILETO_H__
#define __SENDFILETO_H__

#include "stdafx.h"
#include <mapi.h>

class CSendFileTo
{
    HWND m_hWndParent;
    
    enum Error {SUCCESS, NO_ATTACHMENT=0x1001, NOT_A_WINDOW, LIB_LOAD_FAILED, GETPROC_FAILED};
    
public:
    CSendFileTo(HWND hWnd)
    {
        m_hWndParent = hWnd;
    }
    
    unsigned int SendMail( CString const &strAttachmentFileName,
        CString const &strSubject=_T(""),
        CString const &strMessage=_T(""),
        CString const &strRecipient=_T(""))
    {
        if (strAttachmentFileName.IsEmpty())
            return NO_ATTACHMENT;
        
        if (!m_hWndParent || !::IsWindow(m_hWndParent))
            return NOT_A_WINDOW;
        
        HINSTANCE hMAPI = ::LoadLibraryA(_T("MAPI32.DLL"));
        if (!hMAPI)
            return LIB_LOAD_FAILED;
        
        ULONG (PASCAL *SendMail)(ULONG, HWND, MapiMessage*, FLAGS, ULONG);
        (FARPROC&)SendMail = GetProcAddress(hMAPI, _T("MAPISendMail"));
        
        if (!SendMail)
            return GETPROC_FAILED;
        
        TCHAR szFileName[_MAX_PATH];
        TCHAR szPath[_MAX_PATH];
        TCHAR szSubject[_MAX_PATH];
        TCHAR szMessage[_MAX_PATH];
        TCHAR szRecipient[_MAX_PATH];
        strcpy(szFileName, (LPCTSTR)strAttachmentFileName);
        strcpy(szPath, (LPCTSTR)strAttachmentFileName);
        strcpy(szSubject, (LPCTSTR)strSubject);
        strcpy(szMessage, (LPCTSTR)strMessage);
        strcpy(szRecipient, (LPCTSTR)strRecipient);
        
        MapiFileDesc fileDesc;
        ZeroMemory(&fileDesc, sizeof(fileDesc));
        fileDesc.nPosition = (ULONG)-1;
        fileDesc.lpszPathName = szPath;
        fileDesc.lpszFileName = szFileName;

        MapiRecipDesc Recipients;
        Recipients.lpszName = szRecipient;
        Recipients.lpszAddress = szRecipient;
		Recipients.ulReserved = 0;
		Recipients.ulRecipClass = MAPI_TO;
		Recipients.ulEIDSize = 0;
		Recipients.lpEntryID = 0;
        
        MapiMessage message;
        ZeroMemory(&message, sizeof(message));
        message.lpszSubject = szSubject;
        message.nFileCount = 1;
        message.lpFiles = &fileDesc;
        message.lpszNoteText = szMessage;
        message.nRecipCount = 1;
        message.lpRecips = &Recipients;
        
        int nError = SendMail(0, m_hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);
        
        if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
            return nError;
        
        return SUCCESS;
    }
    
    
    void ShowError(UINT errCod)
    {
        if (errCod)
        {
            CString str;
            str.Format("Error: %X", errCod);
            AfxMessageBox(str);
        }
    }
};

#endif 