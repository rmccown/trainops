#include "stdafx.h"
#include <stdarg.h>
#include "parseit.h"
                   
#ifdef _DEBUG
#define	new DEBUG_NEW
#endif



///////////////////////////////////////////////////////////////////////////
//	Desc:	default constructor just set some default values
//	params:
//	returns:
///////////////////////////////////////////////////////////////////////////
CParseIt::CParseIt(BOOL Strip)
{
	strcpy(Seperator,",");
	StripQuotes=Strip;
	NumFields=0;
	TheData=NULL;
}

///////////////////////////////////////////////////////////////////////////
//	Desc:	construct object to parse data using sep for the list of seperators
//	params:
//	returns:
///////////////////////////////////////////////////////////////////////////
CParseIt::CParseIt(LPCSTR Data,LPCSTR Sep,BOOL Strip)
{
	NumFields=0;
	StripQuotes=Strip;
	TheData=new char[strlen(Data)+1];
	strcpy(TheData,Data);
	strncpy(Seperator,Sep,9);
	Parse();
	
}
///////////////////////////////////////////////////////////////////////////
//	Desc:	destructor: free all allocated memory variables and free up the
//			data stored in the pointer array
//	params:
//	returns:
///////////////////////////////////////////////////////////////////////////
CParseIt::~CParseIt()
{   
	CParseField* pData;
	if(TheData!=NULL)
		delete[] TheData;
	for(int y=0;y < TheFields.GetSize();++y)
	{
		pData=(CParseField*)TheFields[y];
		delete pData;
	}						
}

void CParseIt::ReSet()
{
	CParseField* pData;
	if(TheData!=NULL)
	{
		delete[] TheData;
		TheData=NULL;
	}
	for(int y=0;y < TheFields.GetSize();++y)
	{
		pData=(CParseField*)TheFields[y];
		delete pData;
	}						
	TheFields.RemoveAll();

}

///////////////////////////////////////////////////////////////////////////
//	Desc:	parse the string data using sep as a seperator list
//	params: data=string to parse sep= list of seperators
//	returns:TRUE if all is well else FALSE if error occours
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::Parse(LPCSTR Data,LPCSTR Sep,BOOL Strip)
{
	if(TheData!=NULL)
		return FALSE;
	StripQuotes=Strip;
	strncpy(Seperator,Sep,9);
	TheData=new char[strlen(Data)+1];
	strcpy(TheData,Data);
	return Parse();
}
///////////////////////////////////////////////////////////////////////////
//	Desc:	Called to do actual parsing of data
//	params:	none
//	returns:TRUE if OK else FALSE
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::Parse()
{
	if(TheData==NULL)
		return FALSE;                      
	
	for(int p=0;p < TheFields.GetSize();++p)
	{
		CParseField* pData=(CParseField*)TheFields[p];
		delete pData;
	}						
	TheFields.RemoveAll();
		
	NumFields=0;
	CString t;		
	t.Empty();        
	for(int y=0;y < (int)strlen(TheData);++y)
	{                       
		if( !IsSeperator(TheData[y]))
		{
			if(StripQuotes)
			{
				if(TheData[y] !='\"')
					t+=TheData[y];
			}
			else
				t+=TheData[y];
		}
		else
		{
			CParseField* pData=new CParseField;     
			*pData=t;
			TheFields.Add(pData);
			t.Empty();
			++NumFields;
		}   
	}
	CParseField* pData=new CParseField;
	*pData=t;
	TheFields.Add(pData);
	++NumFields;
	return TRUE;		
}
///////////////////////////////////////////////////////////////////////////
//	Desc: 	determines if the given cahracter is in the list of seperators
//	params:	The character in question
//	returns:TRUE if the cahr is a seperator else FALSE
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::IsSeperator(char s)
{
	for(int x=0;x < (int) strlen(Seperator);++x)
	{
		if(s==Seperator[x])
			return TRUE;
	}
	return FALSE;
}					
///////////////////////////////////////////////////////////////////////////
//	Desc:	returns field n in the cstring buff (1 based not zero based
//	params: The field number (1..nun fields) and a CString reference
//	returns:TRUE if OK else FALSE (out of bounds index)
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::GetField(int N,CString& Buff)
{   
	if(N <= TheFields.GetSize() && N > 0)
	{      
		Buff=(LPCSTR)*(CParseField*)TheFields[--N];
		return TRUE;
	}
	Buff.Empty();
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////
//	Desc:	returns field n in the string buff (1 based not zero based
//	params: The field number (1..nun fields) and a pointer to a string
//	returns:TRUE if OK else FALSE (out of bounds index)
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::GetField(int N,LPSTR Buff)
{   
	if(N <= TheFields.GetSize() && N > 0)
	{      
		strcpy(Buff,(LPCSTR)*(CParseField*)TheFields[--N]);
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////
//	Desc:	returns the CParseField object for the indicated field ( 1 based)
//	params:	the 1 based index of the field
//	returns:A CparseField object ( this can be cast to a string,long,double or int)
///////////////////////////////////////////////////////////////////////////
CParseField  CParseIt::GetField(int N)
{                                        
	CParseField p;
	if(N <= TheFields.GetSize() && N > 0)
	{      
		--N;
		return *(CParseField*)TheFields[N];
	}
	return p;
}

///////////////////////////////////////////////////////////////////////////
//	Desc:	Protected function to load data from a disk file
//	params:	The filename
//	returns:TRUE if all OK else FALSE
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::LoadFile(LPCSTR lpzFileName)
{
	CFileException e;
	int	res;               

	if(TheData != NULL)
		return FALSE;	
	CFile* TheFile=new CFile;		
	char* buffer =new char[2048];

	TRY
	{
		res=TheFile->Open(lpzFileName,CFile::modeReadWrite,&e);
	}
	CATCH( CFileException, e )
	{                       
		#ifdef _DEBUG
			afxDump << "File " << lpzFileName << " does not exist or cannot be opened\n";
		#endif
	}
	END_CATCH   
		
	if(e.m_cause==CFileException::none)
	{                                                                  
		int numRead=TheFile->Read((void *)buffer,2048);
		TheData=new char[numRead+1];
		memset((void*)TheData,'\0',numRead+1);
		strncpy(TheData,buffer,numRead);
		delete[] buffer;		
		delete TheFile;
		return numRead > 0 ? TRUE:FALSE;
	}
	delete[] buffer;		
	delete TheFile;
	return FALSE;		
}


///////////////////////////////////////////////////////////////////////////
//	Desc:	Parse a file loaded from disk.
//	params:	The filename and a list of seperators (default is ,)
//	returns:TRUE if all OK else FALSE
///////////////////////////////////////////////////////////////////////////
BOOL CParseIt::ParseFile(LPCSTR lpzFileName,LPCSTR Sep)
{
	strncpy(Seperator,Sep,9);
	if(LoadFile(lpzFileName))
		return Parse();
	return FALSE;	
}
				


