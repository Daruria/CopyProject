#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CShader;
class CRenderer;

END

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void Set_PosX(const _float& fX);
	void Set_PosY(const _float& fY);
	void Set_PosZ(const _float& fZ);

	void Set_Scale(const _vec3* pScale);
	virtual void Set_Scale(const _float& fScale);
	void Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void Set_ScaleX(const _float& fX);
	void Set_ScaleY(const _float& fY);
	void Set_ScaleZ(const _float& fZ);

	void Set_LifeTime(const _float& fLifeTime)		{ m_fLifeTime = fLifeTime; }
	void Set_DeltaScale(const _vec3* pDeltaScale)	{ memcpy(&m_vDeltaScale, pDeltaScale, sizeof(_vec3)); }
	void Set_MaxScale(const _vec3* pMaxScale)		{ memcpy(&m_vMaxScale, pMaxScale, sizeof(_vec3)); }
	void SetUp_ScaleChange(const _vec3* pStartScale, const _vec3* pMaxScale, const _float& fScalingSpeed);

	void Set_Rotation(const _vec3* pRot);
	void Set_Rotation(const _float& fX, const _float&fY, const _float& fZ);
	void Set_RotationX(const _float& fX);
	void Set_RotationY(const _float& fY);
	void Set_RotationZ(const _float& fZ);

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	_float	m_fLifeTime = 1.f;
	_float	m_fLifeCount = 0.f;
	_vec3	m_vDeltaScale = { 0.f, 0.f, 0.f };
	_vec3	m_vMaxScale = { 0.f, 0.f, 0.f };

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag);

protected:
	virtual void Free();
};


#endif // Effect_h__
