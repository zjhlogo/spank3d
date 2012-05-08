/*!
 * \file RenderInterface_Impl.h
 * \date 4-18-2012 21:46:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RENDERINTERFACE_IMPL_H__
#define __RENDERINTERFACE_IMPL_H__

#include <render/IRenderInterface.h>

class RenderInterface_Impl : public IRenderInterface
{
public:
	RTTI_DEF(RenderInterface_Impl, IRenderInterface);

	RenderInterface_Impl();
	virtual ~RenderInterface_Impl();

	static RenderInterface_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IShader* CreateShader(const tstring& strShaderFile);
	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

private:
	IShader* InternalCreateShader(const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem);

private:
	tstring m_strDefaultDir;

};
#endif // __RENDERINTERFACE_IMPL_H__
