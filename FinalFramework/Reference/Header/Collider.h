#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
public:
	enum PARENT_TYPE { PT_BONE, PT_TRANSFORM, PT_OBJECT, PT_COLLIDER, PT_END };

protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	Render_Collider();

public:
	virtual void	Activate()		{ m_bActivated = true; }
	virtual void	Deactivate()	{ m_bActivated = false; }
	_bool			Is_Activate()	{ return m_bActivated; }

	void			Set_Bone(const _matrix* pmatBone) { m_pParentBone = pmatBone; }
	COLLISIONTYPE	Check_CollisionType(void* pOpponent, _bool bCollision);

#ifdef _DEBUG
	virtual	void	Set_Color(COLLIDERCOLOR eColor) { m_eColor = eColor; }
#endif // _DEBUG

public:
	COLLIDERTYPE	Get_CollType()	const { return m_eType; }
	const wstring&	Get_Name()		const { return m_wstrName; }
	const _matrix*	Get_LocalMatrix(_matrix* pOut = nullptr)	const { if (pOut) { memcpy(pOut, &m_matLocal, sizeof(_matrix)); } return &m_matLocal; }
	const _matrix*	Get_WorldlMatrix(_matrix* pOut = nullptr)	const { if (pOut) { memcpy(pOut, &m_matWorld, sizeof(_matrix)); } return &m_matWorld; }
	void			Get_Pos(_vec3* pOut)						const { if (pOut) { memcpy(pOut, m_matWorld.m[3], sizeof(_vec3)); } }

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	COLLIDERTYPE		m_eType;
	wstring				m_wstrName;
	_matrix				m_matLocal, m_matWorld;

	PARENT_TYPE			m_eParentType = PT_END;
	void*				m_pParent = nullptr;
	const _matrix*		m_pParentBone = nullptr;

	_bool				m_bActivated = false;
	list<void*>			m_CollidedList;

#ifdef _DEBUG
protected:
	HRESULT				Ready_ColliderTexture(LPDIRECT3DDEVICE9 pGraphicDev);

	class CTexture*		m_pTextureCom = nullptr;
	COLLIDERCOLOR		m_eColor = CC_GREEN;
#endif // _DEBUG


protected:
	virtual void Free() override;
};

END

#endif // Collider_h__
