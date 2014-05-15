// TBTreeStateMgr.cpp: implementation of the TBTreeStateMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TBTreeStateMgr.h"
#include <atlbase.h>
#include <atlconv.h>
#include <comdef.h>

////////////////////////////////////////
// Initialize all static variables
////////////////////////////////////////
TCHAR			* TBTreeStateMgr::m_StgFileName=_T("CtlStateStg.ctss");
CTreeCtrl		* TBTreeStateMgr::pCtrl=NULL;
IStream			* TBTreeStateMgr::pCurrStream=NULL;
IStorage		* TBTreeStateMgr::pCurrStorage=NULL;
IXMLDOMDocument * TBTreeStateMgr::pDoc=NULL;
IXMLDOMNode		* TBTreeStateMgr::pExpNodes=NULL;
bool			  TBTreeStateMgr::fInitialized=false;
bool			  TBTreeStateMgr::fUsingCustomStorage=false;
CRITICAL_SECTION  TBTreeStateMgr::csection;
TBTreeStateMgr::BSTRVECT_T	*	 TBTreeStateMgr::v_bsStates;


/*
 *	SaveTreeState	User calls this function
 *
 *	To prevent multiple UI Threads, this function acquired a CS, then delegates to _SaveTreeState
 *	Returns :	TRUE if successful, FALSE otherwise
 */
BOOL TBTreeStateMgr::SaveTreeState (LPCTSTR lpszTreeName, CTreeCtrl  * ctrl)
{
	BOOL	bRet;

	//Sanity
	if(!fInitialized) {
		TRACE("TBTreeStateMgr: Initialize function not called, use Initialize()\n");
		return FALSE;
	}
	__try {
		::EnterCriticalSection(&csection);

		bRet=_SaveTreeState(lpszTreeName,ctrl);

	} __finally {
		::LeaveCriticalSection(&csection);
	}

	return bRet;

}
/*
 *	LoadTreeState User calls this function
 *
 *	To prevent multiple UI Threads, this function acquired a CS, then delegates to _LoadTreeState
 *	Returns :	TRUE if successful, FALSE otherwise
 */
BOOL TBTreeStateMgr::LoadTreeState (LPCTSTR lpszTreeName, CTreeCtrl  * ctrl)
{
	BOOL	bRet;
	__try {
		::EnterCriticalSection(&csection);

		bRet=_LoadTreeState(lpszTreeName,ctrl);

	} __finally {
		::LeaveCriticalSection(&csection);
	}

	return bRet;

}
/*
 *	Initialize()	This needs to be called before the manager can be used
 */
void TBTreeStateMgr::Initialize()
{
	if(!fInitialized)
	{
		fInitialized=true;
		InitializeCriticalSection(&csection);
	}
}
/*
 *	Uninitialize()	This needs to be called to prevent CSECTION leaks
 */
void TBTreeStateMgr::Uninitialize()
{
	if(fInitialized)
	{
		fInitialized=false;
		DeleteCriticalSection(&csection);
		if (fUsingCustomStorage) delete [] m_StgFileName;
	}
}
/*
 *	SetStorageFile()	If the user doesnt like the default storage name
 */
void TBTreeStateMgr::SetStorageFile(LPCTSTR lpszStorageName)
{
	if (fUsingCustomStorage) {
		delete [] m_StgFileName;
	}
	m_StgFileName=_tcsdup(lpszStorageName);
	fUsingCustomStorage=true;
}
/*
 *	MakeValidName		Scrubs the input name into a valid IStream name
 *
 *  The main reason for having this function is so that folks can specify 
 *	filenames as input to SaveState()
 *
 *	Returns :	A valid name that can be used for a IStream
 *	Memory  :	The returned context name must be freed via a delete[]
 */
