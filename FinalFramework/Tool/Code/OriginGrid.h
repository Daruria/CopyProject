#ifndef OriginGrid_h__
#define OriginGrid_h__

#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CLineCol;
}

using namespace Engine;

class COriginGrid : public CGameObject
{
	explicit COriginGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COriginGrid();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

private:
	HRESULT Ready_Component();

private:
	CLineCol*	m_pBufferCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CRenderer*	m_pRenderer = nullptr;

private:
	_vec3 m_vWorldPos = { 0.f, 0.f, 0.f };
	_uint m_uiCntX = 10;
	_uint m_uiCntZ = 10;
	_float	m_fItv = 1.f;


public:
	static COriginGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};



#endif // OriginGrid_h__
