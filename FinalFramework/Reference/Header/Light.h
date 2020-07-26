#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CScreen;
class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);

public:
	const D3DLIGHT9*		Get_LightInfo(void) const { return &m_tLightInfo; }
	const _matrix*			Get_LightProj(_matrix* pOut = nullptr)	const { if (pOut) memcpy(pOut, &m_matLightProj, sizeof(_matrix)); return &m_matLightProj; }
	const _matrix*			Get_LightView(_matrix* pOut = nullptr)	const { if (pOut) memcpy(pOut, &m_matLightView, sizeof(_matrix)); return &m_matLightView; }

	void	Set_LightProj(const _float& fFovy, const _float& fAspect, const _float& fNear, const _float& fFar);
	void	Set_LightView(const _vec3* pEye, const _vec3* pAt, const _vec3*pUp);

public:
	HRESULT			Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void			Render_Light(LPD3DXEFFECT& pEffect);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;

	CScreen*					m_pScreenBuffer;
	_matrix						m_matLightProj;
	_matrix						m_matLightView;

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	virtual void		Free(void);
};

END
#endif // Light_h__
