/*!
 * \file IFile.h
 * \date 4-18-2012 0:27:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IFILE_H__
#define __IFILE_H__

#include "../core/IObject.h"

class IFile : public IObject
{
public:
	typedef void* IFILE_HANDLE;

	enum OPEN_FILE_FLAG
	{
		OFF_READ	= 0x00000001,
		OFF_WRITE	= 0x00000002,
	};

public:
	RTTI_DEF(IFile, IObject);

	IFile(const tstring& strFileName, uint nFlag);
	virtual ~IFile();

	uint Read(void* pBuffOut, uint nSize);
	uint Write(const void* pBuffIn, uint nSize);
	uint Seek(uint nOffset);
	uint GetSize() const;
	uint Tell() const;

private:
	void Init();
	void Destroy();

	bool Open(const tstring& strFileName, uint nFlag);

private:
	IFILE_HANDLE m_hFile;
	uint m_nFlag;

};

#endif // __IFILE_H__
