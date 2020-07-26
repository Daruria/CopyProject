#ifndef LoadingScene_h__
#define LoadingScene_h__

#include "Defines.h"
#include "Scene.h"
#include "Loading.h"

BEGIN(Engine)

class CTransform;

END
class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(const _float& fTimeDelta) override;

private:
	HRESULT		Ready_UI_Layer(const wstring& wstrLayerTag);

private:
	CLoading*				m_pLoading;
	Engine::CTransform*		m_pCircleTransform = nullptr;

public:
	static CLoadingScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // LoadingScene_h__
