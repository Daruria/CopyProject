#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "ControlCam.h"
#include "QuaterCam.h"
#include "Observer.h"

#include "Export_Function.h"
#include "MapObject.h"
#include "Goblin.h"
#include "SkySphere.h"
#include "FallenDog.h"
#include "HellHound.h"
#include "Moloch.h"
#include "ThirdViewCamera.h"
#include "Door.h"
#include "PlayerUI.h"
#include "Portal.h"
#include "WoodPlatform.h"
#include "UndeadChampion.h"
#include "Skeleton.h"
#include "TricksterDoor.h"
#include "CutSceneCam.h"
#include "CircularEffect.h"
#include "Particle.h"
#include "CylinderEffect.h"
#include "RockChips.h"
#include "MeshEffect.h"
#include "FireEffect.h"
#include "ChargeEffect.h"
#include "SparkEffect.h"
#include "SphereEffect.h"
#include "UVEffect.h"
#include "DistortionEffect.h"
#include "DamageFont.h"
#include "DamageEffect.h"
#include "EndingScene.h"
#include "HealingObject.h"
#include "EmptyEffect.h"
#include "FollowGlowRoute.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Monster_Layer(L"Layer_Monster"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Loading_Layer(&m_mapLayer), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);


	m_vStartArea[0] = _vec3{ -13.4f, 0.71196f, -7.2f };
	m_vStartArea[1] = _vec3{ 19.7f, 0.71196f, 26.f };

	m_pPlayerObserver = Engine::CObserver::Create(L"Player");

	m_eCurPhase = S1P_START;

	Engine::Stop_All();
	Engine::Play_BGM(L"mus_level01_ambient.ogg");

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	m_pLightCam->Update_Camera(fTimeDelta);

	_int iExit = 0;


	if (Engine::KeyDown(DIK_F1))
	{
		_vec3 vPos = { 0.f, 3.f, 0.f };

		//m_eCurPhase = S1P_5_END;

		wstring wstrBufferTag = L"Mesh_Crate_A";
		CMapObject* pObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
		NULL_CHECK_RETURN(pObj, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Crate_A", pObj), -1);
		pObj->Set_Pos(3.f, 0.71196f, 3.f);

		//CUVEffect* pDust = CUVEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//NULL_CHECK_RETURN(pDust, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Dust", pDust), -1);
		//pDust->Set_UV(&_vec2((rand() % 2) * 0.5f, (rand() % 2) * 0.5f), &_vec2(0.5f, 0.5f));
		//pDust->Set_Pos(&vPos);
		//pDust->SetUp_ScaleChange(&_vec3(3.f, 3.f, 3.f), &_vec3(3.2f, 3.2f, 3.2f), 1.f);
		//pDust->Set_LifeTime(1.f);
		//pDust->Set_DeltaAlpha(-1.f);
		//pDust->On_BillBoard();

		//CDamageEffect* pDmgFx = CDamageEffect::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_DamageFont", D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 100);
		//NULL_CHECK_RETURN(pDmgFx, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_DMG", pDmgFx), -1);
		//pDmgFx->Set_LifeTime(1.5f);
		//pDmgFx->Set_Pos(&vPos);
		//pDmgFx->Set_DigitScale(0.01f);
		//pDmgFx->SetUp_DigitScaling(0.f, 0.005f, 0.04f, -0.12f);

		//vPos = { WINCX * 0.5f, WINCY * 0.5f, 1.f };
		//CDamageFont* pDmgFont = CDamageFont::Create(m_pGraphicDev, 1234567890);
		//NULL_CHECK_RETURN(pDmgFont, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_UI", L"UI_DMG", pDmgFont), -1);
		//pDmgFont->Set_LifeTime(1.5f);
		//pDmgFont->Set_Pos(&vPos);
		////pDmgFont->SetUp_Scaling(0.f, 2.f, 1.f, 10.f);
		//pDmgFont->SetUp_ScalingDelta(0.f, 0.5f, 4.f, -12.f);


		//static _bool bTest = false;

		//if (bTest)
		//	m_pSkySphere->Start_ChangeColor(&_vec4(1.f,0.3f, 0.1f, 1.f));
		//else
		//	m_pSkySphere->Start_ChangeColor(&_vec4(1.f, 1.f, 1.f, 1.f));

		//bTest = !bTest;

		//CUVEffect* pUVEffect = CUVEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_CrackUV", D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		//NULL_CHECK_RETURN(pUVEffect, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Crack", pUVEffect), -1);
		//pUVEffect->Set_DeltaAlpha(-1.f);
		//pUVEffect->Set_LifeTime(1.f);
		//pUVEffect->Set_Scale(10.f);
		//pUVEffect->Set_Pos(&vPos);
		//pUVEffect->Set_Rotation(D3DX_PI * 0.5f, 0.f, 0.f);
		//pUVEffect->Set_UV(&_vec2(0.f, 0.f), &_vec2(1.f, 0.25f));


		//CSparkEffect* pSpark = CSparkEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Spark", D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.f, 0.f), &_vec2(0.5f, 0.5f));
		//NULL_CHECK_RETURN(pSpark, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Spark", pSpark), -1);
		//pSpark->Set_Scale(1.f);
		//pSpark->Set_Pos(&vPos);
		//pSpark->Set_LifeTime(2.f);
		//pSpark->Set_Speed(5.f);

		//CFireEffect* pFire = CFireEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_FireAlpha");
		//NULL_CHECK_RETURN(pFire, -1);
		//Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Fire", pFire);
		//pFire->Set_Scale(1.f);
		//pFire->Set_Pos(&vPos);
		//pFire->Set_LifeTime(1.f);

		//CSphereEffect* pSphereFx = CSphereEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", D3DXCOLOR(1.f, 1.f, 1.f, 1.f)/*, Engine::RENDER_DISTORTION, 5*/);
		//NULL_CHECK_RETURN(pSphereFx, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pSphereFx), -1);
		//pSphereFx->Set_Pos(&vPos);
		//pSphereFx->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(1.f, 1.f, 1.f), 1.f);
		//pSphereFx->Set_LifeTime(1.f);
		//pSphereFx->Set_Alpha(1.f);
		//pSphereFx->Set_DeltaAlpha(-1.f);


		//CGoblin* pGoblin = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin");
		//NULL_CHECK_RETURN(pGoblin, E_FAIL);
		//Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Monster", pGoblin);

		//_vec3 vPos = { 0.f, -10.7519f, 0.f};
		//pGoblin->Set_Pos(&vPos);


		//CGameObject* pObj = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", L"WoodPlatform");
		//NULL_CHECK_RETURN(pObj, E_FAIL);
		//Engine::Add_GameObject(L"Layer_GameLogic", L"WoodPlatform", pObj);

		//_vec3 vPos = { 62.8346f, -8.f, -84.4675f };
		//_vec3 vPos = {0.f, 0.f, 0.f};
		//CUndeadChampion* pUndead = CUndeadChampion::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_UndeadChampion", L"Undead", &vPos);
		//NULL_CHECK_RETURN(pUndead, -1);
		//Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Monster", pUndead);

		
		//wstring wstrBufferTag = L"Mesh_Skeleton";
		//CSkeleton* pSkeleton = CSkeleton::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Skeleton", &_vec3(0.f, 0.f, 0.f));
		//NULL_CHECK_RETURN(pSkeleton, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pSkeleton), -1);

		//CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireNoise", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		//NULL_CHECK_RETURN(pTest, E_FAIL);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		//pTest->Set_Pos(0.f, 5.f, 0.f);
		//pTest->Set_DeltaAlpha(-1.f);
		//pTest->Set_LifeTime(1.f);
		//pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(10.f, 10.f, 10.f), 1.f);

		//CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 100);
		//NULL_CHECK_RETURN(pRockChips, E_FAIL);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), -1);
		//pRockChips->Set_Pos(0.f, 5.f, 0.f);
		//pRockChips->Set_Speed(30.f);
		//pRockChips->Set_Gravity(0.5f);
		//pRockChips->Set_Scale(0.1f);
		//pRockChips->Set_LifeTime(1.f);
	}

	if (Engine::KeyDown(DIK_F2))
	{
		//CFallenDog* pGoblin = CFallenDog::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_FallenDog", L"FallenDog");
		//NULL_CHECK_RETURN(pGoblin, E_FAIL);
		//Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Monster", pGoblin);

		//_vec3 vPos = { 61.f + rand() % 10, -10.7519f, 5.f };
		//pGoblin->Set_Pos(&vPos);
	}

	if (Engine::KeyDown(DIK_F3))
	{
		CHellHound* pGoblin = CHellHound::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_HellHound", L"HellHound");
		NULL_CHECK_RETURN(pGoblin, E_FAIL);
		Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Monster", pGoblin);

		_vec3 vPos = { (rand() % 100) / 10.f, 0.f, (rand() % 100) / 10.f };
		pGoblin->Set_Pos(&vPos);
	}

	if (Engine::KeyDown(DIK_F4))
	{
		_vec3 vPos = { 63.3778f, 0.769858f, -169.072f };
		CMoloch* pMoloch = CMoloch::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Moloch", L"Moloch", &vPos);
		NULL_CHECK_RETURN(pMoloch, E_FAIL);
		Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Boss", pMoloch);

		//pMoloch->Set_NaviIndex(dwIndex);
		pMoloch->Set_Pos(&vPos);
	}

	iExit = Update_CameraAngle(fTimeDelta);

	switch (m_eCurPhase)
	{
	case CStage::S1P_START:
		iExit = Update_Start(fTimeDelta);
		break;
	case CStage::S1P_1_ING:
		iExit = Update_1_Ing(fTimeDelta);
		break;
	case CStage::S1P_1_END:
		iExit = Update_1_End(fTimeDelta);
		break;
	case CStage::S1P_2_START:
		iExit = Update_2_Start(fTimeDelta);
		break;
	case CStage::S1P_2_ING:
		iExit = Update_2_Ing(fTimeDelta);
		break;
	case CStage::S1P_2_END:
		iExit = Update_2_End(fTimeDelta);
		break;
	case CStage::S1P_3_START:
		iExit = Update_3_Start(fTimeDelta);
		break;
	case CStage::S1P_3_ING:
		iExit = Update_3_Ing(fTimeDelta);
		break;
	case CStage::S1P_3_END:
		iExit = Update_3_End(fTimeDelta);
		break;
	case CStage::S1P_4_START:
		iExit = Update_4_Start(fTimeDelta);
		break;
	case CStage::S1P_4_ING:
		iExit = Update_4_Ing(fTimeDelta);
		break;
	case CStage::S1P_4_END:
		iExit = Update_4_End(fTimeDelta);
		break;
	case CStage::S1P_5_START:
		iExit = Update_5_Start(fTimeDelta);
		break;
	case CStage::S1P_5_ING:
		iExit = Update_5_Ing(fTimeDelta);
		break;
	case CStage::S1P_5_END:
		iExit = Update_5_End(fTimeDelta);
		break;
	case CStage::S1P_END:
		iExit = Update_End(fTimeDelta);
		break;
	}

	if (iExit == 1)
		return 1;

	if (iExit & 0x80000000)
		return -1;

	iExit = CScene::Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

_int CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::LateUpdate_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CStage::Render_Scene(const _float& fTimeDelta)
{
	
}

HRESULT CStage::Ready_GameLogic_Layer(const wstring& wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pObj = nullptr;
	Engine::CStaticObject* pStaticObj = nullptr;
	Engine::CDynamicObject* pDynamicObj = nullptr;

	pObj = m_pSkySphere =  CSkySphere::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_SkySphere", L"Sky");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"Sky", pObj);
	m_pSkySphere->AddRef();

	pObj = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"Player", pObj);

	//wstring wstrBufferTag = L"Mesh_Crate_A";
	//pObj = pStaticObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	//NULL_CHECK_RETURN(pObj, E_FAIL);
	//pLayer->Add_GameObject(L"Crate_A", pObj);
	//pStaticObj->Set_Pos(3.f, 0.71196f, 3.f);

	vector<_ulong> vecIndices = { 31, 32 };
	pObj = m_pPhase2Door = CDoor::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Fence", L"Fence", &vecIndices);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"Fence", pObj);
	m_pPhase2Door->Set_Pos(85.1458f, -10.799f, 19.0644f);

	m_pPhase2Door->Get_Transform()->Rotation(Engine::ROT_Y, D3DXToRadian(90.f));

	CWoodPlatform* pWoodPlatform = nullptr;
	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(163.5f, -11.f, -16.8f);

	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(168.2f, -11.f, -16.8f);

	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_B", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(173.5f, -11.7f, -16.2f);
	pWoodPlatform->Set_Angle(0.f, D3DXToRadian(90.f), 0.f);

	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(178.2f, -11.f, -16.8f);


	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_B", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(171.5f, -12.f, -21.f);
	pWoodPlatform->Set_Angle(0.f, D3DXToRadian(90.f), 0.f);

	pObj = pWoodPlatform = CWoodPlatform::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", L"WoodPlatform");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"WoodPlatform", pObj);
	pWoodPlatform->Set_Pos(179.701f, -11.f, -21.f);

	vector<_ulong> vIndices = { 525, 526 };
	m_pPhase5Door = CTricksterDoor::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_TricksterDoor_A", L"TricksterDoor", &vIndices);
	NULL_CHECK_RETURN(m_pPhase5Door, E_FAIL);
	pLayer->Add_GameObject(L"TricksterDoor", m_pPhase5Door);
	m_pPhase5Door->Set_Pos(63.2424f, -10.5836f, -94.3529f);


	//	상자 배치
	wstring wstrBufferTag = L"Mesh_Crate_A";
	CMapObject* pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(16.35f, 0.4f, -2.81f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(43.25f, -10.7f, 24.75f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(94.4f, -10.88f, 9.15f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(146.13f, -11.4f, 27.62f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(189.76f, -10.78f, -18.33f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(147.08f, -10.78f, -38.63f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(71.8f, -10.89f, -47.88f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(51.7442f, -10.99f, -85.17f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(71.27f, -10.86f, -122.52f);

	pMapObj = CMapObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Crate_A");
	NULL_CHECK_RETURN(pMapObj, E_FAIL);
	pLayer->Add_GameObject(L"Crate_A", pMapObj);
	pMapObj->Set_Pos(51.85f, -10.99f, -112.1f);

	//CCircularEffect* pEffect = CCircularEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectTrail", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//NULL_CHECK_RETURN(pEffect, E_FAIL);
	//pLayer->Add_GameObject(L"Obj_Trail", pEffect);
	//pEffect->Set_Pos(0.f, 5.f, 0.f);
	//pEffect->Set_Scale(1.f);
	//pEffect->Set_Angle(D3DX_PI);

	//CDistortionEffect* pDistortion = CDistortionEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireNoise");
	//NULL_CHECK_RETURN(pDistortion, E_FAIL);
	//pLayer->Add_GameObject(L"Obj_Distortion", pDistortion);
	//pDistortion->Set_Pos(0.f, 5.f, 0.f);
	//pDistortion->Set_Scale(1.f);
	//pDistortion->Set_UV(&_vec2(0.f, 0.f), &_vec2(1.f, 1.f));
	//pDistortion->Set_LifeTime(100.f);

	m_mapLayer.insert(make_pair(wstrLayerTag, pLayer));
	
	return S_OK;
}

HRESULT CStage::Ready_Monster_Layer(const wstring & wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert(make_pair(wstrLayerTag, pLayer));

	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const wstring & wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CPlayerUI* pPlayerUI = CPlayerUI::Create(m_pGraphicDev, L"UI_Player");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pLayer->Add_GameObject(L"UI_Player", pPlayerUI);

	m_mapLayer.insert(make_pair(wstrLayerTag, pLayer));

	return S_OK;
}

HRESULT CStage::Ready_Camera()
{
	Engine::CCamera* pCam = CThirdViewCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCam, E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Add_BasicCamera(0, L"Cam_3rd", pCam), E_FAIL);
	
	pCam = CQuaterCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCam, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_BasicCamera(1, L"Cam_Quater", pCam),E_FAIL);

	pCam = CControlCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCam, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_BasicCamera(2, L"Cam_Control", pCam), E_FAIL);

	pCam = CCutSceneCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCam, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_BasicCamera(3, L"Cam_CutScene", pCam), E_FAIL);

	pCam = CQuaterCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCam, E_FAIL);
	pCam->Set_Angle(D3DX_PI * 0.25f, D3DX_PI * 0.75f, 0.f);
	dynamic_cast<CQuaterCam*>(pCam)->Set_Distance(50.f);
	
	FAILED_CHECK_RETURN(Engine::Add_Camera(Engine::CAM_STATIC, L"Cam_Light", pCam), E_FAIL);
	m_pLightCam = pCam;
	pCam->AddRef();

	Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER);

	//FAILED_CHECK_RETURN(Engine::Add_Camera(Engine::CAM_DYNAMIC, L"ControlCam", pCam), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::SetUp_MainCamera(Engine::CAM_DYNAMIC, L"ControlCam"), E_FAIL);

	//CQuaterCam* pCam = CQuaterCam::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pCam, E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Add_Camera(Engine::CAM_DYNAMIC, L"QuaterCam", pCam), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::SetUp_MainCamera(Engine::CAM_DYNAMIC, L"QuaterCam"), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, -1.f);
	tLightInfo.Position = { -10.f, 10.f, 10.f };

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0, D3DX_PI * 0.5f, (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_FAIL);


	return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
	return S_OK;
}

_int CStage::Update_Start(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_EQUALS))
		m_eCurPhase = S1P_1_ING;

	return 0;
}

