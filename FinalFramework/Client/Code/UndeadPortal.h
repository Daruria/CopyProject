#ifndef UndeadPortal_h__
#define UndeadPortal_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCircleTex;
class CRcTex;
class CTexture;
class CShader;
class CRenderer;
END

class CUndeadPortal : public Engine::CGameObject
{
public:
	enum class EUndeadPortal { OPENING, SPAWN, CLOSING, EEND };

private:
	explicit CUndeadPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUndeadPortal();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, 
		const wstring& wstrMaskTextureTag, const _float& fMaxScale, _vec4 vColor) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

private:
	_int Update_State(const _float& fTimeDelta);
	_int Update_Opening(const _float& fTimeDelta);
	_int Update_Spawn(const _float& fTimeDelta);
	_int Update_Closing(const _float& fTimeDelta);

public:
	void Set_Pos(const _vec3* pPos);
	void Set_Pos(const _vec3& rPos);
	void Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void Set_PosX(const _float& fX);
	void Set_PosY(const _float& fY);
	void Set_PosZ(const _float& fZ);

	void Set_Angle(const _float& fX, const _float& fY, const _float& fZ);
	void Set_State(EUndeadPortal eState);

	const EUndeadPortal& Get_State() const { return m_eCurState; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CCircleTex*			m_pBufferCom = nullptr;
	//Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTexture*			m_pNoiseTextureCom = nullptr;
	Engine::CTexture*			m_pMaskTextureCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

	_float		m_fCurScale		= 0.f;
	_float		m_fSizing_Speed = 10.f;
	_float		m_fMaxScale		= 5.f;
	_float		m_fLifeCount = 0.f;
	_vec4		m_vColor;

	EUndeadPortal		m_eCurState = EUndeadPortal::EEND;

public:
	static CUndeadPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, 
		const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, 
		const _float& fMaxScale, _vec4 vColor = { 1.f, 1.f, 1.f, 1.f });

private:
	virtual void Free();
};


#endif // UndeadPortal_h__
