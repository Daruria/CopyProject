#ifndef SkySphere_h__
#define SkySphere_h__

#include "Defines.h"
#include "StaticObject.h"

BEGIN(Engine)

class CTexture;

END

class CSkySphere : public Engine::CStaticObject
{
private:
	explicit CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkySphere();

public:
	HRESULT			Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _ulong& dwFlags);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(const _float& fTimeDelta) override;

public:
	void Start_ChangeColor(const _vec4* pAfterColor);

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect) override;

private:
	Engine::CTexture*	m_pDissolveTextureCom = nullptr;

	_float		m_fDissolveTime = 0.f;
	_bool		m_bDissolve = false;
	_vec4		m_vColor		= { 1.f, 1.f, 1.f, 1.f };
	_vec4		m_vDissolveColor = { 1.f, 0.5f, 0.25f, 1.f };
	_vec4		m_vAfterColor	= { 1.f, 0.5f, 0.25f, 1.f };
	_bool		m_bTest = false;;

public:
	static CSkySphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, const _ulong& dwFlags = 0);

private:
	virtual void Free() override;

};


#endif // SkySphere_h__
