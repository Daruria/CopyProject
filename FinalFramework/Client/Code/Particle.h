#ifndef Particle_h__
#define Particle_h__

#include "Effect.h"

BEGIN(Engine)

class CParticleTex;
class CTexture;

END

class CParticle : public CEffect
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticle();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Speed(const _float& fSpeed);

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CParticleTex*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pColorTextureCom = nullptr;

	_float m_fSpeed = 1.f;
	_float	m_fAccTime = 0.f;

public:
	static CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt);

private:
	virtual void Free();
};


#endif // Particle_h__
