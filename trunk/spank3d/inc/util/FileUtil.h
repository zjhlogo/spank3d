/*!
 * \file FileUtil.h
 * \date 4-18-2012 0:34:36
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include <util/IFile.h>

class FileUtil
{
public:
	FileUtil();
	virtual ~FileUtil();

	static FileUtil& GetInstance();

	IFile* OpenFile(const tstring& strFileName, uint nFlag = IFile::OFF_READ);

	static IFile::IFILE_HANDLE FileOpen(const tstring& strFile, const tstring& strOption);
	static void FileClose(IFile::IFILE_HANDLE hFile);
	static void FileSeek(IFile::IFILE_HANDLE hFile, int nOffset, uint nOrigin);
	static uint FileTell(IFile::IFILE_HANDLE hFile);
	static uint FileRead(void* pDataOut, uint nSize, IFile::IFILE_HANDLE hFile);
	static uint FileWrite(const void* pDataIn, uint nSize, IFile::IFILE_HANDLE hFile);

};
#endif // __FILEUTIL_H__
