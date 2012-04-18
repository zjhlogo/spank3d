/*!
 * \file RenderInterface_Impl.cpp
 * \date 4-18-2012 21:47:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RenderInterface_Impl.h"
#include "Shader_Impl.h"
#include <util/FileUtil.h>
#include <Spank3D.h>

RenderInterface_Impl::RenderInterface_Impl()
{
	g_pRenderInterface = this;
}

RenderInterface_Impl::~RenderInterface_Impl()
{
	g_pRenderInterface = NULL;
}

RenderInterface_Impl& RenderInterface_Impl::GetInstance()
{
	static RenderInterface_Impl s_RenderInterface_Impl;
	return s_RenderInterface_Impl;
}

bool RenderInterface_Impl::Initialize()
{
	// TODO: 
	return true;
}

void RenderInterface_Impl::Terminate()
{
	// TODO: 
}

IShader* RenderInterface_Impl::CreateShader(const tstring& strVertexShaderFile, const tstring& strFragmentShaderFile)
{
	IFile* pVertexShaderFile = FileUtil::CreateNewFile(strVertexShaderFile);
	if (!pVertexShaderFile) return NULL;

	IFile* pFragmentShaderFile = FileUtil::CreateNewFile(strFragmentShaderFile);
	if (!pFragmentShaderFile)
	{
		SAFE_RELEASE(pVertexShaderFile);
		return NULL;
	}

	uint nVertexShaderFileSize = pVertexShaderFile->GetSize();
	char* pszVertexShaderCode = new char[nVertexShaderFileSize+1];
	pVertexShaderFile->Read(pszVertexShaderCode, nVertexShaderFileSize);
	pszVertexShaderCode[nVertexShaderFileSize] = '\0';

	uint nFragmentShaderFileSize = pFragmentShaderFile->GetSize();
	char* pszFragmentShaderCode = new char[nFragmentShaderFileSize+1];
	pFragmentShaderFile->Read(pszFragmentShaderCode, nFragmentShaderFileSize);
	pszFragmentShaderCode[nFragmentShaderFileSize] = '\0';

	Shader_Impl* pShader = new Shader_Impl(pszVertexShaderCode, pszFragmentShaderCode);

	SAFE_RELEASE(pVertexShaderFile);
	SAFE_RELEASE(pFragmentShaderFile);
	SAFE_DELETE_ARRAY(pszVertexShaderCode);
	SAFE_DELETE_ARRAY(pszFragmentShaderCode);

	if (!pShader || !pShader->IsOk())
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	return pShader;
}
