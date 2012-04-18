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

Shader_Impl::Shader_Impl(const char* pVertexCode, const char* pFragmentCode)
{
	m_nProgram = 0;
	m_nVertexShader = 0;
	m_nFragmentShader = 0;
	CreateShader(pVertexCode, pFragmentCode);
}

Shader_Impl::~Shader_Impl()
{
	DestroyShader();
}

bool Shader_Impl::SetMatrix4x4(const Matrix4x4& m, const tstring& strName)
{
	if (!IsOk()) return false;

	int nLoc = glGetUniformLocation(m_nProgram, StringUtil::tchar2char(strName));
	if (nLoc <= 0) return false;

	glUniformMatrix4fv(nLoc, 1, false, m.e);
	return true;
}

bool Shader_Impl::Commit()
{
	if (!IsOk()) return false;

	glUseProgram(m_nProgram);
	return true;
}

bool Shader_Impl::CreateShader(const char* pVertexCode, const char* pFragmentCode)
{
	m_nVertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_nFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_nVertexShader, 1, &pVertexCode, NULL);
	glShaderSource(m_nFragmentShader, 1, &pFragmentCode, NULL);

	glCompileShader(m_nVertexShader);
	if (GetShaderErrorLog(m_nVertexShader))
	{
		LOG(_("Compile vertex shader failed with error log %s"), m_strError.c_str());
		return false;
	}

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
