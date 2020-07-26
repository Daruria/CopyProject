#ifndef WoodPlatform_h__
#define WoodPlatform_h__

#include "MapObject.h"

BEGIN(Engine)

class CNavigationMesh;

END

class CWoodPlatform : public CMapObject
{
protected:
	explicit CWoodPlatform(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWoodPlatform();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	virtual void Collision_Enter(CGameObject* pOpponent) override;
	virtual void Collision_Stay(CGameObject* pOpponent) override;
	virtual void Collision_Leave(CGameObject* pOpponent) override;

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);

	void Set_Angle(const _float& fX, const _float& fY, const _float& fZ);

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

private:
	_int	m_iWood = 10;

	_int	m_iHitCount = 0;
	
	_bool	m_bShaking = false;
	_float	m_fShakingTime = 0.f;
	_vec3	m_vShakingDir = {};
	_vec3	m_vOriginPos = {};

	Engine::CNavigationMesh*	m_pNaviCom = nullptr;

public:
	static CWoodPlatform* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag);

protected:
	virtual void Free() override;
};

#endif // WoodPlatform_h__
