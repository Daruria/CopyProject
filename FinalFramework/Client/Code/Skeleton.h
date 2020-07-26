#ifndef Skeleton_h__
#define Skeleton_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CWeapon;
class CSkeleton : public CMonster
{
public:
	enum class ESkeleton { IDLE, RUN, FALL, IMPACT_F, IMPACT_B, IMPACT_L, IMPACT_R, DEATH, SPAWN,
		ATK_MELEE1, ATK_MELEE2, SPAWN_WALK, EEND
	};

private:
	explicit CSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkeleton();

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
	_int Update_AtkMelee1(const _float& fTimeDelta);
	_int Update_AtkMelee2(const _float& fTimeDelta);


private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	ESkeleton			m_eCurState = ESkeleton::EEND;
	ESkeleton			m_ePreState = ESkeleton::EEND;

	_uint				m_uiTestAniIndex = 0;

	CWeapon*			m_pEquipWeapon = nullptr;

	_int				m_iAtk_Phase = 0;
	_bool				m_bAtk_Start = false;

	Engine::CObserver*	m_pObserver = nullptr;
	
	_float				m_fPatternDelay = 0.f;
	
	_vec3					m_vSpawnPoint = {};
	class CUndeadPortal*	m_pSpawnPortal = nullptr;

	_float					m_fSpawnWalkTime = 0.f;

public:
	static CSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, 
		const _vec3* pSpawnPoint);

protected:
	virtual void Free() override;

};

#endif // Skeleton_h__
