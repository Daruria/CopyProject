#ifndef FallenDog_h__
#define FallenDog_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CFallenDog : public CMonster
{
public:
	enum class EFallenDog { IDLE, RUN, FALL, SPAWN, IMPACT_BACK, IMPACT_FRONT, IMPACT_LEFT, IMPACT_RIGHT, IMPACT_HEAVY, DEATH01, ATTACK01, ATTACK02, ATK_3HIT, START_WALK, EEND };

private:
	explicit CFallenDog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFallenDog();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	HRESULT		Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, EFallenDog eStartState);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void Hit(_int iAttack, _vec3* pAtkPos);

private:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

	virtual void Change_State();
	virtual _int Update_State(const _float& fTimeDelta);

	virtual void On_NaviMesh();

	HRESULT	SetUp_AnimIndex();

	virtual void Attack_Stop();

	_int Update_Hands(const _float& fTimeDelta);

private:
	_int Update_Idle(const _float& fTimeDelta);
	_int Update_Run(const _float& fTimeDelta);
	_int Update_Fall(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_Attack01(const _float& fTimeDelta);
	_int Update_Attack02(const _float& fTimeDelta);
	_int Update_Atk3Hit(const _float& fTimeDelta);
	_int Update_Death(const _float& fTimeDelta);
	_int Update_Impact(const _float& fTimeDelta);
	_int Update_StartWalk(const _float& fTimeDelta);

private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	class CInvisibleWeapon*	m_pLeftHand = nullptr;
	class CInvisibleWeapon*	m_pRightHand = nullptr;
	class CInvisibleWeapon* m_pHead = nullptr;

	EFallenDog			m_eCurState = EFallenDog::EEND;
	EFallenDog			m_ePreState = EFallenDog::EEND;

	_uint				m_uiTestAniIndex = 0;

	Engine::CObserver*	m_pObserver = nullptr;

	_float			m_fStartWalkTime = 0.f;


public:
	static CFallenDog* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);
	static CFallenDog* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, EFallenDog eStartState);

protected:
	virtual void Free() override;

};

#endif // FallenDog_h__
