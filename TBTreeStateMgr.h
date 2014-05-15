// TreeStateMgr.h: interface for the TreeStateMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREESTATEMGR_H__D810C7D2_D603_11D5_AC1A_00A00C90889A__INCLUDED_)
#define AFX_TREESTATEMGR_H__D810C7D2_D603_11D5_AC1A_00A00C90889A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml.dll" named_guids raw_interfaces_only

//#import named_guids
using namespace MSXML; 


#include <vector>
using namespace std;

/*
 ***********************************************************************
 *	TBTreeStateMgr:		Saves the state of tree controls
 *
 *  All methods are static
 *
 *  NOTE________________________________________________________________
 *	| I kindly request folks to add their revisions (name,date,comments)|
 *	|___________________________________________________________________|
 *
 *	Revisions
 *	---------
 *	5/21	Vivek_Rajan		Article for Codeproject
 ***********************************************************************
 */
class TBTreeStateMgr  
{
protected:
// [useful typedefs]
	typedef vector<BSTR>	BSTRVECT_T;

// [temp variables]
protected:
	static TCHAR			* m_StgFileName;		// IStorage name
	static CTreeCtrl		* pCtrl;				// Temp ptr to tree control
	static IStream			* pCurrStream;			// Current Stream 
	static IStorage			* pCurrStorage;			// Current storage
	static IXMLDOMDocument  * pDoc;					// XML Document
	static IXMLDOMNode		* pExpNodes;			// Expanded nodes
	static BSTRVECT_T		* v_bsStates;			// States vector 

	static CRITICAL_SECTION	  csection;				// Critical section
	static bool				  fInitialized;			// Need to init this class
	static bool				  fUsingCustomStorage;	// For memory tracking
// [client methods]
public:
	static void Initialize();
	static void Uninitialize();
	static void SetStorageFile(LPCTSTR lpszStorageName);
	static BOOL SaveTreeState (LPCTSTR lpszContext, CTreeCtrl * ctrl);
	static BOOL LoadTreeState (LPCTSTR lpszContext, CTreeCtrl * ctrl);

// [utility functions for this class]
protected:
	static BOOL			_SaveTreeState (LPCTSTR lpszContext, CTreeCtrl * ctrl);
	static BOOL			_LoadTreeState (LPCTSTR lpszContext, CTreeCtrl * ctrl);
	static void			DeepSaveItemState (HTREEITEM hItem);
	static HRESULT		OpenStorageFile(LPCTSTR lpszFn, bool fCreate=true);
	static IStream *	OpenStateStream(LPCTSTR lpszName, bool fCreate=true);
	static bool			IsExpandedState( LPCTSTR nodename);
	static HRESULT		LoadStateVector();
	static void			DeepSetItemState (HTREEITEM hItem);
	static char *		MakeValidName( LPCTSTR lpszContext);
	static IStream *	RecreateStream(LPCTSTR lpszName);

// [xml utility functions] private
private:
	static HRESULT		 CreateNewDocument(IXMLDOMDocument ** pDoc, IXMLDOMNode **pRoot);
	static IXMLDOMNode * AppendElementChild(IXMLDOMDocument * pDoc,IXMLDOMNode * pParent,LPCTSTR sNodeName);
	static IXMLDOMNode * AddNamedElementChild(IXMLDOMDocument * pDoc,IXMLDOMNode * pParent,LPCTSTR sNodeName,LPCSTR sNodeValue);
	static HRESULT		 CreateNewDocumentFromStream( IXMLDOMDocument **pDoc,IXMLDOMNode ** pRoot,IStream * pStm);
	static IXMLDOMNode * GetChildElementByName( IXMLDOMNode * pParent, LPCTSTR	childName);
};

#endif // !defined(AFX_TREESTATEMGR_H__D810C7D2_D603_11D5_AC1A_00A00C90889A__INCLUDED_)
