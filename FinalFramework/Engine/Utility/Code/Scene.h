#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class CCollisionMgr;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	const CComponent*	Get_Component(const wstring& wstrLayerTag, const wstring& pObjTag, const wstring& wstrComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObjcet(const wstring& wstrLayerTag, const wstring& wstrObjTag) const;
	const CLayer*		Get_Layer(const wstring& wstrLayerTag) const;

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);

public:
	HRESULT		Add_Layer(const wstring& wstrLayerTag, CLayer* pLayer);
	HRESULT		Add_GameObject(const wstring& wstrLayerTag, const wstring&ObjTag, CGameObject* pObj);
	void		Die_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	map<wstring, CLayer*>		m_mapLayer;
	CCollisionMgr*				m_pCollisionMgr = nullptr;
	
protected:
	virtual void Free();
};

END

#endif // Scene_h__
