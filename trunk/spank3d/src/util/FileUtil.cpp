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

IFile* FileUtil::LoadFile(const tstring& strFileName, uint nFlag /*= IFile::OFF_READ*/)
{
	return LoadFile(strFileName.c_str(), nFlag);
}

IFile* FileUtil::LoadFile(const tchar* pszFileName, uint nFlag /*= IFile::OFF_READ*/)
{
	IFile *pFile = new IFile(pszFileName, nFlag);
	if (!pFile || !pFile->IsOk())
	{
		LOG(_("FileUtil::OpenFile Failed \"%s\""), pszFileName);
		SAFE_RELEASE(pFile);
		return NULL;
	}

	return pFile;
}

bool FileUtil::ReadFileIntoString(std::string& strOut, const tstring& strFileName)
{
	return ReadFileIntoString(strOut, strFileName.c_str());
}

bool FileUtil::ReadFileIntoString(std::string& strOut, const tchar* pszFileName)
{
	IFile* pFile = FileUtil::LoadFile(pszFileName);
	if (!pFile) return false;

	uint nFileSize = pFile->GetSize();
	if (nFileSize <= 0)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	char* pszFileData = new char[nFileSize+1];
	if (pFile->Read(pszFileData, nFileSize) != nFileSize)
	{
		SAFE_RELEASE(pFile);
		return false;
	}
	pszFileData[nFileSize] = '\0';

	strOut.clear();
	strOut.append(pszFileData);
	SAFE_DELETE_ARRAY(pszFileData);
	return true;
}

IFile::IFILE_HANDLE FileUtil::FileOpen(const tchar* pszFileName, const tchar* pszOption)
{
	FILE* pFile = NULL;
	_tfopen_s(&pFile, pszFileName, pszOption);
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
