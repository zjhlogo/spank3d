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

Shader_Impl::Shader_Impl(const tstring& strVertexShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
	m_nProgram = 0;
	m_nVertexShader = 0;
	m_nFragmentShader = 0;
	m_pVertexAttribute = NULL;
	CreateShader(strVertexShader, strFragmentShader, pVertexAttrItem);
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

bool Shader_Impl::CreateShader(const tstring& strVertexShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
	if (!pVertexAttrItem) return false;

	m_pVertexAttribute = new VertexAttribute(pVertexAttrItem);
	if (!m_pVertexAttribute  || !m_pVertexAttribute->IsOk()) return false;

	m_nVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* pszVertexShader = StringUtil::tchar2char(strVertexShader.c_str());
	glShaderSource(m_nVertexShader, 1, &pszVertexShader, NULL);
	glCompileShader(m_nVertexShader);
	if (GetShaderErrorLog(m_nVertexShader))
	{
		LOG(_("Compile vertex shader failed with error log %s"), m_strError.c_str());
		return false;
	}

	m_nFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pszFragmentShader = StringUtil::tchar2char(strFragmentShader.c_str());
	glShaderSource(m_nFragmentShader, 1, &pszFragmentShader, NULL);
	glCompileShader(m_nFragmentShader);
	if (GetShaderErrorLog(m_nFragmentShader))
	{
		LOG(_("Compile fragment shader failed with error log %s"), m_strError.c_str());
		return false;
	}

	m_nProgram = glCreateProgram();
	glAttachShader(m_nProgram, m_nVertexShader);
	glAttachShader(m_nProgram, m_nFragmentShader);

	glLinkProgram(m_nProgram);
	if (GetProgramErrorLog(m_nProgram))
	{
		LOG(_("Link program failed with error log %s"), m_strError.c_str());
		return false;
	}

	SetOk(true);
	return true;
}

void Shader_Impl::DestroyShader()
{
	if (m_nProgram)
	{
		glDetachShader(m_nProgram, m_nVertexShader);
		glDetachShader(m_nProgram, m_nFragmentShader);
		glDeleteProgram(m_nProgram);
		m_nProgram = 0;
	}

	if (m_nVertexShader)
	{
		glDeleteShader(m_nVertexShader);
		m_nVertexShader = 0;
	}

	if (m_nFragmentShader)
	{
		glDeleteShader(m_nFragmentShader);
		m_nFragmentShader = 0;
	}

	SAFE_DELETE(m_pVertexAttribute);
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