_int CStage::Update_1_Ing(const _float & fTimeDelta)
{
	auto iter = m_mapLayer.find(L"Layer_Monster");
	if (iter == m_mapLayer.end())
		return -1;

	if ((*iter).second->Get_ObjectMapSize() == 0 && m_iCountForNext > 20)
	{
		m_eCurPhase = S1P_1_END;
		m_iCountForNext = 0;
		return 0;
	}

	m_fPhaseDelay += fTimeDelta;

	if (m_fPhaseDelay > 0.3f)
	{
		m_fPhaseDelay -= 0.3f;

		if ((*iter).second->Get_ObjectMapSize() > 10)
			return 0;

		if (m_iCountForNext > 20)
			return 0;

		CGoblin* pGoblin = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin");
		NULL_CHECK_RETURN(pGoblin, -1);
		Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pGoblin);
		switch (rand() % 4)
		{
		case 0:
			pGoblin->Set_Pos(m_vStartArea[0].x, 
							m_vStartArea[0].y,
							(m_vStartArea[0].z + m_vStartArea[1].z) * 0.5f + ((rand() % 100) * 3.f / 10.f - 15.f));
			pGoblin->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90.f));
			break;
		case 1:
			pGoblin->Set_Pos(m_vStartArea[1].x,
							m_vStartArea[0].y,
							(m_vStartArea[0].z + m_vStartArea[1].z) * 0.5f + ((rand() % 100) * 3.f / 10.f - 15.f));
			pGoblin->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(-90.f));
			break;
		case 2:
			pGoblin->Set_Pos((m_vStartArea[0].x + m_vStartArea[1].x) * 0.5f + ((rand() % 100) * 3.f / 10.f - 15.f),
							m_vStartArea[0].y,
							m_vStartArea[0].z );
			break;
		case 3:
			pGoblin->Set_Pos((m_vStartArea[0].x + m_vStartArea[1].x) * 0.5f + ((rand() % 100) * 3.f / 10.f - 15.f),
							m_vStartArea[0].y,
							m_vStartArea[1].z);
			pGoblin->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(180.f));
			break;
		}

		++m_iCountForNext;
	}

	return 0;
}

