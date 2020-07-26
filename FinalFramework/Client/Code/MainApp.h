#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
}

class CMouse;
class CMainApp : public CBase 
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	_int	LateUpdate_MainApp(const _float& fTimeDelta);
	void	Render_MainApp(const _float & fTimeDelta);

private:
	HRESULT SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	Engine::CManagement*	m_pManagement = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;

	CMouse*					m_pMouse = nullptr;

private:
	_int	m_iExit;


public:
	static	CMainApp*	Create();

private:
	virtual void		Free();
};


#endif // MainApp_h__
