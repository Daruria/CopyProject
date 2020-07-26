#ifndef GMolochSword_h__
#define GMolochSword_h__

#include "Weapon.h"

BEGIN(Engine)

class CCollider;
class CStaticMesh;

END

class CMolochSword : public CWeapon
{
private:
	explicit CMolochSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMolochSword();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Attack();
	virtual void Stop();

public:
	void CrystalOn();
	
private:
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CStaticMesh*	m_pCrystalMeshCom = nullptr;

	_bool				m_bTrail = false;
	class CSwordTrail*	m_pSwordTrail = nullptr;
	
	_bool				m_bCrystalOn = false;
	_float				m_fSwordLength = 3.2f;

	

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CMolochSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // GMolochSword_h__