_int CStage::Update_1_End(const _float & fTimeDelta)
{
	_ulong dwPlayerNaviIndex = 0;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwPlayerNaviIndex);

	if (dwPlayerNaviIndex == 10 || dwPlayerNaviIndex == 11)
		m_eCurPhase = S1P_2_START;

	return 0;
}

_int CStage::Update_2_Start(const _float & fTimeDelta)
{
	m_eCurPhase = S1P_2_ING;

	m_fPhaseDelay = 0.f;
	m_iCountForNext = 0;

	m_vStartArea[0] = { 60.f, -10.3397f, 5.16173f };
	m_vStartArea[1] = { 70.6082f, -10.3268f, 33.8304f };

	
	return 0;
}

_int CStage::Update_2_Ing(const _float & fTimeDelta)
{
	auto iter = m_mapLayer.find(L"Layer_Monster");
	if (iter == m_mapLayer.end())
		return -1;

	if ((*iter).second->Get_ObjectMapSize() == 0 && m_iCountForNext > 20)
	{
		m_eCurPhase = S1P_2_END;
		m_iCountForNext = 0;
		return 0;
	}

	m_fPhaseDelay += fTimeDelta;

	if (m_fPhaseDelay > 0.3f)
	{
		m_fPhaseDelay -= 0.3f;

		if ((*iter).second->Get_ObjectMapSize() > 20)
			return 0;

		if (m_iCountForNext > 20)
			return 0;

		_int iRand = rand() % 2;
		
		CMonster* pMonster = nullptr;
		switch (iRand)
		{
		case 0:
			pMonster = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin");
			break;
		case 1:
			pMonster = CFallenDog::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_FallenDog", L"FallenDog");
			break;
		}
		NULL_CHECK_RETURN(pMonster, -1);
		Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pMonster);
		switch (iRand)
		{
		case 0:
			pMonster->Set_Pos((m_vStartArea[0].x + m_vStartArea[1].x) * 0.5f + ((rand() % 100) * 2.f / 10.f - 10.f),
								m_vStartArea[0].y,
								m_vStartArea[0].z);
			break;
		case 1:
			pMonster->Set_Pos((m_vStartArea[0].x + m_vStartArea[1].x) * 0.5f + ((rand() % 100) * 2.f / 10.f - 10.f),
								m_vStartArea[0].y,
								m_vStartArea[1].z);
			pMonster->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(180.f));
			break;
		}

		++m_iCountForNext;
	}
	

	return 0;
}

