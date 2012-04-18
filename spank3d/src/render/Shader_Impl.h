/*!
 * \file Shader_Impl.h
 * \date 4-18-2012 10:27:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SHADER_IMPL_H__
#define __SHADER_IMPL_H__

#include <render/IShader.h>

class Shader_Impl : public IShader
{
public:
	RTTI_DEF(Shader_Impl, IShader);

	Shader_Impl(const char* pVertexCode, const char* pFragmentCode);
	virtual ~Shader_Impl();

	virtual bool SetMatrix4x4(const Matrix4x4& m, const tstring& strName);
	virtual bool Commit();

private:
	bool CreateShader(const char* pVertexCode, const char* pFragmentCode);
	void DestroyShader();

	bool GetShaderErrorLog(uint nShader);
	bool GetProgramErrorLog(uint nProgram);

private:
	uint m_nProgram;
	uint m_nVertexShader;
	uint m_nFragmentShader;
	tstring m_strError;

};
#endif // __SHADER_IMPL_H__
