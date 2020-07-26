#ifndef Portal_h__
#define Portal_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCircleTex;
class CTexture;
class CShader;
class CRenderer;
class CNavigationMesh;

END

class CPortal : public Engine::CGameObject
{
public:
	enum class EPortal { OPENING, SPAWN, CLOSING, EEND };

private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortal();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, _vec4 vColor) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

private:
	_int Update_State(const _float& fTimeDelta);
	_int Update_Opening(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_Closing(const _float& fTimeDelta);

	void Change_NaviIndex(const _int& iOption);

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _vec3& rPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void Set_PosX(const _float& fX);
	void Set_PosY(const _float& fY);
	void Set_PosZ(const _float& fZ);

	EPortal		Get_State() const { return m_eCurState; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CCircleTex*			m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTexture*			m_pNoiseTextureCom = nullptr;
	Engine::CTexture*			m_pMaskTextureCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CNavigationMesh*	m_pNaviCom = nullptr;

	_float		m_fCurScale		= 0.f;
	_float		m_fSizing_Speed = 3.f;
	_float		m_fMaxScale		= 10.f;
	EPortal		m_eCurState		= EPortal::EEND;

	_float		m_fLifeCount = 0.f;
	_float		m_fIntervalTime = 0.f;
	_int		m_iMonsterCount = 0;

	_vec4		m_vColor = {};

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, _vec4 vColor = { 1.f, 1.f, 1.f, 1.f });

private:
	virtual void Free();
};


#endif // Portal_h__