_int CStage::Update_2_End(const _float & fTimeDelta)
{
	m_pPhase2Door->Open();
	m_eCurPhase = S1P_3_START;

	return 0;
}

_int CStage::Update_3_Start(const _float & fTimeDelta)
{
	_ulong dwPlayerNaviIndex = 0;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwPlayerNaviIndex);

	if (dwPlayerNaviIndex == 51 || dwPlayerNaviIndex == 52 || dwPlayerNaviIndex == 53 || dwPlayerNaviIndex == 54 || dwPlayerNaviIndex == 55)
	{
		m_eCurPhase = S1P_3_ING;
		m_pPhase3Portal = CPortal::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_PortalMask", _vec4(0.f, 0.f, 0.f, 1.f));
		NULL_CHECK_RETURN(m_pPhase3Portal, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Portal", m_pPhase3Portal), E_FAIL);
	}

	return 0;
}

_int CStage::Update_3_Ing(const _float & fTimeDelta)
{
	if (m_pPhase3Portal->Get_State() == CPortal::EPortal::CLOSING)
	{
		m_eCurPhase = S1P_3_END;
		
		m_fPhaseDelay = 0.f;
		m_iCountForNext = 0;

		m_vecGoblins.reserve(10);
		for (_int i = 0; i < 10; ++i)
		{
			CGoblin* pGoblin = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin", CGoblin::EGoblin::SIT_IDLE);
			NULL_CHECK_RETURN(pGoblin, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pGoblin), E_FAIL);
			pGoblin->Set_Pos(173.f + ((rand() % 100) - 50.f) / 10.f, -6.8f, -17.f + ((rand() % 100) - 50.f) / 30.f);
			m_vecGoblins.push_back(pGoblin);
		}
	}


	return 0;
}

