#ifndef MapObject_h__
#define MapObject_h__

#include "Defines.h"
#include "StaticObject.h"

BEGIN(Engine)

class CTexture;
class CNavigationMesh;

END

class CMapObject : public Engine::CStaticObject
{
protected:
	explicit CMapObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapObject();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);

	virtual void	Collision_Enter(CGameObject* pOpponent)	override;
	virtual void	Collision_Stay(CGameObject* pOpponent) override;
	virtual void	Collision_Leave(CGameObject* pOpponent)override;

private:
	void Add_HealingObject(const _vec3* pPos);
	void Add_Dust(const _vec3* pPos);

private:
	_float		m_fDissolveTime = 0.f;
	Engine::CTexture*	m_pDissolveTextureCom = nullptr;
	Engine::CNavigationMesh*	m_pNaviCom = nullptr;
	
	_int		m_iHP = 3;
	_bool		m_bShake = false;
	_float		m_fShakingTime = 0.f;
	_vec3		m_vShakingDir = {};
	_vec3		m_vOriginPos = {};

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CMapObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);

protected:
	virtual void Free() override;
};

#endif // MapObject_h__
