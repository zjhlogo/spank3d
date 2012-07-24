/*!
 * \file ResMgr_Impl.cpp
 * \date 4-19-2012 22:56:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ResMgr_Impl.h"
#include <Spank3D.h>
#include <util/LogUtil.h>
#include <util/StringUtil.h>
#include <util/FileUtil.h>
#include <libpng1.5.12/png.h>

#include "BitmapData_Impl.h"
#include "../render/Texture_Impl.h"

ResMgr_Impl::ResMgr_Impl()
{
	g_pResMgr = this;
}

ResMgr_Impl::~ResMgr_Impl()
{
	g_pResMgr = NULL;
}

ResMgr_Impl& ResMgr_Impl::GetInstance()
{
	static ResMgr_Impl s_ResMgr_Impl;
	return s_ResMgr_Impl;
}

bool ResMgr_Impl::Initialize()
{
	SetDefaultDir(_("data"));
	return true;
}

void ResMgr_Impl::Terminate()
{
	// TODO: check m_MeshMap/m_BonesMap whether is empty, and logout
}

void ResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& ResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

IMesh* ResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;

	// check whether the mesh created
	TM_MESH::iterator itfound = m_MeshMap.find(strFilePath);
	if (itfound != m_MeshMap.end())
	{
		IMesh* pMesh = itfound->second;
		pMesh->IncReference();
		return pMesh;
	}

	// create new
	Mesh_Impl* pMesh = new Mesh_Impl(strFilePath);
	if (!pMesh || !pMesh->IsOk())
	{
		LOG(_("IResMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFilePath, pMesh));
	return pMesh;
}

IBitmapData* ResMgr_Impl::CreateBitmapData(uint width, uint height, uint bpp /*= 32*/)
{
	BitmapData_Impl* pBitmapData = new BitmapData_Impl(width, height, bpp);
	if (!pBitmapData || !pBitmapData->IsOk())
	{
		SAFE_DELETE(pBitmapData);
		return NULL;
	}

	return pBitmapData;
}

static void PngReaderCallback(png_structp pPngStruct, png_bytep pData, png_size_t nSize)
{
	IFile* pFile = (IFile*)png_get_io_ptr(pPngStruct);

	if (!pFile->Read(pData, nSize))
	{
		png_error(pPngStruct,"PngReaderCallback failed");
	}
}

IBitmapData* ResMgr_Impl::CreateBitmapData(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;
	return InternalCreateBitmapData(strFilePath);
}

ITexture* ResMgr_Impl::CreateTexture(const IBitmapData* pBitmapData)
{
	Texture_Impl* pTexture = new Texture_Impl();
	if (!pTexture->LoadFromBitmapData(pBitmapData))
	{
		SAFE_DELETE(pTexture);
		return NULL;
	}

	return pTexture;
}

ITexture* ResMgr_Impl::CreateTexture(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;

	// TODO: check texture is exist in the cache

	IBitmapData* pBitmapData = InternalCreateBitmapData(strFilePath);
	if (!pBitmapData) return NULL;

	ITexture* pTexture = CreateTexture(pBitmapData);
	// TODO: cache the texture res

	return pTexture;
}

IBitmapData* ResMgr_Impl::InternalCreateBitmapData(const tstring& strFile)
{
	IFile* pFile = FileUtil::LoadFile(strFile.c_str());
	if (!pFile) return NULL;

	png_structp pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pPngStruct)
	{
		png_destroy_read_struct(&pPngStruct, NULL, NULL);
		SAFE_RELEASE(pFile);
		return NULL;
	}

	png_infop pPngInfo = png_create_info_struct(pPngStruct);
	if (!pPngInfo)
	{
		png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);
		SAFE_RELEASE(pFile);
		return NULL;
	}

	setjmp(png_jmpbuf(pPngStruct));

	// define our own callback function for I/O instead of reading from a file
	png_set_read_fn(pPngStruct, pFile, PngReaderCallback);
	png_read_info(pPngStruct, pPngInfo);

	uint width = png_get_image_width(pPngStruct, pPngInfo);
	uint height = png_get_image_height(pPngStruct, pPngInfo);
	// can be PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_PALETTE, ...
	png_byte colorType = png_get_color_type(pPngStruct, pPngInfo);
	uint bpp = png_get_bit_depth(pPngStruct, pPngInfo);

	// convert stuff to appropriate formats!
	if(colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_packing(pPngStruct);
		// expand data to 24-bit RGB or 32-bit RGBA if alpha available
		png_set_palette_to_rgb(pPngStruct);
	}

	// expand data to 24-bit RGB or 32-bit RGBA if alpha available
	if (colorType == PNG_COLOR_TYPE_GRAY && bpp < 8) png_set_expand_gray_1_2_4_to_8(pPngStruct);
	if (colorType == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(pPngStruct);
	if (bpp == 16) png_set_strip_16(pPngStruct);

	// expand paletted or RGB images with transparency to full alpha channels so the data will be available as RGBA quartets.
	if(png_get_valid(pPngStruct, pPngInfo, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pPngStruct);

	// create bitmap data
	IBitmapData* pBitmapData = CreateBitmapData(width, height);
	if (!pBitmapData)
	{
		png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);
		SAFE_RELEASE(pFile);
		return NULL;
	}

	uchar* pData = (uchar*)pBitmapData->GetData();

	// read image data into pRowPointers
	uchar** pRowPointers = new uchar*[height];
	for (uint y = 0; y < height; y++)
	{
		pRowPointers[y] = pData + (y*width*4);		//each pixel(RGBA) has 4 bytes
	}
	png_read_image(pPngStruct, pRowPointers);

	// free the stream object and png structure
	png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);
	SAFE_RELEASE(pFile);
	SAFE_DELETE_ARRAY(pRowPointers);

	return pBitmapData;
}
