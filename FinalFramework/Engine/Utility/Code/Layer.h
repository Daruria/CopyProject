#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:	//Get Functions
	const CComponent*	Get_Component(const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObjcet(const wstring& wstrObjTag) const;
	const multimap<wstring, CGameObject*>* Get_LayerMap() const { return &m_mapObject; }
	size_t				Get_ObjectMapSize() const { return m_mapObject.size(); }

public:
	CGameObject*	Get_GameObject_NC(const wstring& pObjTag);
	void			All_Die();
	void			Die_Object(const wstring& wstrObjTag);

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float& fTimeDelta);
	_int		LateUpdate_Layer(const _float& fTimeDelta);
	void		Render_Layer(const _float& fTimeDelta);

public:
	HRESULT		Add_GameObject(const wstring& wstrObjTag, CGameObject* pGameObject);

private:
	multimap<wstring, CGameObject*>	m_mapObject;

public:
	static CLayer*	Create();

private:
	virtual void Free();


};

END

#endif // Layer_h__