_int CStage::Update_3_End(const _float & fTimeDelta)
{
	_ulong dwIndex;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwIndex);

	if (dwIndex == 95 || dwIndex == 94)
		m_eCurPhase = S1P_4_START;


	return 0;
}

_int CStage::Update_4_Start(const _float & fTimeDelta)
{
	if (m_iCountForNext >= 10)
	{
		_ulong dwIndex;
		m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwIndex);

		if (dwIndex == 108 || dwIndex == 109 || dwIndex == 114 || dwIndex == 115)
		{
			m_eCurPhase = S1P_4_ING;

			m_vStartArea[0] = { 55.2409f, -10.779f, -38.1254f };
			m_vStartArea[1] = { 98.1884f, -10.779f, -24.1188f };

			CMonster* pMonster = nullptr;
			for (_int j = 0; j < 2; ++j)
			{
				for (_int i = 0; i < 3; ++i)
				{
					pMonster = CFallenDog::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_FallenDog", L"FallenDog");
					NULL_CHECK_RETURN(pMonster, -1);
					FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pMonster), -1);
					pMonster->Set_Pos(m_vStartArea[0].x - 3.f + 15.f * j, m_vStartArea[0].y, m_vStartArea[0].z + 5.f * i);
					pMonster->Set_AngleY(D3DXToRadian(90.f));
				}

				for (_int i = 0; i < 3; ++i)
				{
					pMonster = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin");
					NULL_CHECK_RETURN(pMonster, -1);
					FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pMonster), -1);
					pMonster->Set_Pos(m_vStartArea[0].x + 5.f + 15.f * j, m_vStartArea[0].y, m_vStartArea[0].z + 5.f * i);
					pMonster->Set_AngleY(D3DXToRadian(90.f));
				}

				for (_int i = 0; i < 3; ++i)
				{
					pMonster = CHellHound::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_HellHound", L"HellHound");
					NULL_CHECK_RETURN(pMonster, -1);
					FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pMonster), -1);
					pMonster->Set_Pos(m_vStartArea[0].x + 10.f + 15.f * j, m_vStartArea[0].y, m_vStartArea[0].z + 5.f * i);
					pMonster->Set_AngleY(D3DXToRadian(90.f));
				}
			}
		}

		return 0;
	}
	m_fPhaseDelay += fTimeDelta;

	if (m_fPhaseDelay >= 0.3f)
	{
		m_fPhaseDelay -= 0.3f;
		m_vecGoblins[m_iCountForNext]->Sit_End();
		++m_iCountForNext;
	}
	return 0;
}

