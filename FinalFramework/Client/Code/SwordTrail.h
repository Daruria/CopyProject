#ifndef SwordTrail_h__
#define SwordTrail_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTrailBuffer;
class CTexture;
class CShader;
class CRenderer;

END

class CSwordTrail : public Engine::CGameObject
{
private:
	explicit CSwordTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwordTrail();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void Add_Vertex(const _vec3* pTopPos, const _vec3* pBottomPos);
	void Clear_Vertex();


protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTrailBuffer*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTexture*		m_pColorTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	list<pair<_vec3, _vec3>> m_TrailList;
	_bool					m_bOn = false;

public:
	static CSwordTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag);

private:
	virtual void Free();
};


#endif // SwordTrail_h__
