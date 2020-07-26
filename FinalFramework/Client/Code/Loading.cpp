#include "stdafx.h"
#include "Loading.h"
#include "StaticObject.h"
#include "Layer.h"

#include "Export_Function.h"
#include "PlayerPool.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();		
		break;
	case LOADING_BOSS:
		break;
	case LOADING_CLEAR:
		iFlag = pLoading->Loading_Clear();
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	// buffer
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
	//											RESOURCE_STATIC,
	//											L"Buffer_TerrainTex",
	//											Engine::BUFFER_TERRAINTEX,
	//											VTXCNTX,
	//											VTXCNTZ,
	//											VTXITV),
	//											E_FAIL);

	m_wstrLoadingText = L"버퍼 준비 중 ( 1 / 5 )";

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												Engine::RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);

	
	
	// 텍스쳐
	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//											RESOURCE_STAGE,
	//											L"Texture_Terrain",
	//											Engine::TEX_NORMAL,
	//											L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
	//											E_FAIL);


	// Stone
	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//											RESOURCE_STAGE,
	//											L"Mesh_Stone", 
	//											Engine::TYPE_STATIC, 
	//											L"../Bin/Resource/Mesh/StaticMesh/TombStone/",
	//											L"TombStone.X"),
	//											E_FAIL);

	/*FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/",
		L"Player.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Sword",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Sword/",
		L"Sword.X"),
		E_FAIL);*/

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navi",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL),
	//	E_FAIL);

	//	Ready Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Buffer_Trail", Engine::BUFFER_TRAIL, 400), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Buffer_CircleTex", Engine::BUFFER_CIRCLETEX, 300), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Buffer_CircularTrail", Engine::BUFFER_CIRCULAR_TRAIL, 1000, 0.5f, 1.f), E_FAIL);

	m_wstrLoadingText = L"텍스쳐 준비 중 ( 2 / 5 )";
	//	Ready Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_HPBar", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/HUD_UnitFrame_Main_HealthMeterFill.png"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_HPBarFrame", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/HUD_UnitFrame_Main_HealthBaseC1b.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_BossHPBarFrame", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/UI_HUD_BossHealthFrame1.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_BossHPBar", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/HUD_UnitFrame_Main_HealthMeterFill_O.tga"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_PortraitFrame", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/HUD_UnitFrame_Sub_CharArtBaseC1.tga"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_WarPortrait", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/UI/HUD_UnitFrame_Sub_CharArtWar.tga"), E_FAIL);
	
	//	DamageFont Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_DamageFont", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/DamageFont/Font.png"), E_FAIL);

	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SwordTrail", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Wave_A_03B_COLOR.tga"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_DashTrail", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Beam_Strife_TEST_01_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectTrail", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Swipe_Stylized_RGB_01.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Swipe", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Swipe_A_05_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Trail2", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_RGB_Egg_01.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Wave_01_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectDot", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Dot_A_01.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/SubUV/T_RockChips_01_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/SubUV/T_Clouds_SubUV_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Portal", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Noise/T_Noise_Cells.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Dissolve_Bits", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Noise/T_Noise_Cells_RGB.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Dissolve", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/DissolveFX/T_DissolveMask_A.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Spark", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/SubUV/VFX_gen_energyBolts.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Noise/T_Fire_Tiled_D.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireNoise", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Noise/T_Noise_Swirl.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireAlpha", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/SubUV/VFX_gen_fire.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_PortalMask", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Beam_Strife_TEST_01_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_CrackUV", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/SubUV/T_Astarte_GroundCracks_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Noise/T_Noise_Cells_COLOR.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Radial_Mask", Engine::TEX_NORMAL,
											L"../Bin/Resources/Texture/Masks/T_Mask_Radial_Point_COLOR.tga"), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Player", Engine::TYPE_DYNAMIC,
	//										L"../Bin/Resources/Mesh/DynamicMesh/Test/", L"Strife.X"), E_FAIL);

	m_wstrLoadingText = L"메쉬 준비 중 ( 3 / 5 )";

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_SkySphere", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Sky/", L"SkyDome.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_WarSword", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/WarSword/", L"WarSword.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_War", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Player/War/", L"War.X"), E_FAIL);

	CPlayerPool::GetInstance()->Ready_PlayerPool(m_pGraphicDev, L"Dummy_War", L"../Bin/Resources/Mesh/DynamicMesh/Player/War/", L"War.X", 5, TS_RENDEROFFSET);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Fortress_Relief_CombineC", Engine::TYPE_STATIC,
	//										L"../Bin/Resources/Mesh/StaticMesh/Hell/Custom/", L"Fortress_Relief_CombineC.X"), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rocks_Circle_A", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/", L"Dn_VFX_Rocks_Circle_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_0", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/", L"Dn_VFX_Rocks_Single_D.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_1", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/", L"Dn_VFX_Rocks_Single_J.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Dn_VFX_Rocks_Cluster", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/", L"Dn_VFX_Rocks_Cluster_C.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Dn_VFX_Rocks_WholeCluster", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/", L"Dn_VFX_Rocks_WholeCluster_A.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"DisCrystal", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"DisCrystal.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Dn_HL_TremorCrystal_A", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Dn_HL_TremorCrystal_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Dn_HL_TremorCrystal_B", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Dn_HL_TremorCrystal_B.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Crystal_Fragments_00", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Crystal_Fragments_00.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Crystal_Fragments_01", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Crystal_Fragments_01.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Crystal_Fragments_02", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Crystal_Fragments_02.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Crystal_Fragments_03", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crystal/", L"Crystal_Fragments_03.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Crate_A", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Crate/", L"Crate_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_UndeadChampion", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/UndeadChampion/", L"UndeadChampion.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_UndeadShield", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/UndeadChampion/", L"UndeadChampion_Shield.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_UndeadSpear", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/UndeadChampion/", L"UndeadChampion_Spear.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/Goblin/", L"Goblin.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_GoblinSword", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/Goblin/", L"Goblin_Sword.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_FallenDog", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/FallenDog/", L"FallenDog.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_HellHound", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/HellHound/", L"HellHound.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Moloch", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/Moloch/", L"Moloch.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_MolochSword", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/Moloch/", L"MolochSword.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_MolochSwordCrystal", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/Moloch/", L"MolochSwordCrystal.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Fence", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Fence/", L"Dn_HL_Fence_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_TricksterDoor_A", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/TricksterDoor_A/", L"Dn_TricksterDoor_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_WoodPlanks_BeamBroken_A.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood1", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_WoodPlanks_BeamBroken_B.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood2", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_WoodChunck_A.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood3", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_WoodChunck_B.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood4", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_WoodChunck_C.X"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_BrokenWood5", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"Dn_HL_Wood_Board_D.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_A", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"WoodPlatform_A_VA.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_WoodPlatform_B", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Wood/", L"WoodPlatform_B_VA.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Skeleton", Engine::TYPE_DYNAMIC,
											L"../Bin/Resources/Mesh/DynamicMesh/Monster/Skeleton/", L"Skeleton.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_SkeletonSword", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Weapon/Skeleton/", L"Skeleton_Sword_A.X"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Fx_Sphere", Engine::TYPE_STATIC,
											L"../Bin/Resources/Mesh/StaticMesh/Effect/Sphere/", L"Fx_Sphere.X"), E_FAIL);

	m_wstrLoadingText = L"맵 준비 중 ( 4 / 5 )";

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Navi", Engine::TYPE_NAVI,
		L"../Bin/Data/", L"CellOptionTest9_1.dat"), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Navi", Engine::TYPE_NAVI,
	//										L"../Bin/Data/", L"ForTest.dat"), E_FAIL);

	FAILED_CHECK_RETURN(Load_MapData(L"../Bin/Data/", L"Map1_6.dat"), E_FAIL);

	m_wstrLoadingText = L"사운드 준비 중 ( 5 / 5 )";

	FAILED_CHECK_RETURN(Engine::LoadSoundFile("../Bin/Sound/Stage/"), E_FAIL);

	m_wstrLoadingText = L"준비 완료";

	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_Clear()
{
	Engine::Clear_Resource(Engine::RESOURCE_STATIC);
	Engine::Clear_Resource(Engine::RESOURCE_LOGO);
	Engine::Clear_Resource(Engine::RESOURCE_STAGE);

	Engine::Clear_Camera();
	Engine::Clear_Subject();

	m_bFinish = true;

	return 0;
}

