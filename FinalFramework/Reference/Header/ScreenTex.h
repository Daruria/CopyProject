#ifndef ScreenTex_h__
#define ScreenTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CScreenTex : public CVIBuffer
{
private:
	explicit CScreenTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScreenTex(const CScreenTex& rhs);
	virtual ~CScreenTex();

public:
	virtual HRESULT Ready_Buffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);

public:
	static CScreenTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);
	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // ScreenTex_h__