TCHAR * TBTreeStateMgr::MakeValidName( LPCTSTR lpszContext)
{
	//Clean the context name
	TCHAR * cleanContext = new TCHAR[32];
	TCHAR * p = cleanContext;
	int len = _tcslen(lpszContext);

	//Sanity
	if(cleanContext==NULL) {
		TRACE("TBTreeStateMgr: Out of memory\n");
		return NULL;
	}

	//Stream names cant be greater than 32 chars
	if ( len > 31 ) {
		strncpy(cleanContext,lpszContext + len - 31, 31);
		cleanContext[31]=0;
	} else {
		strcpy(cleanContext,lpszContext);
	}

	//Scrub invalid characters
	while ( *p ) {
		if ( *p == _T('\\') || *p == _T('/') || *p == _T(':') || *p == _T('!')  || *p == _T('.'))
			 *p = _T('_');
		p++;
	}

	return cleanContext;
}
/*
 *	_SaveTreeState	User calls this function to save the state of a tree 
 *
 *	Each tree control that is stored has a user-specified name or ID, when the user wants to 
 *	restore the state of the tree, the same ID is used.
 *
 *	Returns :	TRUE if successful, FALSE otherwise
 */
BOOL TBTreeStateMgr::_SaveTreeState (LPCTSTR lpszTreeName, CTreeCtrl  * ctrl)
{	
	HRESULT		  hr;
	IXMLDOMNode * pTemp, *pRoot;

	// Clean the context name
	TCHAR * cleanContext = MakeValidName(lpszTreeName);

	// Open the storage (create if it doesnt exist)
	if ( FAILED(OpenStorageFile(m_StgFileName, true)) ){
		TRACE("TBTreeStateMgr: Cant Save Tree State\n");
		return FALSE;
	}

	// Recreate the stream (cuz we gonna rewrite the entire XML Doc)
	pCurrStream = RecreateStream(cleanContext);
	if ( pCurrStream == NULL) {
		pCurrStorage->Release();
		TRACE("Cant open stream %s\n", lpszTreeName);
		return FALSE;
	}

	// Get ready for the save operation
	pCtrl = ctrl;
	ASSERT ( pCtrl != NULL);

	// Create the XML Document and save the tree state
	try {

		// Create a new document
		CreateNewDocument(&pDoc,&pRoot);

		// Create the doc schema
		pTemp = AppendElementChild( pDoc, pRoot, _T("TreeState"));
		pTemp = AppendElementChild( pDoc, pTemp, _T("ExpandedNodes"));
		pExpNodes = pTemp;
		
		ULONG oldTickCount=GetTickCount();
		
		// Call the save function with root
		HTREEITEM hItem = pCtrl->GetRootItem();
		DeepSaveItemState(hItem);

		TRACE("Saved Item state in %u (msecs)\n",GetTickCount()-oldTickCount);
		// Write the document to stream
		_variant_t vstm( pCurrStream);
		hr=pDoc->save(vstm);
		if(FAILED(hr))_com_issue_error(hr);

	} catch (const _com_error& err) {
		TRACE("TBTreeStateMgr: Major COM Error [0x%x]\n",err.Error());
		return FALSE;
	}

	// Release the document
	pDoc->Release();pDoc = NULL;

	// Close the stream
	pCurrStream->Release();	pCurrStream = NULL;
	pCurrStorage->Release();pCurrStorage=NULL;

	// celan
	delete [] cleanContext;

	return TRUE;
}
/*
 *	DeepSaveItemState		Recursively save the state of a tree item
 *	
 *	Reverse of DeepSetItemState
 *
 *	Exceptions:	May throw a COM Exception
 */
void TBTreeStateMgr::DeepSaveItemState (HTREEITEM hItem)
{

	   HTREEITEM hChildItem;
	   HTREEITEM hNextItem = hItem;

	   while (hNextItem!= NULL)
	   {
			// If not expanded skip this 
			if ( (pCtrl->GetItemState(hNextItem, TVIS_EXPANDED) & TVIS_EXPANDED) )
			{
				//Expanded.. so save this item text
				CString cstr = pCtrl->GetItemText(hNextItem);
				AddNamedElementChild(pDoc,pExpNodes,"node",cstr);
				TRACE("Saved expanded item [%s]\n", cstr);
				
				//Step down one level and process children
				hChildItem=pCtrl->GetChildItem(hNextItem);
				if(hChildItem!=NULL) DeepSaveItemState(hChildItem);
			}

			//Next sibling item
			hNextItem = pCtrl->GetNextItem(hNextItem,TVGN_NEXT);
	   }

}
/*
 *	DeepSetItemState	Recursively Set the state of the tree
 *	
 *	Converse of DeepSaveItemState
 *	
 *	Exceptions :	May throw a COM Exception 
 */
