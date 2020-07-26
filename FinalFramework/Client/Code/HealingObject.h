#ifndef HealingObject_h__
#define HealingObject_h__

#include "Defines.h"
#include "StaticObject.h"

BEGIN(Engine)

class CTexture;
class CObserver;
class CCollider;

END

class CHealingObject : public Engine::CStaticObject
{
private:
	enum class EState { Spread, Stop, Move, End };

protected:
	explicit CHealingObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHealingObject();

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void	Collision_Enter(CGameObject* pOpponent)	override;
	virtual void	Collision_Stay(CGameObject* pOpponent) override;
	virtual void	Collision_Leave(CGameObject* pOpponent)override;

public:
	void Set_Variables(const _float& fInitSpeed, const _float& fInitAccel, const _vec3* pSpreadDir);

public:
	const _int&		Get_HealingPoint() const { return m_iHealingPoint; }

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect) override;
	
private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	Engine::CObserver*	m_pPlayerObserver = nullptr;
	_float	m_fSpeed = 0.f;
	_float	m_fAcceleration = 10.f;

	_int	m_iHealingPoint = 10;
	
	EState	m_eState = EState::Spread;
	_vec3	m_vSpreadDir = {};

public:
	static CHealingObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);

protected:
	virtual void Free() override;
};

#endif // HealingObject_h__
