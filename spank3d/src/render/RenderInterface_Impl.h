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

private:
	IShader* InternalCreateShader(const tstring& strVertexShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem);

};
#endif // __RENDERINTERFACE_IMPL_H__
