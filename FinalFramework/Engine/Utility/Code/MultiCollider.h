#ifndef MultiCollider_h__
#define MultiCollider_h__

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CMultiCollider : public CCollider
{
private:
	explicit CMultiCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMultiCollider();


public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void Render_Collider() override;

#ifdef _DEBUG
	virtual	void	Set_Color(COLLIDERCOLOR eColor) { for (auto& pChild : m_ChildList) pChild->Set_Color(eColor); }
#endif // _DEBUG

public:
	HRESULT Ready_MultiCollider(CCollider::PARENT_TYPE eParentType, void* pParent,
		const _matrix* pLocal, list<CCollider*>* pColliderList, const wstring& wstrName);

	HRESULT Add_Collider(CCollider* pColl);

	const list<CCollider*>*	Get_ChildList() const { return &m_ChildList; }

public:
	virtual void	Activate()		{ m_bActivated = true;  for (auto& pChild : m_ChildList) pChild->Activate(); }
	virtual void	Deactivate()	{ m_bActivated = false; for (auto& pChild : m_ChildList) pChild->Deactivate();}

private:
	list<CCollider*>	m_ChildList;

public:
	static CMultiCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void* pParent,
		const _matrix* pLocal, list<CCollider*>* pColliderList, const wstring& wstrName);

private:
	virtual void Free();

};

END
#endif // MultiCollider_h__