void TBTreeStateMgr::DeepSetItemState (HTREEITEM hItem)
{

	   HTREEITEM hChildItem;
	   HTREEITEM hNextItem = hItem;

	   while (hNextItem!= NULL)
	   {
			// If item is in expanded list, then expand please
			CString cstr = pCtrl->GetItemText(hNextItem);
			if ( IsExpandedState( cstr) ) {
				pCtrl->Expand(hNextItem,TVE_EXPAND);
				TRACE("expanded item [%s]\n", cstr);

				//Step down one level and process children
				hChildItem=pCtrl->GetChildItem(hNextItem);
				DeepSetItemState(hChildItem);
			}

			//Next sibling item
			hNextItem = pCtrl->GetNextItem(hNextItem,TVGN_NEXT);
	   }
}
/*
 *	OpenStorageFile		Opens/Create a Compound Document
 *
 *	Returns :	Can be checked via FAILED() macro
 */
HRESULT TBTreeStateMgr::OpenStorageFile(LPCTSTR lpszFn, bool fCreate)
{
	USES_CONVERSION;
	HRESULT hr;

	// Open the storage
	hr =::StgOpenStorage(T2COLE(lpszFn),NULL,                       
							STGM_READWRITE | STGM_SHARE_EXCLUSIVE ,            
							NULL, 0, &pCurrStorage);
	if (FAILED(hr))
	{
		if( (hr == STG_E_FILENOTFOUND) && fCreate) {
			hr = ::StgCreateDocfile(T2COLE(lpszFn), 
									STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
									0, &pCurrStorage);
			if ( FAILED(hr) )
			{
				TRACE( "Error in StgCreateDocFile");
				return E_FAIL;
			}
		} else {
			TRACE("Problem with IStorage\n");
			return hr;
		}
	}

	return hr;
}
/*
 *	RecreateStream	Need to recreate or create an IStream based on whether or not 
 *					we are trying to re-save a tree or saving a new tree.
 *
 *	Returns :	NULL if failed, IStream if succeeded
 */
IStream * TBTreeStateMgr::RecreateStream(LPCTSTR lpszName)
{
	USES_CONVERSION;
	IStream * pStm;
	if ( pCurrStorage->CreateStream(A2COLE(lpszName),
							STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
							0, 0, &pStm) != S_OK )	{
		TRACE( "Error in context stream creation");
		return NULL;
	}
	return pStm;
}

/*
 *	OpenStateStream		Open a specified stream by its name
 *
 *	Note: This function will create a new stream if one doesnt already exist
 *
 *	Returns :	NULL if failed, valid IStream otherwise
 */
IStream * TBTreeStateMgr::OpenStateStream(LPCTSTR lpszName, bool fCreate)
{
	USES_CONVERSION;
	HRESULT hr;

	IStream * pStm;

	hr = pCurrStorage->OpenStream(T2COLE(lpszName),NULL,
							STGM_READWRITE | STGM_SHARE_EXCLUSIVE ,
							0, &pStm);
	if ( FAILED(hr))	
	{
		if( (hr == STG_E_FILENOTFOUND) && fCreate) {
			// Create new stream with name (in buf)
			if ( pCurrStorage->CreateStream(A2COLE(lpszName),
									STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
									0, 0, &pStm) != S_OK )	{
				TRACE( "Error in context stream creation");
				return NULL;
			}
		} else {
			TRACE("Problem with IStorage\n");
			return NULL;
		}	
	} // failed

	return pStm;
}

/*
 *	LoadTreeState		User calls this function to restore the state of a tree
 *
 *	Reverse of SaveTreeState
 *
 *	Returns :	TRUE if successful, FALSE otherwise
 */
