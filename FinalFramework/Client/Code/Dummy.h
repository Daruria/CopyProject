#ifndef Dummy_h__
#define Dummy_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CDynamicMesh;
class CShader;

END

class CDummy : public Engine::CGameObject 
{
private:
	explicit CDummy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDummy();

public:
	HRESULT Ready_GameObject(const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags = 0) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void SWSkinning(const _uint& iIndex, const _double& dItplTime, const _double& dPosition);
	void Set_Transform(const _vec3* pScale, const _vec3* pAngle, const _vec3* pPos);
	void Set_LifeTime(const _float& fLifeTime) { m_fLifeTime = fLifeTime; }
	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }
	void Set_Alpha(const _float& fAlpha) { m_fAlpha = fAlpha; }
	void SetUp_Variables(const _float& fLifeTime, const _float& fAlpha, const _float& fRim, const _float& fDeltaAlpha = 0.f, const _float& fDeltaRim = 0.f);
	void Reset();

protected:
	HRESULT Add_Component(const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags = 0);
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

protected:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRenderer = nullptr;
	Engine::CDynamicMesh*	m_pDynamicMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	_float	m_fLifeCount = 0.f;
	_float	m_fLifeTime = 0.f;
	_float	m_fAlpha = 0.f;
	_float	m_fRim = 0.f;

	_float	m_fDeltaAlpha = 0.f;
	_float	m_fDeltaRim = 0.f;

public:
	static CDummy*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags = 0);

protected:
	virtual void Free() override;
};

#endif // Dummy_h__
