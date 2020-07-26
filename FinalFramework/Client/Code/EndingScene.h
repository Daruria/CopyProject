#ifndef EndingScene_h__
#define EndingScene_h__

#include "Defines.h"
#include "Scene.h"

class CEndingScene : public Engine::CScene
{
private:
	explicit CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEndingScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(const _float& fTimeDelta) override;

private:
	HRESULT		Ready_Environment_Layer(const wstring& wstrLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const wstring& wstrLayerTag);
	HRESULT		Ready_UI_Layer(const wstring& wstrLayerTag) { return S_OK; }

private:
	class CLoading*		m_pLoading = nullptr;

public:
	static CEndingScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // EndingScene_h__