BOOL TBTreeStateMgr::_LoadTreeState (LPCTSTR lpszContext, CTreeCtrl  * ctrl)
{
	IXMLDOMNode * pTemp, *pRoot;

	// Open the storage (create if it doesnt exist)
	if ( FAILED(OpenStorageFile(m_StgFileName, false)) ){
		TRACE("Cant Load Tree State (storage problem)\n");
		return FALSE;
	}

	// Clean the context name
	TCHAR * cleanContext = MakeValidName(lpszContext);


	// Open the stream (create if it dont exist)
	pCurrStream = OpenStateStream(cleanContext, false);
	if ( pCurrStream == NULL) {
		TRACE("Cant open stream %s\n", lpszContext);
		pCurrStorage->Release(); pCurrStorage=NULL;
		return FALSE;
	}

	// Get ready for the load operation
	pCtrl = ctrl;
	ASSERT ( pCtrl != NULL);

	// Create a new document
	CreateNewDocumentFromStream(&pDoc,&pRoot, pCurrStream);

	if ( !pRoot ) {
		ATLTRACE("Ill formed XML\n");
		return S_FALSE;
	}
	// Navigate schema
	pTemp = GetChildElementByName(pRoot, _T("TreeState"));
	if ( ! pTemp ) {
		pCurrStream->Release(); 
		pCurrStorage->Release();
		return S_FALSE;
	}
	pTemp = GetChildElementByName( pTemp, _T("ExpandedNodes"));
	pExpNodes = pTemp;
	
	// Load expanded child names into a vector
	v_bsStates = new BSTRVECT_T;
	LoadStateVector();

	// Call the save function with root
	HTREEITEM hItem = pCtrl->GetRootItem();
	DeepSetItemState(hItem);

	// Delete the vector
	for ( BSTRVECT_T:: const_iterator _iter = v_bsStates->begin();
		 _iter != v_bsStates->end();
		 _iter ++ ) 
	{
			 ::SysFreeString(*_iter);
	}

	delete v_bsStates;

	// Release the document
	pDoc->Release();pDoc = NULL;

	// Close the stream
	pCurrStream->Release();	pCurrStream = NULL;
	pCurrStorage->Release();pCurrStorage=NULL;

	// clean
	delete []  cleanContext;

	return TRUE;
}

/*
 *	LoadStateVector		Loads the expanded nodes XML Child text here
 *
 *	For performance reasons we load all the children first instead of one by one.
 *
 *	Returns :	Check via FAILED() macro
 */
HRESULT TBTreeStateMgr::LoadStateVector()
{
	ASSERT ( pExpNodes != NULL);

	CComQIPtr<IXMLDOMElement>  spEle((IXMLDOMNode*) pExpNodes);
	IXMLDOMNode * pChild;
	IXMLDOMNodeList	* pdlist;
	long  value;
	BSTR  bs;

	// Debug
	TRACE("\n+-- Creating state vector +\n");
	// Get the tag names
	if (FAILED(spEle->getElementsByTagName(_bstr_t("node"), &pdlist)))
	{
		TRACE("Empty Collection");
		return S_OK;
	}

	// Cycle thru elements and locate name attribute
	pdlist->get_length(&value);
	pdlist->reset();
	for(int ii = 0; ii < value; ii++)
	{
		pdlist->get_item(ii, &pChild);
		if ( pChild) {
			pChild->get_text(&bs);
			v_bsStates->push_back(bs);
			TRACE("Added node %S\n",bs);
		} 
	}

	return S_OK;
}

/*
 *	IsExpandedState		Helper function
 *
 *	Returns :	True if a given node is expanded, False otherwise
 */
bool TBTreeStateMgr::IsExpandedState( LPCTSTR nodename)
{
	BSTR bs2;

	for ( BSTRVECT_T:: const_iterator _iter = v_bsStates->begin();
		 _iter != v_bsStates->end();
		 _iter ++ ) 
		 {
			bs2  = *_iter;
			if ( _bstr_t(nodename) == _bstr_t(bs2) ) {
				return true;
			}
	}
	return false;
}


/**
 *	CreateNewDocument	XML Helper
 *	Throws Exception
 */
HRESULT TBTreeStateMgr::CreateNewDocument(IXMLDOMDocument ** pDoc, IXMLDOMNode **pRoot)
{
		//Create a new document CoClass
		HRESULT hr =  CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL, 
									   IID_IXMLDOMDocument, (void**)pDoc);
		if ( FAILED(hr) )_com_issue_error(hr);
		
		return (*pDoc)->QueryInterface(IID_IXMLDOMNode, (void **)pRoot);
}

