#ifndef Thrust_h__
#define Thrust_h__

#include "Skill.h"

BEGIN(Engine)

class CCollider;

END

class CCylinderEffect;
class CThrust : public CSkill
{
protected:
	explicit CThrust(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThrust();

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;


protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag);

protected:
	Engine::CCollider*		m_pColliderCom	= nullptr;
	_float		m_fLifeTime = 0.f;
	_float		m_fLifeCount = 0.f;
	
	
public:
	static CThrust*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

protected:
	virtual void Free() override;
};

#endif // Thrust_h__
