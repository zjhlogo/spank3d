/*!
 * \file Device_Impl.h
 * \date 4-15-2012 11:59:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DEVICE_IMPL_H__
#define __DEVICE_IMPL_H__

#include <core/IDevice.h>

class Device_Impl : public IDevice
{
public:
	enum CONST_DEFINE
	{
		DEFAULT_WINDOW_WIDTH = 1024,
		DEFAULT_WINDOW_HEIGHT = 768,
// 		DEFAULT_FPS = 60,
	};

public:
	RTTI_DEF(Device_Impl, IDevice);

	static Device_Impl& GetInstance();

	bool Initialize();
	void Terminate();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual const Vector2& GetSize() const;
	virtual uint GetWidth() const;
	virtual uint GetHeight() const;

private:
	Device_Impl();
	virtual ~Device_Impl();

private:
	bool InternalCreateWindow();
	void InternalDestroyWindow();

	bool InternalCreateOGL();
	void InternalDestroyOGL();

	void PerformOnce(float dt);
	void InitializeOGL();

private:
	float m_fLastTime;

	Vector2 m_WindowSize;
	uint m_nWindowWidth;
	uint m_nWindowHeight;

};
#endif // __DEVICE_IMPL_H__
