/*!
 * \file BitmapData_Impl.h
 * \date 7-21-2012 16:27:15
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BITMAPDATA_IMPL_H__
#define __BITMAPDATA_IMPL_H__

#include <core/IBitmapData.h>

class BitmapData_Impl : public IBitmapData
{
public:
	RTTI_DEF(BitmapData_Impl, IBitmapData);

	BitmapData_Impl(const tstring& id, uint width, uint height, uint bpp = 32);
	virtual ~BitmapData_Impl();

	virtual const tstring& GetId() const;

	virtual uint GetWidth() const;
	virtual uint GetHeight() const;
	virtual uint GetBPP() const;

	virtual void* GetData();
	virtual const void* GetData() const;

private:
	bool CreateData(uint width, uint height, uint bpp);
	void FreeData();

private:
	tstring m_strId;
	uint m_nWidth;
	uint m_nHeight;
	uint m_nBpp;
	uchar* m_pData;

};
#endif // __BITMAPDATA_IMPL_H__
