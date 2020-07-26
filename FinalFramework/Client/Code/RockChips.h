#ifndef RockChips_h__
#define RockChips_h__

#include "Effect.h"

BEGIN(Engine)

class CRcTexInstance;
class CTexture;

END

class CRockChips : public CEffect
{
private:
	explicit CRockChips(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRockChips();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt, D3DXCOLOR dwColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void	Set_Speed(const _float& fSpeed);
	void	Set_Gravity(const _float& fGravity) { m_fGravity = fGravity; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt, D3DXCOLOR dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CRcTexInstance*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pColorTextureCom = nullptr;

	_float m_fSpeed = 1.f;
	_float	m_fAccTime = 0.f;
	_float	m_fGravity = 1.f;

public:
	static CRockChips* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt, D3DXCOLOR dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

private:
	virtual void Free();
};


#endif // RockChips_h__
