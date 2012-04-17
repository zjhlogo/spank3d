/*!
 * \file IFile.cpp
 * \date 4-18-2012 0:28:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/IFile.h>
#include <util/FileUtil.h>

IFile::IFile(const tstring& strFileName, uint nFlag)
{
	Init();
	Open(strFileName, nFlag);
}

IFile::~IFile()
{
	Destroy();
}

void IFile::Init()
{
	m_nFlag = 0;
	m_hFile = NULL;
}

void IFile::Destroy()
{
	FileUtil::FileClose(m_hFile);
	m_hFile = NULL;
}

uint IFile::Read(void* pBuffOut, uint nSize)
{
	return FileUtil::FileRead(pBuffOut, nSize, m_hFile);
}

uint IFile::Write(const void* pBuffIn, uint nSize)
{
	if (m_nFlag & OFF_WRITE)
	{
		return FileUtil::FileWrite(pBuffIn, nSize, m_hFile);
	}

	return 0;
}

uint IFile::Seek(uint nOffset)
{
	FileUtil::FileSeek(m_hFile, nOffset, SEEK_SET);
	return FileUtil::FileTell(m_hFile);
}

uint IFile::GetSize() const
{
	uint nBackupPos = FileUtil::FileTell(m_hFile);
	FileUtil::FileSeek(m_hFile, 0, SEEK_END);
	uint nSize = FileUtil::FileTell(m_hFile);
	FileUtil::FileSeek(m_hFile, nBackupPos, SEEK_SET);

	return nSize;
}

uint IFile::Tell() const
{
	return FileUtil::FileTell(m_hFile);
}

bool IFile::Open(const tstring& strFileName, uint nFlag)
{
	m_nFlag = nFlag;

	if (OFF_READ == m_nFlag)
	{
		m_hFile = FileUtil::FileOpen(strFileName.c_str(), _("rb"));
	}
	else if (OFF_WRITE == m_nFlag)
	{
		m_hFile = FileUtil::FileOpen(strFileName.c_str(), _("wb"));
	}
	else if ((OFF_READ|OFF_WRITE) == m_nFlag)
	{
		m_hFile = FileUtil::FileOpen(strFileName.c_str(), _("rwb"));
	}

	if (!m_hFile) return false;

	SetOk(true);
	return true;
}
