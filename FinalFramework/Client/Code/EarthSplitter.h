#ifndef EarthSplitter_h__
#define EarthSplitter_h__

#include "Skill.h"

BEGIN(Engine)

class CCollider;

END

class CCylinderEffect;
class CEarthSplitter : public CSkill
{
protected:
	explicit CEarthSplitter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthSplitter();

public:
	virtual HRESULT Ready_GameObject(const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;

	void Set_Navi(const _int& iNaviIndex) { m_iNavi = iNaviIndex; }

protected:
	HRESULT Add_Component();

	void Add_Effect();
	void Add_RockMeshEffect(const _vec3* pPos, const _vec3* pAngle, const _float& fTurnAngle);

protected:
	Engine::CCollider*		m_pColliderCom	= nullptr;
	_float					m_fLifeTime = 0.f;
	_float					m_fLifeCount = 0.f;
	
	_float					m_fIntervalTime = 0.f;
	_float					m_fSpeed = 20.f;
	_int					m_iNavi = 0;

	
public:
	static CEarthSplitter*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

protected:
	virtual void Free() override;
};

#endif // EarthSplitter_h__
