#ifndef TrailEffect_h__
#define TrailEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CTexture;
class CTrailBuffer;
class CMoveOnRoute;

END

class CTrailEffect : public CEffect
{
private:
	explicit CTrailEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrailEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _ushort& wRenderID, const _uint& iRenderPass);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void Add_Vertex(const _vec3* pTopPos, const _vec3* pBottomPos);
	void Clear_Vertex();

	void Set_Axis(const Engine::INFO& eInfo) { m_eAxis = eInfo; }
	void Set_UpLength(const _float& fLength) { m_fUpLength = fLength; }
	void Set_DownLength(const _float& fLength) { m_fDownLength = fLength; }
	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_InterpoleCnt(const _uint& iInterpoleCnt) { m_iInterpoleCnt = iInterpoleCnt; }

	HRESULT Add_MoveOnRoute(Engine::CMoveOnRoute* pRouteOnMove);

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTrailBuffer*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pColorTextureCom = nullptr;

	list<pair<_vec3, _vec3>> m_TrailList;

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;
	
	_float	m_fItvlTime = 0.f;
	_float	m_fInterval = 0.05f;
	_uint	m_iInterpoleCnt = 1;
	_float	m_fUpLength = 0.1f;
	_float	m_fDownLength = 0.1f;
	Engine::INFO	m_eAxis = Engine::INFO_RIGHT;

	_ushort		m_wRenderID = Engine::RENDER_ALPHA;
	_uint		m_iRenderPass = 2;

public:
	static CTrailEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor, _ushort wRenderID = (_ushort)Engine::RENDER_ALPHA, _uint iRenderPass = 2);

private:
	virtual void Free();
};


#endif // TrailEffect_h__
