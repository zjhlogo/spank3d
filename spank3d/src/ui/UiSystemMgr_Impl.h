/*!
 * \file UiSystemMgr_Impl.h
 * \date 10-10-2012 9:20:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UISYSTEMMGR_IMPL_H__
#define __UISYSTEMMGR_IMPL_H__

#include <ui/IUiSystemMgr.h>

class UiSystemMgr_Impl : public IUiSystemMgr
{
public:
	RTTI_DEF(UiSystemMgr_Impl, IUiSystemMgr);

	UiSystemMgr_Impl();
	virtual ~UiSystemMgr_Impl();

	static UiSystemMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

};
#endif // __UISYSTEMMGR_IMPL_H__