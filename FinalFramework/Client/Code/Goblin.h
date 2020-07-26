#ifndef Goblin_h__
#define Goblin_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CWeapon;
class CGoblin : public CMonster
{
public:
	enum class EGoblin { IDLE, RUN, FALL, IMPACT, DEATH01, SPAWN, ATTACK01, ATTACK02, ATTACK03, SIT_IDLE, SIT_END, EEND };

private:
	explicit CGoblin(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGoblin();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, EGoblin eStartState);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void Hit(_int iAttack, _vec3* pAtkPos);

private:
	virtual void	Decision_Position();
public:
	void Sit_End();

private:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

	virtual void Change_State();
	virtual _int Update_State(const _float& fTimeDelta);

	virtual void On_NaviMesh();

	HRESULT	SetUp_AnimIndex();

private:
	_int Update_Weapon(const _float& fTimeDelta);

private:
	_int Update_Idle(const _float& fTimeDelta);
	_int Update_Run(const _float& fTimeDelta);
	_int Update_Fall(const _float& fTimeDelta);
	_int Update_Attack01(const _float& fTimeDelta);
	_int Update_Attack02(const _float& fTimeDelta);
	_int Update_Attack03(const _float& fTimeDelta);
	_int Update_Death(const _float& fTimeDelta);
	_int Update_Impact(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_SitIdle(const _float& fTimeDelta);
	_int Update_SitEnd(const _float& fTimeDelta);

private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	EGoblin				m_eCurState = EGoblin::EEND;
	EGoblin				m_ePreState = EGoblin::EEND;

	_uint				m_uiTestAniIndex = 0;

	CWeapon*			m_pEquipWeapon = nullptr;

	Engine::CObserver*	m_pObserver = nullptr;
	_vec3				m_vTarget = {};
	_float				m_fUpSpeed = 0.f;

public:
	static CGoblin* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);
	static CGoblin* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, EGoblin eStartState);

protected:
	virtual void Free() override;

};

#endif // Goblin_h__
