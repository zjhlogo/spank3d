/*!
 * \file DebugUtil.cpp
 * \date 10-11-2012 20:21:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/DebugUtil.h>
#include <windows.h>

void DebugUtil::DebugOut(const tstring& strOut)
{
	::OutputDebugString(strOut.c_str());
}
