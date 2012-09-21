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

	Shader_Impl(const tstring& id, const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem);
	virtual ~Shader_Impl();

	virtual const tstring& GetId() const;

	virtual bool Commit();

	virtual bool SetMatrix4x4(const Matrix4x4& m, const tstring& strName);
	virtual bool SetTexture(ITexture* pTexture, const tstring& strName, uint nIndex = 0);

	virtual bool DrawTriangleList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual const VertexAttribute* GetVertexAttribute() const;

private:
	bool CreateShader(const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem);
	void DestroyShader();

	uint CompileShader(uint nShaderType, const tstring& strShader);
	bool GetShaderErrorLog(uint nShader);
	bool GetProgramErrorLog(uint nProgram);

private:
	tstring m_strId;
	uint m_nProgram;

	uint m_nVertexShader;
	uint m_nGeometryShader;
	uint m_nFragmentShader;

	tstring m_strError;

	const VertexAttribute* m_pVertexAttribute;

};
#endif // __SHADER_IMPL_H__
