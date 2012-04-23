/*!
 * \file FileUtil.h
 * \date 4-18-2012 0:34:36
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include "IFile.h"

class FileUtil
{
public:
	static IFile* LoadFile(const tstring& strFileName, uint nFlag = IFile::OFF_READ);
	static IFile* LoadFile(const tchar* pszFileName, uint nFlag = IFile::OFF_READ);

	static bool ReadFileIntoString(std::string& strOut, const tstring& strFileName);
	static bool ReadFileIntoString(std::string& strOut, const tchar* pszFileName);

	// for IFile used only
	static IFile::IFILE_HANDLE FileOpen(const tchar* pszFileName, const tchar* pszOption);
	static void FileClose(IFile::IFILE_HANDLE hFile);
	static void FileSeek(IFile::IFILE_HANDLE hFile, int nOffset, uint nOrigin);
	static uint FileTell(IFile::IFILE_HANDLE hFile);
	static uint FileRead(void* pDataOut, uint nSize, IFile::IFILE_HANDLE hFile);
	static uint FileWrite(const void* pDataIn, uint nSize, IFile::IFILE_HANDLE hFile);

};
#endif // __FILEUTIL_H__
