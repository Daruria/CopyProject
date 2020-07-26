#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject();

public:
	const CComponent*	Get_Component(const wstring& wstrComponentTag, COMPONENTID eID) const;
	CComponent*			Get_Component(COMPONENTID eID, const wstring& wstrComponentTag);

public:
	virtual HRESULT Ready_GameObject();
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName);
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta, const _uint& iIndex);

	virtual void	Collision_Enter(CGameObject* pOpponent) {}
	virtual void	Collision_Stay(CGameObject* pOpponent) {}
	virtual void	Collision_Leave(CGameObject* pOpponent) {}

	virtual void	Move_Pos(const _vec3* pPos) {}

protected:
	virtual HRESULT	Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0);

public:
	const wstring&	Get_Name()					const { return m_wstrName; }

	const _bool&	IsDead()					const { return m_bIsDead; }
	void			Set_IsDead(const _bool& bIsDead)  { m_bIsDead = bIsDead; }
	void			Die()							  { m_bIsDead = true; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<wstring, CComponent*>	m_mapComponent[ID_END];
	_bool						m_bIsDead = false;
	wstring						m_wstrName = L"";

public:
	virtual void Free();
};

END

#endif // GameObject_h__
