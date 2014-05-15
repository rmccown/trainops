#if !defined(VERSTR_H)
#define VERSTR_H

// Translate the fileversion numbers found in _bldnum.h to be
// strings for use by the resource file using macros

#include "_bldnum.h"

#define DO_STR2( v ) #v
#define DO_STR( v ) DO_STR2( v )
#define FILEVERSION_STRING() DO_STR FILE_VERSION0 "," DO_STR FILE_VERSION1 "," DO_STR FILE_VERSION2 "," DO_STR BUILD_NUMBER

// g++ has a problem with the above
//#define FILEVERSION_STRING() DO_STR (FILE_VERSION0) "," DO_STR (FILE_VERSION1) "," DO_STR (FILE_VERSION2) "," DO_STR (BUILD_NUMBER) "\0"

#define APPNAME_STRING() DO_STR (APPNAME)

// Standard legalese strings
#define COPYRIGHT_STRING "Copyright © 2006-2007 Bob McCown dba Large Scale Central,\nAll Rights Reserved\0"
#define LICENSE_STRING "By using or possessing this program you agree to abide by the terms\n"\
                      "and conditions set forth in the License Agreement."
#define LICENSE_WEBPAGE_STRING "http://www.largescalecentral.com/TrainOps/license.php"

#endif
