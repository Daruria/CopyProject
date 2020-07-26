#include "stdafx.h"
#include "Conversation.h"
#include "SelectBox.h"

CConversation::CConversation():
	m_bTalkingEnd(false),
	m_iTalkWord(0),
	m_iTextLine(0),
	m_dwTalkingTime(0),
	m_dwTalkingSpeed(0),
	m_ePreState(END),
	m_eCurState(END)
{
}


CConversation::~CConversation()
{
	Release();
}

void CConversation::Initialize()
{
	CUIWindow::Initialize();
	//m_vecText.push_back("You're here! are you ready to train?");



	m_iTextLine = 0;
	m_dwTalkingSpeed = 50;
}

void CConversation::Initialize(const string & strKey, const wchar_t * pPath)
{
}

int CConversation::Update()
{
	if (!m_bTalkingEnd && m_dwTalkingTime + m_dwTalkingSpeed <= GetTickCount())
	{
		if (m_iTalkWord < (int)m_vecText[m_iTextLine].size())
		{
			++m_iTalkWord;
			GET_SINGLE(CSoundMgr)->PlaySound(L"PickUpItem.wav", CSoundMgr::EFFECT);
		}
		else
			m_bTalkingEnd = true;
		m_dwTalkingTime = GetTickCount();
	}

	if (m_bTalkingEnd)
	{
		for (auto pSelectBox : m_SelectList)
		{
			if (pSelectBox->Update())
			{

			}
		}
	}

	return 0;
}

void CConversation::LateUpdate()
{
}

void CConversation::Render(HDC hDC)
{
	TalkBoxRender(hDC, static_cast<float>(m_tPoint.x), static_cast<float>(m_tPoint.y), m_tSize.cx, m_tSize.cy, 4.f, LEFT, LEFT);

	GET_SINGLE(CFontManager)->Render(hDC, m_tPoint.x + 30.f, m_tPoint.y + 20.f, FT_NORMAL, 3.f, m_strKey, FPX_LEFT);

	string strText = m_vecText[m_iTextLine].substr(0, m_iTalkWord);
	GET_SINGLE(CFontManager)->Render(hDC, m_tPoint.x + 30.f, m_tPoint.y + 70.f, FT_NORMAL, 2.f, strText, FPX_LEFT);
	
	int iChoiceSize = 30 + m_SelectList.size() * 30;
	int iChoiceY = m_tPoint.y - iChoiceSize;


	if (m_bTalkingEnd)
	{
		TalkBoxRender(hDC, 
			static_cast<float>(WINCX), static_cast<float>(m_tPoint.y), 
			static_cast<int>(m_tSelectBoxInfo.icx), 
			static_cast<int>(m_tSelectBoxInfo.icy), 4.f, RIGHT, RIGHT);
		
		for (auto pSelectBox : m_SelectList)
			pSelectBox->Render(hDC);
	}
}

void CConversation::Release()
{
	for (auto pSelectBox : m_SelectList)
		SAFE_DELETE(pSelectBox);
	m_SelectList.clear();
}

void CConversation::AddTextLine(const string & strText)
{
	string str = strText;
	m_vecText.push_back(str);
}

void CConversation::AddSelectBox(CSelectBox * pSelectBox)
{
	int iTextSizeX = pSelectBox->GetSizeX();

	int iCY = 0;

	for (auto pBox : m_SelectList)
	{
		iCY += static_cast<int>(pBox->GetSizeY() + 2 * m_fRatio);

		if (pBox->GetSizeX() > iTextSizeX)
		{
			iTextSizeX = pBox->GetSizeX();
		}
	}

	m_tSelectBoxInfo.icx = static_cast<int>(iTextSizeX + 40 * m_fRatio);
	m_tSelectBoxInfo.icy = static_cast<int>(pSelectBox->GetSizeY() + iCY + 40 * m_fRatio);

	for (auto pBox : m_SelectList)
	{
		pBox->AddPoint(0, -static_cast<int>(pSelectBox->GetSizeY() + 2 * m_fRatio));
		pBox->SetPoint(WINCX - m_tSelectBoxInfo.icx / 2, pBox->GetPoint().y);
	}


	m_tSelectBoxInfo.fx = static_cast<float>(m_tSize.cx - m_tSelectBoxInfo.icx / 2);
	m_tSelectBoxInfo.fy = static_cast<float>(m_tPoint.y - m_tSelectBoxInfo.icy / 2);

	pSelectBox->SetPoint(WINCX - m_tSelectBoxInfo.icx / 2, static_cast<int>(m_tPoint.y - pSelectBox->GetSizeY() / 2 - 20 * m_fRatio));

	m_SelectList.push_back(pSelectBox);
}
