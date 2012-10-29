/*!
 * \file TextureUtil.cpp
 * \date 10-29-2012 10:28:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/TextureUtil.h>
#include <Spank3d.h>
#include <util/FileUtil.h>
#include <libpng1.5.12/png.h>
#include <vector>

static void PngReaderCallback(png_structp pPngStruct, png_bytep pData, png_size_t nSize)
{
	IFile* pFile = (IFile*)png_get_io_ptr(pPngStruct);

	if (!pFile->Read(pData, nSize))
	{
		png_error(pPngStruct,"PngReaderCallback failed");
	}
}

static IBitmapData* CreateBitmapData(png_struct* pPngStruct, png_info* pPngInfo, const tstring& id, uint width, uint height, uint bpp)
{
	// create bitmap data
	IBitmapData* pBitmapData = g_pResMgr->CreateBitmapData(id, width, height, bpp);
	if (!pBitmapData) return NULL;

	uint nBytesPerPixel = bpp / 8;
	uchar* pData = (uchar*)pBitmapData->GetData();

	// read image data into vRowPointers
	std::vector<uchar*> vRowPointers;
	for (uint y = 0; y < height; ++y)
	{
		vRowPointers.push_back(pData + ((height-y-1)*width*nBytesPerPixel));		//each pixel nBytesPerPixel bytes
	}

	png_read_image(pPngStruct, &vRowPointers[0]);
	return pBitmapData;
}

static IBitmapData* CreatePaletteBitmapData(png_struct* pPngStruct, png_info* pPngInfo, const tstring& id, uint width, uint height)
{
	// get palette
	png_color* pPalette = NULL;
	int nNumPalette = 0;
	png_get_PLTE(pPngStruct, pPngInfo, &pPalette, &nNumPalette);

	// get trans palette
	png_byte* pTrans = NULL;
	png_color_16* pTransColor = NULL;
	int nNumTrans = 0;
	png_get_tRNS(pPngStruct, pPngInfo, &pTrans, &nNumTrans, &pTransColor);

	// create bitmap data
	IBitmapData* pBitmapData = NULL;

	if (pTrans)
	{
		pBitmapData = g_pResMgr->CreateBitmapData(id, width, height, 32);
		if (!pBitmapData) return NULL;

		uchar* pData = (uchar*)pBitmapData->GetData();

		// read image data into pRowPointers
		std::vector<uchar*> vRowPointers;
		for (uint y = 0; y < height; ++y)
		{
			vRowPointers.push_back(new png_byte[width]);
		}
		png_read_image(pPngStruct, &vRowPointers[0]);

		for (uint y = 0; y < height; ++y)
		{
			for (uint x = 0; x < width; ++x)
			{
				uint nRGBAIndex = y*width*4 + x*4;
				uint nPaletteIndex = vRowPointers[height-y-1][x];
				pData[nRGBAIndex+0] = pPalette[nPaletteIndex].red;
				pData[nRGBAIndex+1] = pPalette[nPaletteIndex].green;
				pData[nRGBAIndex+2] = pPalette[nPaletteIndex].blue;
				pData[nRGBAIndex+3] = pTrans[nPaletteIndex];
			}
		}

		for (uint y = 0; y < height; ++y)
		{
			png_byte* pRow = vRowPointers[y];
			SAFE_DELETE_ARRAY(pRow);
		}
		vRowPointers.clear();
	}
	else if (pTransColor)
	{
		// TODO: not support trans color yet
	}
	else
	{
		pBitmapData = g_pResMgr->CreateBitmapData(id, width, height, 24);
		if (!pBitmapData) return NULL;

		uchar* pData = (uchar*)pBitmapData->GetData();

		// read image data into pRowPointers
		std::vector<uchar*> vRowPointers;
		for (uint y = 0; y < height; ++y)
		{
			vRowPointers.push_back(new png_byte[width]);
		}
		png_read_image(pPngStruct, &vRowPointers[0]);

		for (uint y = 0; y < height; ++y)
		{
			for (uint x = 0; x < width; ++x)
			{
				uint nRGBAIndex = y*width*3 + x*3;
				uint nPaletteIndex = vRowPointers[height-y-1][x];
				pData[nRGBAIndex+0] = pPalette[nPaletteIndex].red;
				pData[nRGBAIndex+1] = pPalette[nPaletteIndex].green;
				pData[nRGBAIndex+2] = pPalette[nPaletteIndex].blue;
			}
		}

		for (uint y = 0; y < height; ++y)
		{
			png_byte* pRow = vRowPointers[y];
			SAFE_DELETE_ARRAY(pRow);
		}
		vRowPointers.clear();
	}

	return pBitmapData;
}

IBitmapData* TextureUtil::DecodePngFromFile(const tstring& strFullPath)
{
	IFile* pFile = FileUtil::LoadFile(strFullPath.c_str());
	if (!pFile) return NULL;

	png_struct* pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info* pPngInfo = png_create_info_struct(pPngStruct);
	setjmp(png_jmpbuf(pPngStruct));

	// define our own callback function for I/O instead of reading from a file
	png_set_read_fn(pPngStruct, pFile, PngReaderCallback);
	png_read_info(pPngStruct, pPngInfo);

	uint width = png_get_image_width(pPngStruct, pPngInfo);
	uint height = png_get_image_height(pPngStruct, pPngInfo);
	uint colorType = png_get_color_type(pPngStruct, pPngInfo);
	uint bpp = png_get_bit_depth(pPngStruct, pPngInfo);

	IBitmapData* pBitmapData = NULL;
	if (bpp == 8)			// noly support bpp of 8
	{
		switch (colorType)
		{
		case PNG_COLOR_TYPE_GRAY:
			{
				pBitmapData = CreateBitmapData(pPngStruct, pPngInfo, strFullPath, width, height, 8);
			}
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			{
				pBitmapData = CreateBitmapData(pPngStruct, pPngInfo, strFullPath, width, height, 16);
			}
			break;
		case PNG_COLOR_TYPE_PALETTE:
			{
				pBitmapData = CreatePaletteBitmapData(pPngStruct, pPngInfo, strFullPath, width, height);
			}
			break;
		case PNG_COLOR_TYPE_RGB:
			{
				pBitmapData = CreateBitmapData(pPngStruct, pPngInfo, strFullPath, width, height, 24);
			}
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			{
				pBitmapData = CreateBitmapData(pPngStruct, pPngInfo, strFullPath, width, height, 32);
			}
			break;
		}
	}

	png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);
	SAFE_RELEASE(pFile);

	return pBitmapData;
}

bool TextureUtil::IsValidTextureSize(uint width, uint height)
{
	static const uint s_nValidSize[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 0};

	bool bValidWidth = false;
	bool bValidHeight = false;

	int nIndex = 0;
	while (s_nValidSize[nIndex] != 0)
	{
		if (!bValidWidth && width == s_nValidSize[nIndex])
		{
			bValidWidth = true;
		}
		if (!bValidHeight && height == s_nValidSize[nIndex])
		{
			bValidHeight = true;
		}

		++nIndex;
	}

	return bValidWidth && bValidHeight;
}
