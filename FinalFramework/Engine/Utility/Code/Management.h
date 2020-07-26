#ifndef Management_h__
#define Management_h__

#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();
public:
	const CComponent*	Get_Component(const wstring& wstrLayerTag, const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID);
	const CLayer*		Get_Layer(const wstring& wstrLayerTag) const;

public:
	HRESULT		Ready_Management(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT		SetUp_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	_int		LateUpdate_Scene(const _float& fTimeDelta);
	void		Render_Scene(const _float& fTimeDelta);

public:
	HRESULT		Add_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag, CGameObject* pObj);
	HRESULT		Add_LoadingLayer(const wstring& wstrLayerTag, CLayer* pLayer);
	HRESULT		Loading_Layer(map<wstring, CLayer*>* pmapLoadingLayer);
	HRESULT		Clear_LoadingLayer();

	void		Die_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag);

private:
	CScene*					m_pScene = nullptr;
	map<wstring, CLayer*>	m_mapLoadingLayer;

public:
	virtual void Free(void);
};

END



#endif // Management_h__
