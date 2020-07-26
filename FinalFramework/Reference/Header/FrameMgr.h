#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase 
{
	DECLARE_SINGLETON(CFrameMgr)
private:
	explicit CFrameMgr();
	virtual	~CFrameMgr();

public:
	_bool IsPermit_Call(const wstring& wstrFrameTag, const _float& fTimeDelta);

public:
	HRESULT	Ready_Frame(const wstring& wstrFrameTag, const _float& fCallLimit);

private:
	CFrame* Find_Frame(const wstring& wstrFrameTag);
	

private:
	map<wstring, CFrame*> m_mapFrame;
	
public:
	virtual void Free();

};

END

#endif // FrameMgr_h__
