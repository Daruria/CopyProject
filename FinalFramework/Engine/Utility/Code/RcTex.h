#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
public:
	enum RC_TEX_POS 
	{ 
		RTS_LEFT_TOP,		RTS_CENTER_TOP,		RTS_RIGHT_TOP,
		RTS_LEFT_CENTER,	RTS_CENTER_CENTER,	RTS_RIGHT_CENTER,
		RTS_LEFT_BOTTOM,	RTS_CENTER_BOTTOM,	RTS_RIGHT_BOTTOM
	};

private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	virtual HRESULT Ready_Buffer() override;
	HRESULT Ready_Buffer(RC_TEX_POS ePos);

public:
	void	Set_LocalPos(const _vec3* pLeftTop, const _vec3* pRightTop, const _vec3* pRightBottom, const _vec3* pLeftBottom);
	void	Set_LocalPos(RC_TEX_POS ePos);
	void	Set_UV(const _vec2* pLeftTop, const _vec2* pRightTop, const _vec2* pRightBottom, const _vec2* pLeftBottom);
	void	Set_UV(const _vec2* pLeftTop, const _float& fU, const _float& fV);
	void	Set_UV(RC_TEX_POS ePos, const _float& fU, const _float& fV);
	void	Set_UV(RC_TEX_POS ePos, const _vec2* pUV);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, RC_TEX_POS ePos);

	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // RcTex_h__
