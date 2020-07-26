#ifndef Skill_h__
#define Skill_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CShader;

END

class CSkill : public Engine::CGameObject
{
protected:
	explicit CSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkill();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;

	virtual void Collision_Enter(CGameObject* pOpponent) override;
	virtual void Collision_Stay(CGameObject* pOpponent) override;
	virtual void Collision_Leave(CGameObject* pOpponent) override;

public:
	void	Set_Attack(const _int& iAttack)		{ m_iAttack = iAttack; }
	_int	Get_Attack()				const	{ return m_iAttack; }
	Engine::CTransform* Get_Transform()			{ return m_pTransformCom; }

protected:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT & pEffect);

protected:
	Engine::CTransform*		m_pTransformCom		= nullptr;
	Engine::CRenderer*		m_pRendererCom		= nullptr;
	Engine::CShader*		m_pShaderCom		= nullptr;

	_float					m_fAlpha = 1.f;
	_int					m_iAttack = 150;

protected:
	virtual void Free() override;
};

#endif // Skill_h__
