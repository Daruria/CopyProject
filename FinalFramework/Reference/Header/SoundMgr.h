#pragma once

#include "Engine_Defines.h"

// FMOD: ����Ƽ, �𸮾�� ���� ��� �������� �η� ���̴� ���� ���� ����.
// ���忡 ���õ� �������̽��� ����.
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
	void Set_Volume(CHANNEL_ID eID, float fVol); // ������ ������ �ּ� 0.f ~ �ִ� 1.f����.
	void Release();

private:
	void LoadSoundFile();

private:
	// FMOD_SOUND: ���� ���Ͽ� ���� ������ ���� ����ü.
	unordered_map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: ���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_CHANNEL*	m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.
	FMOD_SYSTEM*	m_pSystem;
};

END