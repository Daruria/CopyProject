#ifndef ToolScene_h__
#define ToolScene_h__

#include "Scene.h"

USING(Engine)

class CToolScene : public CScene
{
private:
	explicit CToolScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolScene();
	
public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(const _float& fTimeDelta) override;

private:
	HRESULT Ready_EnvironmentLayer(const wstring& wstrLayerTag);
	HRESULT Ready_GameLogicLayer(const wstring& wstrLayerTag);

public:
	HRESULT SaveData(const wstring& wstrPath);
	HRESULT LoadData(const wstring& wstrPath);

public:
	static CToolScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // ToolScene_h__
