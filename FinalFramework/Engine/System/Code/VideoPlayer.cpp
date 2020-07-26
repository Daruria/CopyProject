#include "VideoPlayer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CVideoPlayer)

CVideoPlayer::CVideoPlayer()
{
}

CVideoPlayer::~CVideoPlayer()
{
	Free();
}

HRESULT CVideoPlayer::Ready_VideoPlayer()
{
	FAILED_CHECK_RETURN(m_BVLibrary.Create(BANDIVIDEO_DLL_FILE_NAME, NULL, NULL), E_FAIL);


	return S_OK;
}

void CVideoPlayer::Play_Video(LPDIRECT3D9 pGraphic, LPDIRECT3DDEVICE9 pGrahpicDev, HWND hWnd, const wstring& wstrFilePath)
{
	FAILED_CHECK_RETURN(m_BVLibrary.Open(wstrFilePath.c_str(), FALSE), );
	
	BV_DEVICE_DX9 BV_Device = { pGraphic, pGrahpicDev, hWnd };

	m_BVLibrary.GetVideoInfo(m_Info);

	m_pBVTexture = new CBandiVideoTexture_DX9(&BV_Device);
	m_pBVTexture->Open(m_Info.width, m_Info.height);
	m_BVLibrary.Play();
	m_bOpen = true;
}

void CVideoPlayer::Stop_Video()
{
	m_BVLibrary.Stop();
	m_BVLibrary.Close();
	m_bOpen = false;

	if (m_pBVTexture)
	{
		m_pBVTexture->Close();
		Safe_Delete(m_pBVTexture);
	}
}

void CVideoPlayer::Render_Video()
{
	BVL_STATUS rStatus;

	m_BVLibrary.GetStatus(rStatus);
	
	if (rStatus == BVL_STATUS_PLAYEND)
	{
		if (!m_bLoop)
		{
			Stop_Video();
		}
		return;
	}

	//if (m_BVLibrary.IsNextFrame())
	{
		INT pitch;

		BYTE* buf = m_pBVTexture->Lock(pitch);
		if (buf)
		{
			BVL_FRAME	BVL_Frame;
			BVL_Frame.frame_buf = buf;
			BVL_Frame.frame_buf_size = m_Info.height * pitch;
			BVL_Frame.pitch = pitch;
			BVL_Frame.width = m_Info.width;
			BVL_Frame.height = m_Info.height;
			BVL_Frame.pixel_format = BVLPF_X8R8G8B8;

			m_BVLibrary.GetFrame(BVL_Frame, TRUE);

			m_pBVTexture->Unlock();

			m_pBVTexture->Draw(0, 0, m_Info.width, m_Info.height);
		}
	}
}

HRESULT CVideoPlayer::Get_VideoStatus(BVL_STATUS& rStatus)
{
	return m_BVLibrary.GetStatus(rStatus);
}

_bool CVideoPlayer::Is_CurVideoEnd()
{
	BVL_STATUS rStatus;

	FAILED_CHECK_RETURN(m_BVLibrary.GetStatus(rStatus), false);

	return rStatus == BVL_STATUS_PLAYEND;
}

void CVideoPlayer::Free()
{
	if (m_bOpen)
	{
		m_BVLibrary.Close();
	}

	m_BVLibrary.Destroy();

	if (m_pBVTexture)
	{
		m_pBVTexture->Close();
		Safe_Delete(m_pBVTexture);
	}
}