_int CStage::Update_4_Ing(const _float & fTimeDelta)
{
	auto iter = m_mapLayer.find(L"Layer_Monster");

	if (m_mapLayer.end() == iter)
		return -1;

	if (iter->second->Get_ObjectMapSize() == 0)
	{
		m_eCurPhase = S1P_4_END;
	}



	return 0;
}

_int CStage::Update_4_End(const _float & fTimeDelta)
{
	_ulong dwIndex;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwIndex);

	if (145 == dwIndex || 428 == dwIndex || 412 == dwIndex)
	{
		m_eCurPhase = S1P_5_START;
		m_bUndeadSpawn = true;
		_vec3 vPos = { 62.8346f, -8.f, -84.4675f };
		CUndeadChampion* pUndead = CUndeadChampion::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_UndeadChampion", L"Undead", &vPos);
		NULL_CHECK_RETURN(pUndead, -1);
		Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pUndead);
	}

	return 0;
}

_int CStage::Update_5_Start(const _float & fTimeDelta)
{
	auto iter = m_mapLayer.find(L"Layer_Monster");

	if (iter == m_mapLayer.end())
		return -1;

	if (m_bUndeadSpawn && iter->second->Get_ObjectMapSize() == 0)
	{
		if (m_pPhase5Door)
		{
			m_pPhase5Door->Open();
			m_eCurPhase = S1P_5_ING;
		}
	}

	return 0;
}

