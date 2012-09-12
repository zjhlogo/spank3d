/*!
 * \file Shader_Impl.cpp
 * \date 4-18-2012 10:28:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Shader_Impl.h"
#include <gl/glew.h>
#include <util/StringUtil.h>
#include <util/LogUtil.h>
#include "Texture_Impl.h"

Shader_Impl::Shader_Impl(const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
	m_nProgram = 0;
	m_nVertexShader = 0;
	m_nGeometryShader = 0;
	m_nFragmentShader = 0;
	m_pVertexAttribute = NULL;
	SetOk(CreateShader(strVertexShader, strGeometryShader, strFragmentShader, pVertexAttrItem));
}

Shader_Impl::~Shader_Impl()
{
	DestroyShader();
}

bool Shader_Impl::Commit()
{
	if (!IsOk()) return false;
	glUseProgram(m_nProgram);
	return true;
}

bool Shader_Impl::SetMatrix4x4(const Matrix4x4& m, const tstring& strName)
{
	if (!IsOk()) return false;

	int nLoc = glGetUniformLocation(m_nProgram, StringUtil::tchar2char(strName.c_str()));
	if (nLoc < 0) return false;

	glUniformMatrix4fv(nLoc, 1, false, m.e);
	return true;
}

bool Shader_Impl::SetTexture(ITexture* pTexture, const tstring& strName, uint nIndex /*= 0*/)
{
	if (!pTexture) return false;

	int nLoc = glGetUniformLocation(m_nProgram, StringUtil::tchar2char(strName.c_str()));
	if (nLoc < 0) return false;

	// active texture
	Texture_Impl* pTexture_Impl = (Texture_Impl*)pTexture;
	glActiveTexture(GL_TEXTURE0+nIndex);
	uint eError = glGetError();
	if (eError != GL_NO_ERROR) LOG("glActiveTexture error code: 0x%04x", eError);

	// bind texture
	glBindTexture(GL_TEXTURE_2D, pTexture_Impl->GetTextureId());
	eError = glGetError();
	if (eError != GL_NO_ERROR) LOG("glBindTexture error code: 0x%04x", eError);

	glUniform1i(nLoc, nIndex);
	eError = glGetError();
	if (eError != GL_NO_ERROR) LOG("glUniform1i error code: 0x%04x", eError);

	return true;
}

bool Shader_Impl::DrawTriangleList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	if (!IsOk()) return false;
	if (!pVerts || !pIndis) return false;
	if (nVerts <= 0 || nIndis <= 0) return false;

	// setup vertex attributes
	int nNumAttrs = m_pVertexAttribute->GetNumAttributeItems();
	for (int i = 0; i < nNumAttrs; ++i)
	{
		const VertexAttribute::ATTRIBUTE_ITEM* pAttrItem = m_pVertexAttribute->GetAttributeItem(i);

		glEnableVertexAttribArray(i);
		GLenum eType = VertexAttribute::GetGlType(pAttrItem->eItemType);
		glVertexAttribPointer(i, pAttrItem->nSize, eType, GL_FALSE, m_pVertexAttribute->GetStride(), ((const uchar*)pVerts)+pAttrItem->nOffset);
		glBindAttribLocation(m_nProgram, i, pAttrItem->szParamName);
	}

	glDrawElements(GL_TRIANGLES, nIndis, GL_UNSIGNED_SHORT, pIndis);
	return true;
}

const VertexAttribute* Shader_Impl::GetVertexAttribute() const
{
	return m_pVertexAttribute;
}

bool Shader_Impl::CreateShader(const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
	if (!pVertexAttrItem) return false;

	// setup vertex attributes
	m_pVertexAttribute = new VertexAttribute(pVertexAttrItem);
	if (!m_pVertexAttribute  || !m_pVertexAttribute->IsOk()) return false;

	m_nProgram = glCreateProgram();
	if (m_nProgram == 0) return false;

	// create vertex shader
	m_nVertexShader = CompileShader(GL_VERTEX_SHADER, strVertexShader);
	if (m_nVertexShader != 0) glAttachShader(m_nProgram, m_nVertexShader);

	// create geometry shader
	m_nGeometryShader = CompileShader(GL_GEOMETRY_SHADER, strGeometryShader);
	if (m_nGeometryShader != 0) glAttachShader(m_nProgram, m_nGeometryShader);

	// create fragment shader
	m_nFragmentShader = CompileShader(GL_FRAGMENT_SHADER, strFragmentShader);
	if (m_nFragmentShader != 0) glAttachShader(m_nProgram, m_nFragmentShader);

	glLinkProgram(m_nProgram);
	if (GetProgramErrorLog(m_nProgram))
	{
		LOG(_("Link program failed with error log %s"), m_strError.c_str());
		return false;
	}

	return true;
}

void Shader_Impl::DestroyShader()
{
	if (m_nProgram != 0)
	{
		glDetachShader(m_nProgram, m_nVertexShader);
		glDetachShader(m_nProgram, m_nGeometryShader);
		glDetachShader(m_nProgram, m_nFragmentShader);
		glDeleteProgram(m_nProgram);
		m_nProgram = 0;
	}

	if (m_nVertexShader != 0)
	{
		glDeleteShader(m_nVertexShader);
		m_nVertexShader = 0;
	}

	if (m_nGeometryShader != 0)
	{
		glDeleteShader(m_nGeometryShader);
		m_nGeometryShader = 0;
	}

	if (m_nFragmentShader != 0)
	{
		glDeleteShader(m_nFragmentShader);
		m_nFragmentShader = 0;
	}

	SAFE_DELETE(m_pVertexAttribute);
}

uint Shader_Impl::CompileShader(uint nShaderType, const tstring& strShader)
{
	if (strShader.length() <= 0) return 0;

	uint nShaderId = glCreateShader(nShaderType);
	if (nShaderId == 0) return 0;

	const char* pszShader = StringUtil::tchar2char(strShader.c_str());
	glShaderSource(nShaderId, 1, &pszShader, NULL);
	glCompileShader(nShaderId);
	if (GetShaderErrorLog(nShaderId))
	{
		LOG(_("Compile shader failed with error log %s"), m_strError.c_str());
		glDeleteShader(nShaderId);
		return 0;
	}

	return nShaderId;
}

bool Shader_Impl::GetShaderErrorLog(uint nShader)
{
	int nInfoLen = 0;
	glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &nInfoLen);
	if (nInfoLen <= 1) return false;

	char* pszInfoLog = new char[nInfoLen+1];

	int nCharWritten  = 0;
	glGetShaderInfoLog(nShader, nInfoLen, &nCharWritten, pszInfoLog);
	StringUtil::char2tchar(m_strError, pszInfoLog);

	SAFE_DELETE(pszInfoLog);
	return true;
}

bool Shader_Impl::GetProgramErrorLog(uint nProgram)
{
	int nInfoLen = 0;
	glGetProgramiv(nProgram, GL_INFO_LOG_LENGTH, &nInfoLen);
	if (nInfoLen <= 1) return false;

	char* pszInfoLog = new char[nInfoLen+1];

	int nCharWritten  = 0;
	glGetProgramInfoLog(nProgram, nInfoLen, &nCharWritten, pszInfoLog);
	StringUtil::char2tchar(m_strError, pszInfoLog);

	SAFE_DELETE(pszInfoLog);
	return true;
}
