#ifndef UndeadChampion_h__
#define UndeadChampion_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CWeapon;
class CUndeadChampion : public CMonster
{
public:
	enum class EUndeadChampion { IDLE, RUN, FALL, IMPACT, DEATH01, SPAWN, SHOCKWAVE, SPEARTOSS, DASH_ATTACK,
		SUMMON_START, SUMMON, SUMMON_END, 
		BLOCK_START, BLOCK_IDLE, BLOCK_END, 
		CHARGE_START, CHARGE_IDLE, CHARGE_END,
		SPAWN_WALK, EEND };

	enum class EUndeadPhase { SHOCKWAVE, SPEARTOSS, DASH_ATTACK, SUMMON, BLOCK, CHARGE };

private:
	explicit CUndeadChampion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUndeadChampion();

public:
	virtual HRESULT Ready_GameObject() override;
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pSpawnPoint);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void Hit(_int iAttack, _vec3* pAtkPos);
	void Set_SpawnPoint(const _vec3* pPos) { m_vSpawnPoint = *pPos; }

private:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

	virtual void Change_State();
	virtual _int Update_State(const _float& fTimeDelta);

	virtual void On_NaviMesh();

	virtual void PlayAnimation(const _float& fTimeDelta);

	HRESULT	SetUp_AnimIndex();
private:
	_int Update_Weapon(const _float& fTimeDelta);

private:
	_int Update_Idle(const _float& fTimeDelta);
	_int Update_Run(const _float& fTimeDelta);
	_int Update_Fall(const _float& fTimeDelta);
	_int Update_Death(const _float& fTimeDelta);
	_int Update_Impact(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_ShockWave(const _float& fTimeDelta);
	_int Update_SpearToss(const _float& fTimeDelta);
	_int Update_DashAttack(const _float& fTimeDelta);
	_int Update_SummonStart(const _float& fTimeDelta);
	_int Update_Summon(const _float& fTimeDelta);
	_int Update_SummonEnd(const _float& fTimeDelta);
	_int Update_BlockStart(const _float& fTimeDelta);
	_int Update_BlockIdle(const _float& fTimeDelta);
	_int Update_BlockEnd(const _float& fTimeDelta);
	_int Update_ChargeStart(const _float& fTimeDelta);
	_int Update_ChargeIdle(const _float& fTimeDelta);
	_int Update_ChargeEnd(const _float& fTimeDelta);
	_int Update_SpawnWalk(const _float& fTimeDelta);


private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	EUndeadChampion		m_eCurState = EUndeadChampion::EEND;
	EUndeadChampion		m_ePreState = EUndeadChampion::EEND;

	_uint				m_uiTestAniIndex = 0;

	CWeapon*			m_pEquipWeapon = nullptr;
	CWeapon*			m_pEquipShield = nullptr;
	
	_int				m_iAtk_Phase = 0;
	_bool				m_bAtk_Start = false;

	Engine::CObserver*	m_pObserver = nullptr;
	
	_float				m_fPatternDelay = 0.f;
	
	_vec3					m_vSpawnPoint = {};
	class CUndeadPortal*	m_pSpawnPortal = nullptr;

	_float					m_fSpawnWalkTime = 0.f;
	
	_bool					m_bShockWave = false;

	_bool					m_bSummon = false;

	_bool					m_bEffect;

	_float					m_fChargeFxTime = 0.f;
	_bool					m_bSpearReturnSound = false;

	_float					m_fBlockTime = 0.f;
	_float					m_fBlockEffectTime = 0.f;

public:
	static CUndeadChampion* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, const _vec3* pSpawnPoint);

protected:
	virtual void Free() override;

};

#endif // UndeadChampion_h__
