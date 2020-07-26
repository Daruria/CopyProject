#ifndef HellHound_h__
#define HellHound_h__

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CObserver;

END
class CHellHound : public CMonster
{
public:
	enum class EHellHound { IDLE, RUN, FALL, SPAWN, IMPACT_BACK, IMPACT_FRONT, IMPACT_LEFT, IMPACT_RIGHT, DEATH01, ATTACK01, EEND };

private:
	explicit CHellHound(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHellHound();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
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

	virtual void PlayAnimation(const _float& fTimeDelta);

private:
	_int Update_Idle(const _float& fTimeDelta);
	_int Update_Run(const _float& fTimeDelta);
	_int Update_Fall(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_Attack01(const _float& fTimeDelta);
	_int Update_Death(const _float& fTimeDelta);
	_int Update_Impact(const _float& fTimeDelta);

private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	EHellHound			m_eCurState = EHellHound::EEND;
	EHellHound			m_ePreState = EHellHound::EEND;

	_uint				m_uiTestAniIndex = 0;

	Engine::CObserver*	m_pObserver = nullptr;

	class CInvisibleWeapon*		m_pHead = nullptr;

public:
	static CHellHound* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);

protected:
	virtual void Free() override;

};

#endif // HellHound_h__
