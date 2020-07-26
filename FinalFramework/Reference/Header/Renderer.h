#ifndef Renderer_h__
#define Renderer_h__

#include "GameObject.h"

#ifdef _DEBUG
#include "Collider.h"
#endif // _DEBUG


BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	HRESULT Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(const _float& fTimeDelta);
	void	Render_Tool(const _float& fTimeDelta);
	void	Clear_RenderGroup();
	void	On_Blur()	{ ++m_iMotionBlur; }
	void	Off_Blur()	{ if (m_iMotionBlur > 0) --m_iMotionBlur; }

private:
	void	Render_Priority(const _float& fTimeDelta);
	void	Render_NonAlpha(const _float& fTimeDelta);
	void	Render_NonAlpha(const _float& fTimeDelta, const _uint& iIndex);
	void	Render_Alpha(const _float& fTimeDelta);
	void	Render_UI(const _float& fTimeDelta);

	void	Render_Deferred(const _float& fTimeDelta);
	void	Render_LightAcc(const _float& fTimeDelta);

	void	Render_BlurFilter(const _float& fTimeDelta);
	void	Render_Blur(const _float& fTimeDelta, const wstring& wstrTarget,const _uint& iPass);


	void	Render_BeforeBlend(const _float& fTimeDelta);
	void	Render_Blend(const _float& fTimeDelta);

	void	Render_PreDepth(const _float& fTimeDelta);

	void	Render_Final(const _float& fTimeDelta);

	void	Render_Effect(const _float& fTimeDelta);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	list<CGameObject*>	m_RenderGroup[RENDER_END];
	
	_bool				m_bRenderTarget = false;
	class CScreen*		m_pBlendBuffer = nullptr;
	class CShader*		m_pLightShader = nullptr;
	class CShader*		m_pBlendShader = nullptr;

	class CScreen*		m_pBlurFilterBuffer = nullptr;
	class CShader*		m_pBlurFilterShader = nullptr;

	class CScreen*		m_pBlurBuffer = nullptr;
	class CShader*		m_pBlurShader = nullptr;

	class CScreen*		m_pPreDepthBuffer = nullptr;
	class CShader*		m_pPreDepthShader = nullptr;

	class CScreen*		m_pFinalBuffer = nullptr;
	class CShader*		m_pFinalShader = nullptr;

	class CScreen*		m_pBeforeBlendBuffer = nullptr;
	class CShader*		m_pBeforeBlendShader = nullptr;

	_int		m_iMotionBlur = 0;

#ifdef _DEBUG
public:
	void				Add_RenderCollider(CCollider* pCollider);
	void				Clear_RenderCollider();
private:
	void				Render_Collider();

	_bool				m_bColliderRender = false;
	list<CCollider*>	m_ColliderList;
#endif // _DEBUG


private:
	virtual void Free(void);
};

END

#endif // Renderer_h__
