#ifndef SkeletonBow_h__
#define SkeletonBow_h__

#include "Weapon.h"

BEGIN(Engine)

class CCollider;

END

class CSkeletonBow : public CWeapon
{
private:
	explicit CSkeletonBow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkeletonBow();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Attack();
	virtual void Stop();
	
private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	
	_bool				m_bTrail = false;
	class CSwordTrail*	m_pSwordTrail = nullptr;
	

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CSkeletonBow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // SkeletonBow_h__
