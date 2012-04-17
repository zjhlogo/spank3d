/*!
 * \file FileUtil.cpp
 * \date 4-18-2012 0:34:42
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/FileUtil.h>
#include <util/LogUtil.h>
#include <tchar.h>

FileUtil::FileUtil()
{
	// TODO: 
}

FileUtil::~FileUtil()
{
	// TODO: 
}

FileUtil& FileUtil::GetInstance()
{
	static FileUtil s_FileUtil;
	return s_FileUtil;
}

IFile* FileUtil::OpenFile(const tstring& strFileName, uint nFlag /*= IFile::OFF_READ*/)
{
	IFile *pFile = new IFile(strFileName, nFlag);
	if (!pFile || !pFile->IsOk())
	{
		LOG(_("FileUtil::OpenFile Failed \"%s\""), strFileName.c_str());
		SAFE_RELEASE(pFile);
		return NULL;
	}

	return pFile;
}

IFile::IFILE_HANDLE FileUtil::FileOpen(const tstring& strFile, const tstring& strOption)
{
	FILE* pFile = NULL;
	_tfopen_s(&pFile, strFile.c_str(), strOption.c_str());
	return (IFile::IFILE_HANDLE)pFile;
}

void FileUtil::FileClose(IFile::IFILE_HANDLE hFile)
{
	if (hFile) fclose((FILE*)hFile);
}

void FileUtil::FileSeek(IFile::IFILE_HANDLE hFile, int nOffset, uint nOrigin)
{
	fseek((FILE*)hFile, nOffset, nOrigin);
}

uint FileUtil::FileTell(IFile::IFILE_HANDLE hFile)
{
	return ftell((FILE*)hFile);
}

uint FileUtil::FileRead(void* pDataOut, uint nSize, IFile::IFILE_HANDLE hFile)
{
	return fread(pDataOut, 1, nSize, (FILE*)hFile);
}

uint FileUtil::FileWrite(const void* pDataIn, uint nSize, IFile::IFILE_HANDLE hFile)
{
	return fwrite(pDataIn, 1, nSize, (FILE*)hFile);
}