HRESULT CLoading::Load_MapData(const wstring & wstrFilePath, const wstring & wstrFileName)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	_tchar szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	HANDLE hFile = ::CreateFile(szFullPath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;

	_ulong dwSize = 0;
	ReadFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	for (_ulong i = 0; i < dwSize; ++i)
	{
		_ulong dwLength = 0;
		_tchar szRead[STR_MAX] = L"";
		ReadFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwLength, &dwBytes, nullptr);

		wstring wstrFirst = szRead;

		ZeroMemory(szRead, sizeof(_tchar) * STR_MAX);
		ReadFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwLength, &dwBytes, nullptr);
		wstring wstrSecond = szRead;

		if (!Engine::Get_Resources(Engine::RESOURCE_STAGE, wstrFirst))
		{
			FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrFirst, 
				Engine::TYPE_STATIC, wstrSecond, wstrFirst), E_FAIL);
		}
	}

	ReadFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	for (_ulong i = 0; i < dwSize; ++i)
	{
		_vec3 vScale, vPos, vAngle;

		_tchar szRead[STR_MAX] = L"";
		_ulong dwNameLength = 0;

		ReadFile(hFile, &vScale, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &dwNameLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwNameLength, &dwBytes, nullptr);
		wstring wstrName = szRead;

		CStaticObject* pObj = CStaticObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrName, wstrName);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		CTransform* pTransform = pObj->Get_Transform();
		pTransform->Set_Pos(vPos);
		pTransform->Set_Angle(vAngle);
		pTransform->Set_Scale(vScale);

		pLayer->Add_GameObject(wstrName, pObj);
	}

	CloseHandle(hFile);

	Engine::Add_LoadingLayer(L"Layer_Environment", pLayer);

	return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}


