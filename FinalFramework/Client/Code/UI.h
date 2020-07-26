#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRcAlphaTex;
class CTexture;
class CRenderer;

END

class CUI : public Engine::CGameObject 
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI();

	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrTextureTag);

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrTextureTag, const wstring& wstrName, const D3DXCOLOR& dwColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void	Set_Pos(const _float& fX, const _float& fY);
	void	Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void	Set_Pos(const _vec2* pPos);
	void	Set_Pos(const _vec3* pPos);
	void	Set_PosX(const _float& fX);
	void	Set_PosY(const _float& fY);
	void	Set_PosZ(const _float& fZ);

	void	Set_Scale(const _float& fX, const _float& fY);
	void	Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void	Set_Scale(const _vec2* pScale);
	void	Set_Scale(const _vec3* pScale);
	void	Set_ScaleX(const _float& fX);
	void	Set_ScaleY(const _float& fY);
	void	Set_ScaleZ(const _float& fZ);

	void	Set_RcPos(const _uint& uiRcPos);
	void	Set_UV(const _vec2* pUV);
	void	Set_UV(const _float& fU, const _float& fV);
	void	Set_U(const _float& fU);
	void	Set_V(const _float& fV);

public:
	_float	Get_OriginWidth(_uint iIndex) const;
	_float	Get_OriginHeight(_uint iIndex) const;
	void	Get_OriginSize(_vec2* pOut, _uint iIndex = 0) const;

protected:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRcAlphaTex*	m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

protected:
	_uint		m_uiRcPos;
	_vec2		m_vUV;
	D3DXCOLOR	m_dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrTextureTag, const wstring& wstrName, const D3DXCOLOR& dwColor);

protected:
	virtual void Free();
};

#endif // UI_h__
