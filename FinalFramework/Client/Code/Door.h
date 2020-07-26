#ifndef Door_h__
#define Door_h__

#include "Defines.h"
#include "DynamicObject.h"

BEGIN(Engine)

class CNavigationMesh;

END

class CDoor : public Engine::CDynamicObject
{
public:
	enum DOOR_STATE { CLOSED, CLOSE, OPENED, OPEN, DOOR_STATE_END};


protected:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDoor();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, vector<_ulong>* pNaviIndices);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void	Open();
	void	Close();


private:
	DOOR_STATE		m_eCurState = CLOSED;
	vector<_ulong>	m_vecIndices;

	Engine::CNavigationMesh*	m_pNaviCom = nullptr;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrNameTag, vector<_ulong>* pNaviIndices);

protected:
	virtual void Free() override;
};

#endif // Door_h__
