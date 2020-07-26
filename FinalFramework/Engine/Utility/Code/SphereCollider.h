#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CCollider
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider();

private:
	HRESULT		Ready_SphereCollider(CCollider::PARENT_TYPE eParentType, void* pParent,
									const _matrix* pLocal, const _float& fRadius, const wstring& wstrName);

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void Render_Collider() override;


public:
	_float Get_Radius() const { return m_fRadius; }

private:
	_float		m_fRadius;

#ifdef _DEBUG
	LPD3DXMESH			m_SphereMesh;
#endif // _DEBUG

	

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void* pParent, 
		const _matrix* pLocal, const _float& fRadius, const wstring& wstrName, const wstring& wstrBoneName = L"");

	virtual CComponent* Clone(void) override;

private:
	virtual void Free();

};

END

#endif // SphereCollider_h__
