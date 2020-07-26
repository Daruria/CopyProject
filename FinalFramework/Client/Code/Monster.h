#ifndef Monster_h__
#define Monster_h__

#include "Defines.h"
#include "DynamicObject.h"
BEGIN(Engine)
class CNavigationMesh;
END

class CMonster : public Engine::CDynamicObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta, const _uint& iIndex) override;

public:
	virtual void	Collision_Enter(CGameObject* pOpponent);
	virtual void	Collision_Stay(CGameObject* pOpponent);
	virtual void	Collision_Leave(CGameObject* pOpponent);

	virtual void	Hit(_int iAttack, _vec3* pAtkPos);

	void	Add_DamageFont(const _int& iAttack);
	void	Add_BloodParticle(const _vec3* pAtkPos);

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fx, const _float& fy, const _float& fz);
	void Set_Angle(const _vec3* pAngle);
	void Set_Angle(const _float& fX, const _float& fY, const _float& fZ);
	void Set_AngleX(const _float& fX);
	void Set_AngleY(const _float& fY);
	void Set_AngleZ(const _float& fZ);

	void Set_NaviIndex(const _ulong& dwIndex);
	void Find_CorrectIndex();

	_int Get_Attack() const { return m_iAttack; }
	void Get_Pos(_vec3* pOut) const;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect) override;

	virtual void	Change_State();
	virtual _int	Update_State(const _float& fTimeDelta);

	void			Change_State(_int iState);
	void			Set_AnimVariable(_int iState, _int iAnimIdx, ANIMATION_TYPE eAnimType, _double dItpl_Time);

	virtual void	On_NaviMesh();

	void			Gravity(const _float& fTimeDelta);
	virtual void	PlayAnimation(const _float& fTimeDelta);
	virtual void	Decision_Position();

	virtual void	Attack_Stop();

	void			Update_Link();

protected:
	Engine::CNavigationMesh* m_pNaviCom = nullptr;
	ANIMATION_TYPE			m_eCurAnimType = AT_REPEAT;

	_float			m_fSpeed = 10.f;
	_vec3			m_vMoveDir = {};

	_float			m_fJumpSpeed = 40.f;
	_float			m_fGravity = 0.f;

	_vec3			m_vOldPos = {};
	_ulong			m_dwOldIndex = 0;

	_vec3			m_vDisplacement = {};
	_vec3			m_vAnimDiffer = {};

	_bool			m_bFirstRender = false;

	vector<_uint>			m_vecAnimSetIdx;
	vector<ANIMATION_TYPE>	m_vecAnimSetType;
	vector<_double>			m_vecAnimItpl;

	_int			m_iHP;
	_int			m_iHPMax;
	
	_int			m_iAttack;

	list<BONE_LINKER*>	m_LinkList;

	_bool				m_bEffect = false;
	_bool				m_bEffectSound = false;


public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);


protected:
	virtual void Free() override;

};

#endif // Monster_h__