_int CStage::Update_5_Ing(const _float & fTimeDelta)
{
	_ulong dwIndex;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwIndex);

	if (dwIndex == 157 || dwIndex == 158 || dwIndex == 159 || dwIndex == 184 || dwIndex == 185)
	{
		m_eCurPhase = S1P_5_END;

		m_pSkySphere->Start_ChangeColor(&_vec4(1.f, 0.3f, 0.1f, 0.f));


		_vec3 vPos = { 63.3778f, 0.769858f, -169.072f };
		CMoloch* pMoloch = CMoloch::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Moloch", L"Moloch", &vPos);
		NULL_CHECK_RETURN(pMoloch, E_FAIL);
		Engine::Add_GameObject(L"Layer_Monster", L"Obj_Boss", pMoloch);
		pMoloch->Set_NaviIndex(dwIndex);
		pMoloch->Set_Pos(&vPos);

		

		return 0;
	}

	

	return 0;
}

_int CStage::Update_5_End(const _float & fTimeDelta)
{
	auto iter = m_mapLayer.find(L"Layer_Monster");

	if (m_mapLayer.end() == iter)
		return -1;

	if (iter->second->Get_ObjectMapSize() == 0)
	{
		CEndingScene* pScene = CEndingScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);
		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), -1);

		return 1;
	}

	return 0;
}

