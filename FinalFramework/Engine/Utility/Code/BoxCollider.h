#ifndef BoxCollider_h__
#define BoxCollider_h__

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider : public CCollider 
{
public:
	enum BOX_PLANE { BP_LEFT, BP_RIGHT, BP_TOP, BP_BOTTOM, BP_FRONT, BP_BACK, BP_END };
	enum BOX_POINT { BPT_LEFT_BOTTOM_FRONT, BPT_LEFT_BOTTOM_BACK,	BPT_RIGHT_BOTTOM_BACK,	BPT_RIGHT_BOTTOM_FRONT,
					 BPT_LEFT_TOP_FRONT,	BPT_LEFT_TOP_BACK,		BPT_RIGHT_TOP_BACK,		BPT_RIGHT_TOP_FRONT,	BPT_END };

private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoxCollider();

private:
	HRESULT		Ready_BoxCollider(CCollider::PARENT_TYPE eParentType, void* pParent, const _matrix* pLocal, const _vec3* pSize, const wstring& wstrName);

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void Render_Collider() override;

	const _vec3* Get_Size(_vec3* pOut = nullptr) const { if (pOut) memcpy(pOut, &m_vSize, sizeof(_vec3)); return &m_vSize; }

	_bool	CheckUpdate() const { return m_bCheckUpdate; }
	
	void	Update_PointsPlanes();
	void	Update_Points();
	void	Update_Planes();

	const D3DXPLANE*	Get_WorldPlanes() const { return m_WorldPlane; }
	const _vec3*		Get_WorldPoints() const { return m_vWorldPoint; }

private:
	_vec3				m_vSize;

	_bool				m_bCheckUpdate = false;
	_vec3				m_vWorldPoint[BPT_END];
	D3DXPLANE			m_WorldPlane[BP_END];

#ifdef _DEBUG
	LPD3DXMESH			m_CubeMesh;
#endif // _DEBUG

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void* pParent,
		const _matrix* pLocal, const _vec3* pSize, const wstring& wstrName);

protected:
	virtual void Free() override;

};

END

#endif // BoxCollider_h__
