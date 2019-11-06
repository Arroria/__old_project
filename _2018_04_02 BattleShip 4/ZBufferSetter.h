#pragma once
#include "Singleton.h"
class ZBufferSetter :
	public Singleton<ZBufferSetter>
{
public:
	void ZBufferOn	()		{ DEVICE->SetRenderState(D3DRS_ZENABLE, true); }
	void ZBufferOff	()		{ DEVICE->SetRenderState(D3DRS_ZENABLE, false); }
	void ZBufferWriteOn	()	{ DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true); }
	void ZBufferWriteOff()	{ DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false); }

public:
	ZBufferSetter();
	~ZBufferSetter();
};


#define ZBufOn()		(SingletonInstance(ZBufferSetter)->ZBufferOn())	
#define ZBufOff()		(SingletonInstance(ZBufferSetter)->ZBufferOff())	
#define ZBufWriteOn()	(SingletonInstance(ZBufferSetter)->ZBufferWriteOn())
#define ZBufWriteOff()	(SingletonInstance(ZBufferSetter)->ZBufferWriteOff())
