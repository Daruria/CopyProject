#ifndef Weapon_h__
#define Weapon_h__

#include "Defines.h"
#include "StaticObject.h"

BEGIN(Engine)

class CTexture;

END

class CWeapon : public Engine::CStaticObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta, const _uint& iIndex) override;

public:
	void Set_BoneMatrix(const _matrix* pmatParent);
	void Set_ParentTransform(CTransform* pTransform);
	void Set_TransformFlags(const _ulong& dwFlags);
	
	void Set_Parent(const _matrix* pmatParent, CTransform* pParentTransform);
	
	void Add_TransformFlags(const _ulong& dwFlags);
	
	void Get_Pos(_vec3* pOut);
	void Get_WorldMatrix(_matrix* pOut);
	void Get_RenderMatrix(_matrix* pOut);

public:
	void Multiply_WorldMatrix(const _matrix* pMatrix);
	virtual void Attack() {}
	virtual void Stop() {}
	_int	Get_Attack() const { return m_iAttack; }
	void Start_Dissolve() { m_bDissolve = true; }

public:
	void Set_RenderOn() { m_bRender = true; }
	void Set_RenderOff() { m_bRender = false; }

protected:
	_int	m_iAttack = 20;

	_bool				m_bRender = true;

	Engine::CTexture*	m_pDissolveTextureCom = nullptr;
	_bool				m_bDissolve = false;
	_float				m_fDissolveTime = 0.f;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;


protected:
	virtual void Free();


};


#endif // Weapon_h__
