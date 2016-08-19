#include "_StdAfx.h"

bool		GGameSingle::Frame() {
	if (g_pMain->m_nGameBeforePhase != g_pMain->m_nGamePhase) {
		g_pMain->m_pSound.Stop(SND_HOWTO);
		g_pMain->m_pSound.Stop(SND_INTRO);
		g_pMain->m_nGameBeforePhase = g_pMain->m_nGamePhase;
	}

	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------

	//m_pMainCamera->Update(m_Timer.GetSPF());
	//m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));

	g_pMain->m_TankManager.frame(&g_pMain->m_Timer, g_pMain->m_pMainCamera);

	g_pMain->m_CustomMap.Frame();

	g_pMain->ShellManagerFrame();

	g_pMain->ColCheck();

	g_pMain->m_WaveManager.Frame();

	g_pMain->m_fPlayTime = (int)g_fDurationTime;

	return true;
};
bool		GGameSingle::Render() {


	g_pMain->m_pSkyBoxObj->SetMatrix(0, g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	g_pMain->m_pSkyBoxObj->Render(g_pMain->m_pImmediateContext, g_pMain->m_pMainCamera);

	//DX::ApplyDSS(m_pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	//DX::ApplyBS(m_pImmediateContext, DX::GDxState::g_pAlphaBlend);
	g_pMain->m_CustomMap.SetMatrix(g_pMain->m_pMainCamera->GetWorldMatrix(), g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	g_pMain->m_CustomMap.Render(g_pMain->m_pImmediateContext);

	//탱크 렌더링
	g_pMain->m_TankManager.render(g_pMain->m_pMainCamera);
	//총알 렌더링
	g_pMain->m_ShellManager.render();
	//게이지 렌더링
	for (int iBar = 0; iBar < 3; iBar++)
	{
		g_pMain->m_pBar[iBar].SetMatrix(&g_pMain->m_mPlaneWorld[iBar], &g_pMain->m_pMainCamera->m_matView, &g_pMain->m_pMainCamera->m_matProj);
		g_pMain->m_pBar[iBar].Render(g_pMain->m_pImmediateContext);
	}
	//텍스트 test
	TCHAR pBuffer[256];
	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("PlayTime : %d"), g_pMain->m_fPlayTime);
	if (g_pMain->m_Font.m_pTextFormat)
	{
		D2D1_SIZE_F rtSize = g_pMain->m_Font.m_pRT->GetSize();
		//Draw a grid background.
		int width = static_cast <int> (rtSize.width);
		int height = static_cast <int> (rtSize.height);
		g_pMain->m_Font.Begin();
		g_pMain->m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		g_pMain->m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		RECT rc1 = { 0,0, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight / 2 };
		g_pMain->m_Font.DrawText(rc1,
			pBuffer,
			D2D1::ColorF(1, 0, 0, 1)
			);

		RECT rc2 = { 30,25, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Score : %d"), g_pMain->m_iScore);

		g_pMain->m_Font.DrawText(rc2,
			pBuffer,
			D2D1::ColorF(0, 1, 0, 1)
			);

		RECT rc3 = { 30,50, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Wave : %d"), g_iWave);

		g_pMain->m_Font.DrawText(rc3,
			pBuffer,
			D2D1::ColorF(0, 0, 1, 1)
			);

		RECT rc4 = { 30,75, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("HP : %d"), g_iHP);

		g_pMain->m_Font.DrawText(rc4,
			pBuffer,
			D2D1::ColorF(1, 0, 1, 1)
			);

		RECT rc5 = { 30,100, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Boost : %d"), g_iBoost);

		g_pMain->m_Font.DrawText(rc5,
			pBuffer,
			D2D1::ColorF(1, 1, 0, 1)
			);

		RECT rc6 = { 30,125, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Cannon : %d"), g_iShell);

		g_pMain->m_Font.DrawText(rc6,
			pBuffer,
			D2D1::ColorF(0, 1, 1, 1)
			);



		g_pMain->m_Font.End();
	}
	g_pMain->m_pSwapChain->Present(0, 0);


	return true;
};

GGameSingle::GGameSingle()
{
}


GGameSingle::~GGameSingle()
{
}
