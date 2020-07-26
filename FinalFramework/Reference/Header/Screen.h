#ifndef Screen_h__
#define Screen_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CScreen : public CVIBuffer
{
private:
	explicit CScreen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScreen(const CScreen& rhs);
	virtual ~CScreen();

public:
	virtual HRESULT Ready_Buffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);

public:
	static CScreen* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);
	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // Screen_h__
