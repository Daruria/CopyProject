#ifndef PlayerPool_h__
#define PlayerPool_h__

#include "Defines.h"
#include "Base.h"

class CPlayerPool : public CBase 
{
	SINGLETON(CPlayerPool)

private:
	explicit CPlayerPool();
	virtual ~CPlayerPool();

public:
	HRESULT Ready_PlayerPool(LPDIRECT3DDEVICE9	pGraphicDev , const wstring& wstrDummyTag, const wstring& wstrFilePath, const wstring& wstrFileName, const _uint& iCount, const _ulong& dwFlags = 0);
	HRESULT Get_Dummy(const wstring& wstrDummyTag, class CDummy** ppOutDummy);

	_int	LateUpdate_PlayerPool(const _float& fTimeDelta);

private:
	map<wstring, list<pair<_bool, class CDummy*>>*> m_mapDummyList;

private:
	virtual void Free() override;

};

#endif // PlayerPool_h__