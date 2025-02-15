#include "SoundMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSoundMgr);
 
CSoundMgr::CSoundMgr()
{

}

CSoundMgr::~CSoundMgr()
{
	Release();
}

HRESULT CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, 100, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();

	return S_OK;
}

void CSoundMgr::Update_Sound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Play_Sound(const wstring& wstrSoundKey, CHANNEL_ID eID)
{
	auto iter_find = m_MapSound.find(wstrSoundKey);

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, iter_find->second, nullptr,
		FALSE, &m_pChannelArr[eID]);

	if (eID == CSoundMgr::EFFECT_LOOP || eID == CSoundMgr::PLAYER_LOOP)
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);

	Update_Sound();
}

void CSoundMgr::Play_BGM(const wstring& wstrSoundKey)
{
	auto iter_find = m_MapSound.find(wstrSoundKey);

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, iter_find->second, nullptr,
		FALSE, &m_pChannelArr[BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);

	Update_Sound();
}

void CSoundMgr::Stop_Sound(CHANNEL_ID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
	Update_Sound();
}

void CSoundMgr::Stop_All()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);

	Update_Sound();
}

void CSoundMgr::Set_Volume(CHANNEL_ID eID, float fVol)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVol);
	Update_Sound();
}

void CSoundMgr::Release()
{
	for_each(m_MapSound.begin(), m_MapSound.end(),
		[](auto& MyPair)
	{
		FMOD_Sound_Release(MyPair.second);
	});

	m_MapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	intptr_t handle = _findfirst("../Bin/Sound/*.*", &fd);

	if (0 == handle)
	{
		MessageBox(NULL, L"_findfirst failed!", L"Failed!!", MB_OK);
		return;
	}

	char szFullPath[128] = "";
	char szRelativePath[128] = "../Bin/Sound/";

	int iResult = 0;

	while (-1 != iResult)
	{
		// szFullPath: "../Sound/"
		strcpy_s(szFullPath, szRelativePath);

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		// szFullPath: "../Sound/Adam Levine - Lost Stars Lyrics.mp3"
		strcat_s(szFullPath, fd.name);

		if (0 == strcmp(fd.name, ".") || 0 == strcmp(fd.name, ".."))
		{
			iResult = _findnext(handle, &fd);
			continue;
		}

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		TCHAR* pSoundKey = new TCHAR[strlen(fd.name) + 1];

		// 멀티 -> 와이드로 변환.
		MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name) + 1,
			pSoundKey, strlen(fd.name) + 1);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath,
			FMOD_DEFAULT, nullptr, &pSound);

		if (FMOD_OK == eResult)
		{
			auto& iter_find = m_MapSound.find(pSoundKey);

			if (m_MapSound.end() == iter_find)
			{
				m_MapSound.insert({ pSoundKey, pSound });
				delete[] pSoundKey;
			}
			else
			{
				delete[] pSoundKey;
				FMOD_Sound_Release(pSound);
			}
		}
		else
			delete[] pSoundKey;

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
}

HRESULT CSoundMgr::LoadSoundFile(const string & strSoundPath)
{
	string strPath = strSoundPath;

	strPath += "*.*";

	_finddata_t fd;

	intptr_t handle = _findfirst(strPath.c_str(), &fd);

	if (0 == handle)
	{
		MessageBox(NULL, L"_findfirst failed!", L"Failed!!", MB_OK);

		return E_FAIL;
	}

	char szFullPath[128] = "";
	//char szRelativePath[128] = strSoundPath.c_str();

	int iResult = 0;

	while (-1 != iResult)
	{
		// szFullPath: "../Sound/"
		strcpy_s(szFullPath, strSoundPath.c_str());

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		// szFullPath: "../Sound/Adam Levine - Lost Stars Lyrics.mp3"
		strcat_s(szFullPath, fd.name);

		if (0 == strcmp(fd.name, ".") || 0 == strcmp(fd.name, ".."))
		{
			iResult = _findnext(handle, &fd);
			continue;
		}

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		TCHAR* pSoundKey = new TCHAR[strlen(fd.name) + 1];

		// 멀티 -> 와이드로 변환.
		MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name) + 1,
			pSoundKey, strlen(fd.name) + 1);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath,
			FMOD_DEFAULT, nullptr, &pSound);

		if (FMOD_OK == eResult)
		{
			auto& iter_find = m_MapSound.find(pSoundKey);

			if (m_MapSound.end() == iter_find)
			{
				m_MapSound.insert({ pSoundKey, pSound });
				delete[] pSoundKey;
			}
			else
			{
				delete[] pSoundKey;
				FMOD_Sound_Release(pSound);
			}
		}
		else
			delete[] pSoundKey;

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	return S_OK;
}
