#ifndef HitGround_h__
#define HitGround_h__

#include "Skill.h"

BEGIN(Engine)

class CCollider;

END

class CHitGround : public CSkill
{
protected:
	explicit CHitGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHitGround();

public:
	HRESULT Ready_GameObject(const _vec3* pPos, const _float& fRadius, const wstring& wstrCollTag);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;

public:
	void Set_LifeTime(const _float& fLifeTime) { m_fLifeTime = fLifeTime; }

protected:
	HRESULT Add_Component(const _float& fRadius);

protected:
	Engine::CCollider*		m_pColliderCom	= nullptr;
	_float					m_fLifeTime = 0.f;
	_float					m_fLifeCount = 0.f;
	
	wstring					m_wstrCollTag = L"";
	
public:
	static CHitGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float& fRadius, const wstring& wstrCollTag);

protected:
	virtual void Free() override;
};

#endif // HitGround_h__
