#ifndef Moloch_h__
#define Moloch_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CMolochSword;
class CMoloch : public CMonster
{
private:
	enum class EMoloch {	IDLE, RUN, FALL, IMPACT, DEATH01, INTRO, 
							ATTACK_180_L, ATTACK_180_R, BLADE_EXTEND, CRYSTAL_ERUPTION, CRYSTAL_ERUPTION2, TREMOR_PULSE,
							ATTACK01, ATTACK02, ATTACK03, DASH_START, DASH_HOLD, DASH_STRIKE, 
							FULL_DASH_START, FULL_DASH, FULL_DASH_STRIKE, FULL_DASH_STRIKE2, FULL_GEYSER_START, FULL_GEYSER, FULL_GEYSER2, 
							FULL_SWING1, FULL_SWING2, FULL_SWING3, FULL_ATK_180_R, 
							FULL_IDLE, FULL_RUN,
							EEND };	
		
private:
	explicit CMoloch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoloch();

public:
	virtual HRESULT Ready_GameObject() override;
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pSpawnPos) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void Hit(_int iAttack, _vec3* pAtkPos);

private:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

	virtual void Change_State();
	virtual _int Update_State(const _float& fTimeDelta);

	virtual void On_NaviMesh();
	virtual void PlayAnimation(const _float& fTimeDelta);
	virtual void Decision_Position();

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
	_int Update_Intro(const _float& fTimeDelta);

	_int Update_Attack180L(const _float& fTimeDelta);
	_int Update_Attack180R(const _float& fTimeDelta);

	_int Update_BladeExtend(const _float& fTimeDelta);
	_int Update_CrystalEruption(const _float& fTimeDelta);
	_int Update_CrystalEruption2(const _float& fTimeDelta);
	_int Update_TremorPulse(const _float& fTimeDelta);

	_int Update_DashStart(const _float& fTimeDelta);
	_int Update_DashHold(const _float& fTimeDelta);
	_int Update_DashStrike(const _float& fTimeDelta);

	_int Update_FullIdle(const _float& fTimeDelta);
	_int Update_FullRun(const _float& fTimeDelta);

	_int Update_FullDashStart(const _float& fTimeDelta);
	_int Update_FullDash(const _float& fTimeDelta);
	_int Update_FullDashStrike(const _float& fTimeDelta);
	_int Update_FullDashStrike2(const _float& fTimeDelta);

	_int Update_FullGeyserStart(const _float& fTimeDelta);
	_int Update_FullGeyser(const _float& fTimeDelta);
	_int Update_FullGeyser2(const _float& fTimeDelta);

	_int Update_FullSwing1(const _float& fTimeDelta);
	_int Update_FullSwing2(const _float& fTimeDelta);
	_int Update_FullSwing3(const _float& fTimeDelta);
	_int Update_FullAtk180R(const _float& fTimeDelta);

private:
	void	Create_Crystal(const _vec3* pPos);
	void	Boom_Crystal();

private:
	Engine::CCollider*	m_pColliderCom	= nullptr;
	EMoloch				m_eCurState		= EMoloch::EEND;
	EMoloch				m_ePreState		= EMoloch::EEND;

	_bool				m_bFullMode		= false;

	CMolochSword*		m_pEquipWeapon	= nullptr;

	Engine::CObserver*	m_pObserver		= nullptr;

	_vec3				m_vSpawnPos = {};

	_bool				m_bAtk_Start	= false;
	_float				m_fPatternDelay = 0.f;
	_int				m_iPattern		= 0;

	_float				m_fDashSpeed	= 40.f;
	_float				m_fDashTime		= 0.f;
	_vec3				m_vDashStartPos = {0.f, 0.f, 0.f};

	_int				m_iGeyserCount	= 0;
	_bool				m_bChainAtk = false;
	_bool				m_bCutScene = false;

	list<class CMeshEffect*> m_pCrytalList;

	_bool				m_bFootStepSound = false;

public:
	static CMoloch* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, const _vec3* pSpawnPos);

protected:
	virtual void Free() override;

};

#endif // Moloch_h__