_int CStage::Update_End(const _float & fTimeDelta)
{


	return 0;
}

_int CStage::Update_CameraAngle(const _float & fTimeDelta)
{
	_ulong dwPlayerIndex = 0;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::NAVI_INDEX), &dwPlayerIndex);

	if ((dwPlayerIndex >= 50 && dwPlayerIndex <= 55) || 365 == dwPlayerIndex)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(45.f), D3DXToRadian(0.f)));
		
	}
	else if ((108 <= dwPlayerIndex && dwPlayerIndex <= 111) || 115 == dwPlayerIndex)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(0.f), D3DXToRadian(0.f)));

	}
	else if (91 <= dwPlayerIndex && dwPlayerIndex <= 93)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
		{
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(90.f), D3DXToRadian(0.f)));
			pQuaterCam->Change_Distance(20.f);
		}

	}
	else if (132 <= dwPlayerIndex && dwPlayerIndex <= 136)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(-90.f), D3DXToRadian(0.f)));
	}
	else if (141 <= dwPlayerIndex && dwPlayerIndex <= 144)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(-135.f), D3DXToRadian(0.f)));
	}
	else if (145 == dwPlayerIndex || 412 == dwPlayerIndex || 428 == dwPlayerIndex)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Set_Rotation(&_vec3(D3DXToRadian(45.f), D3DXToRadian(-180.f), D3DXToRadian(0.f)));
	}
	else if (155 == dwPlayerIndex ||156 == dwPlayerIndex || 491 == dwPlayerIndex || 492 == dwPlayerIndex)
	{
		CQuaterCam* pQuaterCam = dynamic_cast<CQuaterCam*>(Engine::Get_MainCamera());
		if (pQuaterCam)
			pQuaterCam->Change_Distance(25.f);
	}



	return 0;
}


CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInst = new CStage(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CStage::Free()
{
	::Safe_Release(m_pSkySphere);
	::Safe_Release(m_pLightCam);
	::Safe_Release(m_pPlayerObserver);
	CScene::Free();
}

