#ifndef RcAlphaTex_h__
#define RcAlphaTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcAlphaTex : public CVIBuffer
{
public:
	enum RC_TEX_POS
	{
		RTS_LEFT_TOP, RTS_CENTER_TOP, RTS_RIGHT_TOP,
		RTS_LEFT_CENTER, RTS_CENTER_CENTER, RTS_RIGHT_CENTER,
		RTS_LEFT_BOTTOM, RTS_CENTER_BOTTOM, RTS_RIGHT_BOTTOM
	};

private:
	explicit CRcAlphaTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcAlphaTex(const CRcAlphaTex& rhs);
	virtual ~CRcAlphaTex();

public:
	HRESULT		Ready_Buffer(const D3DXCOLOR& d3dColor);
	HRESULT		Ready_Buffer(const D3DXCOLOR& d3dColor, RC_TEX_POS ePos);


public:
	void	Set_Color(const D3DXCOLOR& d3dColor);
	void	Set_LocalPos(const _vec3* pLeftTop, const _vec3* pRightTop, const _vec3* pRightBottom, const _vec3* pLeftBottom);
	void	Set_LocalPos(RC_TEX_POS ePos);
	void	Set_UV(const _vec2* pLeftTop, const _vec2* pRightTop, const _vec2* pRightBottom, const _vec2* pLeftBottom);
	void	Set_UV(const _vec2* pLeftTop, const _float& fU, const _float& fV);
	void	Set_UV(RC_TEX_POS ePos, const _float& fU, const _float& fV);
	void	Set_UV(RC_TEX_POS ePos, const _vec2* pUV);

private:
	D3DXCOLOR	 m_d3dColor;


public:
	static CRcAlphaTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR& d3dColor);
	static CRcAlphaTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR& d3dColor, RC_TEX_POS ePos);
	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // RcAlphaTex_h__
