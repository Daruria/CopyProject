#ifndef Character_h__
#define Character_h__

#include "Defines.h"
#include "DynamicObject.h"
#include "PlayerTransform.h"

BEGIN(Engine)

class CNavigationMesh;
class CCollider;

END

class CCharacter : public Engine::CDynamicObject
{
protected:
	explicit CCharacter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCharacter();

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	virtual void	Collision_Enter(CGameObject* pOpponent);
	virtual void	Collision_Stay(CGameObject* pOpponent);
	virtual void	Collision_Leave(CGameObject* pOpponent);

	virtual void	Hit(_int iAttack, _vec3* pAtkPos);
	virtual void	Move_Pos(const _vec3* pPos);

	_int			Get_Attack() const { return m_iAttack; }
	_ulong			Get_NaviIndex() const;
	
	void			Set_NaviIndex(const _ulong& dwIndex);
	
protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	virtual void Change_State() PURE;
	virtual void Update_State(const _float& fTimeDelta) PURE;
	virtual void KeyInput(const _float& fTimeDelta) PURE;

protected:
	void Change_State(_int iState);
	void Change_Idle();
	void Change_Run();
	void Change_Jump();
	void Change_Fall();
	void Change_Dash();
	void Change_Test();

protected:
	void Update_Idle(const _float& fTimeDelta);
	void Update_Run(const _float& fTimeDelta);
	void Update_Jump(const _float& fTimeDelta);
	void Update_Fall(const _float& fTimeDelta);
	virtual void Update_Dash(const _float& fTimeDelta);
	void Update_Test(const _float& fTimeDelta);
	void Update_Impact(const _float& fTimeDelta);

protected:
	void KeyInput_Move(const _float& fTimeDelta);
	void KeyInput_Jump(const _float& fTimeDelta);
	void KeyInput_Fall(const _float& fTimeDelta);
	void KeyInput_Dash(const _float& fTimeDelta);
	void KeyInput_Test(const _float& fTimeDelta);

protected:
	void Falling(const _float& fTimeDelta);
	void SmoothTurn(const _float& fTimeDelta);
	void Gravity(const _float& fTimeDelta);
	void PlayAnimation(const _float& fTimeDelta);
	virtual void Stop_Attack();

	void On_NaviMesh();
	void Decision_Position();
	void Check_NaviIndex();
	void Set_AnimVariable(_int iState, _int iAnimIdx, ANIMATION_TYPE eAnimType, _double dItpl_Time);

	void Add_BloodParticle(const _vec3* pAtkPos);

protected:
	UState	m_eCurState = EPlayer::FINISH;
	UState	m_ePreState = EPlayer::FINISH;
	ANIMATION_TYPE	m_eCurAnimType = AT_REPEAT;	

	_float			m_fSpeed = 10.f;
	_vec3			m_vMoveDir = {};

	_float			m_fJumpSpeed = 30.f;
	_float			m_fGravity = 0.f;

	_vec3			m_vOldPos = {};
	_ulong			m_dwOldIndex = 0;

	_uint			m_uiTestAniIndex = 0;

	vector<_uint>			m_vecAnimSetIdx;
	vector<ANIMATION_TYPE>	m_vecAnimSetType;
	vector<_double>			m_vecAnimItpl;

	_int			m_iHP = 200;
	_int			m_iHPMax = 200;
	_int			m_iAttack = 20;

	_vec3			m_vDisplacement = {};
	_vec3			m_vAnimDiffer	= {};

	Engine::CNavigationMesh*		m_pNaviCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;

	_float			m_fRunSoundItvl = 0.f;

	_bool			m_bEffect = false;
	_float			m_fDashTimeCount = 0.f;
	_float			m_fDashEffectTime = 0.f;

#ifdef _DEBUG
	_bool		m_bNaviMeshRender = false;
#endif // _DEBUG


protected:
	virtual void Free() override;
};


#endif // Character_h__
