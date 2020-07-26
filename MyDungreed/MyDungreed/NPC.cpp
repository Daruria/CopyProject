#include "stdafx.h"
#include "NPC.h"
#include "Player.h"


CNPC::CNPC():
	m_bDir(true),
	m_bCanInterAct(false),
	m_bIsOpen(false)
{
}


CNPC::~CNPC()
{
	Release();
}

void CNPC::Initialize()
{
}

int CNPC::Update()
{
	return 0;
}

void CNPC::LateUpdate()
{
}

void CNPC::Render(HDC hDC)
{
}

void CNPC::Release()
{
}

void CNPC::Interaction()
{
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	if (!pPlayer)
		return;
	

}