/**
 *	AppendElementChild	XML Helper
 *	Throws Exception
 */
IXMLDOMNode * TBTreeStateMgr::AppendElementChild( IXMLDOMDocument * pDoc, 
												  IXMLDOMNode     * pParent,
												  LPCTSTR		    sNodeName)
{
	IXMLDOMElement	*pElem;
	IXMLDOMNode		*pRet;
	HRESULT			 hr;

	// Create new element node
	hr=pDoc->createElement(_bstr_t(sNodeName), &pElem);
	if(FAILED(hr))_com_issue_error(hr);

	// Add to parent and return
	hr=pParent->appendChild(pElem, &pRet);
	if(FAILED(hr))_com_issue_error(hr);

	return pRet;

}
/**
 *	AddNamedElementChild	XML Helper
 *	Throws Exception
 */
IXMLDOMNode * TBTreeStateMgr::AddNamedElementChild( IXMLDOMDocument * pDoc, 
											  IXMLDOMNode     * pParent,
											  LPCTSTR		    sNodeName,
											  LPCSTR		    sNodeValue)
{
	IXMLDOMElement	*pElem;
	IXMLDOMNode		*pRet;
	HRESULT			 hr;

	// Create new element node
	hr=pDoc->createElement(_bstr_t(sNodeName), &pElem);
	if(FAILED(hr))_com_issue_error(hr);

	// Create a text node with the sNodeValue
	hr=pElem->put_text(_bstr_t(sNodeValue));
	if(FAILED(hr))_com_issue_error(hr);

	// Add to parent and return
	hr=pParent->appendChild(pElem, &pRet);
	if(FAILED(hr))_com_issue_error(hr);

	return pRet;
}
/**
 *	CreateNewDocumentFromStream	XML Helper
 *	Returns:	Check using FAILED() macro
 */
HRESULT TBTreeStateMgr::CreateNewDocumentFromStream( IXMLDOMDocument **pDoc,
											   IXMLDOMNode ** pRoot,
											   IStream * pStm)
{
	// Create the document and root
	HRESULT hr = CreateNewDocument(pDoc,pRoot);
	if (FAILED(hr)) return hr;

	// Load the document from stream
	hr = (*pDoc)->put_async(VARIANT_FALSE);
	VARIANT_BOOL fSuccess;
	_variant_t vstm(pStm);

	hr = (*pDoc)->load(vstm, &fSuccess);
	if ( FAILED(hr)){
		return hr;
	} else if(fSuccess != VARIANT_TRUE) {
		return E_FAIL;
	}
	return S_OK;
}

/**
 *	GetChildElementByName	XML Helper
 *	Throws exception
 */
IXMLDOMNode * TBTreeStateMgr::GetChildElementByName( IXMLDOMNode * pParent, 
													 LPCTSTR	   childName)
{
	HRESULT		  hr;
	DOMNodeType	  nodeType;
	IXMLDOMNode * pChild = NULL;
	IXMLDOMNode * pNext  = NULL;
	BSTR		  bstrNodeName;
	_bstr_t bstr_tModuleInfo = _bstr_t(childName);

	ASSERT(pParent != NULL);

	hr=pParent->get_firstChild(&pChild);
	if(FAILED(hr))_com_issue_error(hr);
	
	//Search all the children
	while ( pChild) {

		hr=pChild->get_nodeType(&nodeType);
		if(FAILED(hr))_com_issue_error(hr);

		TRACE("Got node type = %d\n", nodeType);
		if ( nodeType == NODE_ELEMENT) {	
			if ( SUCCEEDED(pChild->get_nodeName(& bstrNodeName)) ) {
				if ( bstr_tModuleInfo == _bstr_t(bstrNodeName) )
				{
					::SysFreeString(bstrNodeName);
					break;
				} 
				::SysFreeString(bstrNodeName);
			}

		}

		hr=pChild->get_nextSibling(&pNext);
		if(FAILED(hr))_com_issue_error(hr);

		if(pChild)pChild->Release();

		pChild = pNext;
	}

	return pChild;
}