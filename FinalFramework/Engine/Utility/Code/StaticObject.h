#ifndef StaticObject_h__
#define StaticObject_h__

#include "GameObject.h"

USING(Engine)

BEGIN(Engine)

class CTransform;
class CRenderer;
class CMesh;
class CCollider;
class CShader;

class ENGINE_DLL CStaticObject : public CGameObject
{
protected:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta, const _uint& iIndex);

	virtual void	Move_Pos(const _vec3* pPos) override;
public:
	CTransform*		Get_Transform() { return m_pTransformCom; }

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void Set_PosX(const _float& fX);
	void Set_PosY(const _float& fY);
	void Set_PosZ(const _float& fZ);

	void Set_Scale(const _float& fScale);

public:
	HRESULT			Add_Collider(const wstring& wstrTag, CCollider* pColl);

protected:
	virtual HRESULT	Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0);
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _uint& iIndex);

protected:
	CTransform*		m_pTransformCom = nullptr;
	CMesh*			m_pMeshCom = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	vector<CShader*> m_vecShader;

	_vec3			m_vCenter = {};
	_float			m_fRadius = 0.f;

public:
	static CStaticObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);


protected:
	virtual void Free() override;

};

END
#endif // StaticObject_h__
