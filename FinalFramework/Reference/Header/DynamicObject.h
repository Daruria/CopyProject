#ifndef DynamicObject_h__
#define DynamicObject_h__

#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CDynamicMesh;
class CRenderer;
class CShader;
class CTexture;
class ENGINE_DLL CDynamicObject : public CGameObject
{
protected:
	explicit CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta, const _uint& iIndex) override;

	virtual void	Hit(_int iAttack, _vec3* pAtkPos) {};
	virtual void	Move_Pos(const _vec3* pPos) override;

public:
	CTransform*		Get_Transform() { return m_pTransformCom; }


	void		Set_Pos(const _vec3* pPos);
	void		Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void		Set_PosX(const _float& fX);
	void		Set_PosY(const _float& fY);
	void		Set_PosZ(const _float& fZ);

public:
	HRESULT			Add_Collider(const wstring& wstrTag, CCollider* pColl);

protected:
	virtual HRESULT	Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0);
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _uint& iIndex);

protected:
	CTransform*			m_pTransformCom = nullptr;
	CDynamicMesh*		m_pMeshCom = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	vector<CShader*>	m_vecShaderCom;
	CTexture*			m_pDissolveTextureCom = nullptr;

	_float				m_fDissolveTime = 0.f;
	_bool				m_bDissolve = false;

	_float				m_fRim = 0.f;
	_float				m_fCapture = 0.f;
	_float				m_fCapTimeCount = 0.f;
	_float				m_fCapTime = 1.f;

public:
	static CDynamicObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);


protected:
	virtual void Free() override;

};

END

#endif // DynamicObject_h__

