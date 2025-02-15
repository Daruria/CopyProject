#pragma once

#include "Engine_Defines.h"

// FMOD: 유니티, 언리얼과 같은 상용 엔진에도 두루 쓰이는 사운드 관련 엔진.
// 사운드에 관련된 인터페이스를 제공.
#include "fmod.h"
#pragma comment(lib, "fmodL_vc.lib")

#include <io.h>


BEGIN(Engine)

class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	enum CHANNEL_ID { BGM, PLAYER, PLAYER_LOOP, MONSTER, EFFECT, EFFECT_LOOP, MAX_CHANNEL };

public:
	HRESULT Initialize();
	HRESULT LoadSoundFile(const string& strSoundPath);
	void Update_Sound();
	void Play_Sound(const wstring& wstrSoundKey, CHANNEL_ID eID);
	void Play_BGM(const wstring& wstrSoundKey);
	void Stop_Sound(CHANNEL_ID eID);
	void Stop_All();
	void Set_Volume(CHANNEL_ID eID, float fVol); // 볼륨의 범위는 최소 0.f ~ 최대 1.f까지.
	void Release();

private:
	void LoadSoundFile();

private:
	// FMOD_SOUND: 사운드 파일에 대한 정보를 갖는 구조체.
	unordered_map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: 사운드를 재생하고 관리하는 역할. ex) 볼륨 조절 등.
	FMOD_CHANNEL*	m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND과 FMOD_CHANNEL을 총괄 관리하는 객체.
	FMOD_SYSTEM*	m_pSystem;
};

END