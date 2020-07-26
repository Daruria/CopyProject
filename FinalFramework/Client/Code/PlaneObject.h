#ifndef PlaneObject_h__
#define PlaneObject_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CRenderer;

END

class CPlaneObject : public Engine::CGameObject
{
private:
	explicit CPlaneObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlaneObject();

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;
	virtual HRESULT Add_Component(const _ushort& wBufferConIdx, const wstring& wstrBufferTag,
		const _ushort& wTextureConIdx, const wstring& wstrTextureTag);

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	HRESULT		Ready_GameObject(const _vec3* pPos, const _vec3* pAngle, const _vec3* pScale,
		const _ushort& wBufferConIdx, const wstring& wstrBufferTag,
		const _ushort& wTextureConIdx, const wstring& wstrTextureTag, const wstring& wstrName);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

public:
	static CPlaneObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pAngle, const _vec3* pScale, 
		const _ushort& wBufferConIdx, const wstring& wstrBufferTag, 
		const _ushort& wTextureConIdx, const wstring& wstrTextureTag, const wstring& wstrName);

private:
	virtual void Free() override;

};


#endif // PlaneObject_h__
