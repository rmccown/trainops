#ifndef	__PARSEIT_H_
#define	__PARSEIT_H_

//#include "stdafx.h"
                   
#ifdef _DEBUG
#define	new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////	
//	class definition for the ParseField class
///////////////////////////////////////////////////////////////////////////////////
//	this class is just a CString with some assignment and cast operators
//	it is used to store a parsed field value and allow the data to be accessed
//	thru casting the result to the wanted datatype
//	************************** ALLOWED OPERATIONS *********************************
//	CParseField p;
//	p="1234.12" 		// assignment to LPCSTR
//	CStrinf e=(LPCSTR)p;//	operator (LPCSTR)
//	double d=(double)p;	//	d=1234.12 	operator double 
//	int n=(int)p;		//	n=1234 	operator int
//////////////////////////////////////////////////////////////////////////////////
class CParseField{
public:
	CParseField() {};
	~CParseField() {};

	operator int() 		{ return atoi(TheData);};
	operator long()		{ return atol(TheData);};
	operator double()	{ return atof(TheData);};
	operator LPCSTR()	{ return (LPCSTR) TheData;}; const
	CParseField& operator =(LPCSTR s) {TheData=s;return *this;};
	CParseField&  operator =(CString& s) {TheData=s;return *this;};
	CParseField&  operator =(CParseField& s) {TheData=s.TheData;return *this;};
	
// Implementation
protected:
	CString	TheData;
};
///////////////////////////////////////////////////////////////////////////////////
//	END OF CparseField DEFINITION AND CODE ( ALL FUNCTIONS HERE)
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////	
//	class definition for the Parsing class
///////////////////////////////////////////////////////////////////////////////////
class CParseIt
{
public:
	CParseIt(BOOL Strip=FALSE);
	CParseIt(LPCSTR Data,LPCSTR Sep=",",BOOL Strip=FALSE);
	~CParseIt();

	//	use default constructoe then call this function to open and use a file contaimning
	//	the data    
    BOOL	ParseFile(LPCSTR lpzFileName,LPCSTR Sep=",");
// Attributes
public:
// Operations             
protected:           
	BOOL 		IsSeperator(char s);
	BOOL 		LoadFile(LPCSTR lpzFileName);
public:         
	//	returns the one based number of fields if 0 there are no fields
	int			GetNumFields() {return NumFields;};
	//	returns the list of seperators
	LPCSTR		GetSeperator() {return Seperator;};
	//	sets the seperator list IE "|,*"
	void		SetSeperator(LPCSTR Sep) {strncpy(Seperator,Sep,9);};
	//	parse a object constructed bt the second constructor (data,sep);
	BOOL		Parse();
	//	parse a object created by the default constructor will call Parse();
	BOOL		Parse(LPCSTR Data,LPCSTR Sep=",",BOOL Strip=FALSE);
	//	returns the 1 based field in a cstring or a string
	BOOL	 	GetField(int nFNum,CString& Buff);
	BOOL	 	GetField(int nFNum,LPSTR Buff);
	//	will return the cparsefield object. This can be cast to the desired type
	//	by (int) (long) (double) (lpcstr)
	CParseField	GetField(int n);
	
// Implementation
public:
	void ReSet();

protected:
	BOOL		StripQuotes;
	char		Seperator[10];
	int			NumFields;
	CPtrArray	TheFields;	
	LPSTR		TheData;
	
};




#endif
