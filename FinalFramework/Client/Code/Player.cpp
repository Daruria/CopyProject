#include "stdafx.h"
#include "Player.h"
#include "War.h"
//#include "PlayerObserver.h"
#include "PlayerTransform.h"
#include "PlayerSubject.h"

#include "Export_Function.h"

#define SC_CHA(Enum) SC_INT(ECharacter::Enum)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	Engine::Add_Subject(L"Player", CPlayerSubject::Create());

	m_vecCharacters.resize(SC_CHA(EEND), nullptr);

	FAILED_CHECK_RETURN(Ready_War(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Strife(), E_FAIL);

	Change_Character(ECharacter::WAR);

	//Engine::Notify(L"Player", CPlayerObserver::TRANSFORM, m_pActiveChar->Get_Transform());
	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::TRANSFORM), m_pActiveChar->Get_Transform());
	_ulong dwNaviIndex = m_pActiveChar->Get_NaviIndex();
	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::NAVI_INDEX), &dwNaviIndex);

	m_wstrName = L"Player";

	

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput_General(fTimeDelta);

	
	_int iExit = m_pActiveChar->Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//Engine::Notify(L"Player", CPlayerObserver::TRANSFORM, m_pActiveChar->Get_Transform());
	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::TRANSFORM), m_pActiveChar->Get_Transform());

	_ulong dwNaviIndex = m_pActiveChar->Get_NaviIndex();
	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::NAVI_INDEX), &dwNaviIndex);

	return iExit;
}

_int CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = m_pActiveChar->LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CPlayer::Render_GameObject(const _float& fTimeDelta)
{

}

void CPlayer::KeyInput_General(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_COMMA))
		Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_3RD);
	
	if (Engine::KeyDown(DIK_PERIOD))
		Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER);
	
	if (Engine::KeyDown(DIK_SLASH))
		Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_CONTROLL);

	if (Engine::KeyDown(DIK_MINUS))
		g_bFrameLimit = !g_bFrameLimit;

	if (Engine::KeyDown(DIK_B))
	{ 
		m_pActiveChar->Set_Pos(62.6961f, -0.0746874f, -143.364f);
		m_pActiveChar->Set_NaviIndex(157);
	}

	if (Engine::KeyDown(DIK_V))
	{
		m_pActiveChar->Set_Pos(0.f, 0.f, 0.f);
		m_pActiveChar->Set_NaviIndex(0);
	}

	if (Engine::KeyDown(DIK_C))
	{
		m_pActiveChar->Set_Pos(177.072f, -10.799f, 15.7958f);
		m_pActiveChar->Set_NaviIndex(84);
	}

	if (Engine::KeyDown(DIK_6))
	{
		Engine::Get_MainCamera()->CameraShake();
	}
}

HRESULT CPlayer::Ready_War()
{
	CWar* pWar = CWar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pWar, E_FAIL);

	m_vecCharacters[SC_CHA(WAR)] = pWar;

	return S_OK;
}

HRESULT CPlayer::Ready_Strife()
{

	return S_OK;
}

HRESULT CPlayer::Change_Character(ECharacter eCharacter)
{
	NULL_CHECK_RETURN(m_vecCharacters[SC_INT(eCharacter)], E_FAIL);
	Engine::Safe_Release(m_pActiveChar);

	m_pActiveChar = m_vecCharacters[SC_INT(eCharacter)];
	m_pActiveChar->AddRef();

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInst = new CPlayer(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CPlayer::Free()
{
	Engine::Safe_Release(m_pActiveChar);

	for (auto& pCha : m_vecCharacters)
		Engine::Safe_Release(pCha);

	CGameObject::Free();
}

