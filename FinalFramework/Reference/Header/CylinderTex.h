#ifndef CylinderTex_h__
#define CylinderTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCylinderTex : public CVIBuffer
{
private:
	explicit CCylinderTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCylinderTex(const CCylinderTex& rhs);
	virtual	~CCylinderTex();

public:
	HRESULT Ready_Buffer(const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight) ;
	void Render_Buffer(const _float& fTimeDelta, const _float& fSpeed, const _vec2* pUV);
	void Render_Buffer();

private:
	_vec2 m_vUV;
	_ulong m_dwStacks;
	_ulong m_dwSlices;

public:
	static CCylinderTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight);
	virtual CResources*	Clone() override;

private:
	virtual void Free();

};

END
#endif // CylinderTex_h__
