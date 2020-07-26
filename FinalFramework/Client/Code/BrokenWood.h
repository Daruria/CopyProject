#ifndef BrokenWood_h__
#define BrokenWood_h__

#include "MapObject.h"

BEGIN(Engine)

class CNavigationMesh;

END

class CBrokenWood : public CMapObject
{
protected:
	explicit CBrokenWood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBrokenWood();

public:
	virtual HRESULT Ready_GameObject() override;
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pDir, const _float& fAttanuation);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _vec3& rPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);

	void Set_Pos(const _vec3* pPos, const _ulong& dwNaviIndex);
	void Set_Pos(const _vec3& rPos, const _ulong& dwNaviIndex);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ, const _ulong& dwNaviIndex);

private:
	void Move(const _float& fTimeDelta);
	void Gravity(const _float& fTimeDelta);
	void Bounce(const _float& fTimeDelta);

private:
	_float		m_fUpSpeed = 20.f;
	_float		m_fGravity = 0.f;
	_float		m_fAttanuation = 0.8f;
	_vec3		m_vDir = { 0.f, 0.f, 0.f };
	_vec3		m_vRot = {};

	Engine::CNavigationMesh*	m_pNaviCom = nullptr;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CBrokenWood* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, const _vec3* pDir,
		const _float& fAttanuation = 0.8f);

protected:
	virtual void Free() override;
};

#endif // BrokenWood_h__
