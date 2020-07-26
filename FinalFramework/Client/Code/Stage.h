#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"

BEGIN(Engine)

class CObserver;
class CCamera;

END

class CStage : public Engine::CScene
{
	enum STAGE1_PHASE { S1P_START,		S1P_1_ING, S1P_1_END, S1P_2_START, S1P_2_ING, S1P_2_END, 
						S1P_3_START,	S1P_3_ING, S1P_3_END, S1P_4_START, S1P_4_ING, S1P_4_END,
						S1P_5_START,	S1P_5_ING, S1P_5_END, S1P_6_START, S1P_6_ING, S1P_6_END, S1P_END };

private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(const _float& fTimeDelta) override;

private:
	HRESULT		Ready_Environment_Layer(const wstring& wstrLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const wstring& wstrLayerTag);
	HRESULT		Ready_Monster_Layer(const wstring& wstrLayerTag);
	HRESULT		Ready_UI_Layer(const wstring& wstrLayerTag);
	HRESULT		Ready_Camera();
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_Prototype();

private:
	_int Update_Start(const _float& fTimeDelta);
	_int Update_1_Ing(const _float& fTimeDelta);
	_int Update_1_End(const _float& fTimeDelta);
	_int Update_2_Start(const _float& fTimeDelta);
	_int Update_2_Ing(const _float& fTimeDelta);
	_int Update_2_End(const _float& fTimeDelta);

	_int Update_3_Start(const _float& fTimeDelta);
	_int Update_3_Ing(const _float& fTimeDelta);
	_int Update_3_End(const _float& fTimeDelta);

	_int Update_4_Start(const _float& fTimeDelta);
	_int Update_4_Ing(const _float& fTimeDelta);
	_int Update_4_End(const _float& fTimeDelta);

	_int Update_5_Start(const _float& fTimeDelta);
	_int Update_5_Ing(const _float& fTimeDelta);
	_int Update_5_End(const _float& fTimeDelta);

	_int Update_End(const _float &fTimeDelta);

	_int Update_CameraAngle(const _float& fTimeDelta);

private:
	STAGE1_PHASE			m_eCurPhase = S1P_START;
	_float					m_fPhaseDelay = 0.f;
	_vec3					m_vStartArea[2];
	_int					m_iCountForNext = 0;
	class CDoor*			m_pPhase2Door = nullptr;
	class CPortal*			m_pPhase3Portal = nullptr;
	class CTricksterDoor*	m_pPhase5Door = nullptr;
	vector<class CGoblin*>	m_vecGoblins;
	_bool					m_bUndeadSpawn = false;

	Engine::CObserver*	m_pPlayerObserver = nullptr;
	Engine::CCamera*	m_pLightCam = nullptr;

	class CSkySphere*	m_pSkySphere = nullptr;

public:
	static CStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

};

#endif // Stage_h__
