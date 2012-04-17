/*!
 * \file BaseType.h
 * \date 4-15-2012 12:23:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#include <string>
#include <cassert>

//typedef bool						bool;

//typedef char						char;
typedef unsigned char				uchar;

//typedef short						short;
typedef unsigned short				ushort;

//typedef int						int;
typedef unsigned int				uint;

typedef __int64						int64;
typedef unsigned __int64			uint64;

//typedef float						float;

#ifdef _UNICODE
	#define _(x) L ## x
	typedef wchar_t tchar;
#else
	#define _(x) x
	typedef char tchar;
#endif // _UNICODE

typedef std::basic_string<tchar>	tstring;

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] (x); (x) = NULL;}
#define SAFE_RELEASE(x) if (x) {(x)->Release(); (x) = NULL;}

#endif // __BASETYPE_H__
