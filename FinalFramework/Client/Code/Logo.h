#ifndef Logo_h__
#define Logo_h__

#include "Defines.h"
#include "Scene.h"

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(const _float& fTimeDelta) override;

private:
	HRESULT		Ready_Environment_Layer(const wstring& wstrLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const wstring& wstrLayerTag);
	HRESULT		Ready_UI_Layer(const wstring& wstrLayerTag) { return S_OK; }
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, Engine::RESOURCETYPE eMax);

public:
	static CLogo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // Logo_h__
