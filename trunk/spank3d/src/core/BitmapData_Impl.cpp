/*!
 * \file BitmapData_Impl.cpp
 * \date 7-21-2012 16:30:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BitmapData_Impl.h"

BitmapData_Impl::BitmapData_Impl(uint width, uint height, uint bpp /*= 32*/)
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	m_pData = NULL;

	SetOk(CreateData(width, height, bpp));
}

BitmapData_Impl::~BitmapData_Impl()
{
	FreeData();
}

uint BitmapData_Impl::GetWidth() const
{
	return m_nWidth;
}

uint BitmapData_Impl::GetHeight() const
{
	return m_nHeight;
}

uint BitmapData_Impl::GetBPP() const
{
	return m_nBpp;
}

void* BitmapData_Impl::GetData()
{
	return m_pData;
}

const void* BitmapData_Impl::GetData() const
{
	return m_pData;
}

bool BitmapData_Impl::CreateData(uint width, uint height, uint bpp)
{
	FreeData();

	m_nWidth = width;
	m_nHeight = height;
	m_nBpp = bpp;

	uint bytesPerPixel = m_nBpp / 8;
	m_pData = new uchar[m_nWidth*m_nHeight*bytesPerPixel];

	return true;
}

void BitmapData_Impl::FreeData()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	SAFE_DELETE_ARRAY(m_pData);
}
