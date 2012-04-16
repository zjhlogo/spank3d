/*!
 * \file BaseType.h
 * \date 4-15-2012 12:23:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BASETYPE_H__
#define __BASETYPE_H__

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

#define NULL						0

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] (x); (x) = NULL;}
#define SAFE_RELEASE(x) if (x) {(x)->Release(); (x) = NULL;}

#endif // __BASETYPE_H__
