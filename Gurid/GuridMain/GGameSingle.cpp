#include "_StdAfx.h"

bool		GGameSingle::Frame() {

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

#pragma region Effect
	//// 2초당 1회전( 1 초 * D3DX_PI = 3.14 )
	//float t = g_pMain->m_Timer.GetElapsedTime() * (float)D3DX_PI;
	////--------------------------------------------------------------------------------------
	//// 엔진에 있는 카메라의 월드행렬 갱신
	////--------------------------------------------------------------------------------------
	//g_pMain->m_pMainCamera->Update(g_fSecPerFrame);
	//g_pMain->m_matWorld = *g_pMain->m_pMainCamera->GetWorldMatrix();
	////m_pBox->m_cbData.Color = D3DXVECTOR4(1, 1, 1, cosf(m_Timer.GetElapsedTime())*0.5f + 0.5f);


	//// 빌보드 행렬
	//FLOAT fDeterminant;
	//D3DXMATRIX matBillboard;
	//D3DXMatrixInverse(&matBillboard, &fDeterminant, g_pMain->m_pMainCamera->GetViewMatrix());
	//matBillboard._41 = 0.0f;
	//matBillboard._42 = 0.0f;
	//matBillboard._43 = 0.0f;
	//matBillboard._44 = 1.0f;

	//for (int iPlane = 0; iPlane < 2; iPlane++)
	//{
	//	g_pMain->m_pSprite[iPlane]->SetMatrix(&matBillboard, g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	//	g_pMain->m_pSprite[iPlane]->Frame(g_pMain->m_pImmediateContext, g_pMain->m_Timer.GetElapsedTime(), g_fSecPerFrame);
	//}
#pragma endregion

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
	
	
#pragma region Effect
	////-----------------------------------------------------------------------
	//// 현재 세팅된 뷰포트 정보 저장
	////-----------------------------------------------------------------------
	//D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	//UINT nViewPorts = 1;
	//g_pMain->m_pImmediateContext->RSGetViewports(&nViewPorts, vpOld);

	//// Store the old render targets
	//ID3D11RenderTargetView* pOldRTV;
	//ID3D11DepthStencilView* pOldDSV;
	//g_pMain->m_pImmediateContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);
	//{
	//	ApplyDSS(g_pMain->m_pImmediateContext, GDxState::g_pDSSDepthDisableNoWrite);
	//	g_pMain->m_pImmediateContext->OMSetBlendState(GDxState::g_pAlphaBlend, 0, -1);
	//	D3DXMATRIX matWorld;
	//	D3DXMatrixScaling(&matWorld, 10.0f, 10.0f, 10.0f);
	//	matWorld._43 = 5.0f;
	//	g_pMain->m_BigPlane.SetMatrix(&matWorld, g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	//	g_pMain->m_BigPlane.Render(g_pMain->m_pImmediateContext);
	//	//-----------------------------------------------------------------------
	//	// 0번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[0].Get(), 0, -1);
	//	static float fTime = 0;
	//	fTime += g_fSecPerFrame;
	//	if (fTime > 1.0f)
	//	{
	//		g_pMain->m_pSprite[0]->m_cbData.Color.x = randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.y = 0;// randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.z = 0;// randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.w = 1.0f;
	//		fTime = 0;
	//	}
	//	RenderScene(g_pMain->m_pSprite[0]);
	//	//-----------------------------------------------------------------------
	//	// 1번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	//-----------------------------------------------------------------------
	//	// 2번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------		
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[1].Get(), 0, -1);
	//	RenderScene(g_pMain->m_pSprite[0]);
	//	g_pMain->m_matPlaneWorld._43 = -2.0f;
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	//-----------------------------------------------------------------------
	//	// 3번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------		
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[2].Get(), 0, -1);
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	g_pMain->m_matPlaneWorld._43 = -1.0f;
	//	RenderScene(g_pMain->m_pSprite[0]);
	//}
	////-----------------------------------------------------------------------
	//// 기본 뷰포트 정보로 복원
	////-----------------------------------------------------------------------
	//g_pMain->m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);
	//g_pMain->m_pImmediateContext->OMSetRenderTargets(1, &pOldRTV, pOldDSV);

	//// OMGetRenderTargets함수를 사용하였다면 반드시 아래와 같이 소멸시켜야 한다.
	//SAFE_RELEASE(pOldRTV);
	//SAFE_RELEASE(pOldDSV);

	////-----------------------------------------------------------------------
	//// 뷰포트 번호 출력
	////-----------------------------------------------------------------------
	////for (int iVp = 0; iVp < 4; iVp++)
	////{
	//	RECT rc;
	//	rc.left = g_pMain->m_ViewPort.m_vp.TopLeftX + g_pMain->m_ViewPort.m_vp.Width*0.5f;
	//	rc.top = g_pMain->m_ViewPort.m_vp.TopLeftY;
	//	rc.right = g_pMain->m_ViewPort.m_vp.Width + rc.left;
	//	rc.bottom = g_pMain->m_ViewPort.m_vp.Height + rc.top;
	//	T_STR str = L"Vp";
	//	TCHAR strNumber[32];
	//	str += _itow(0, strNumber, 10);// _wtoi //ivp 0으로
	//	g_pMain->DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()));
	////}
#pragma endregion
	
	
	g_pMain->m_pSwapChain->Present(0, 0);
	return true;
};
bool		GGameSingle::RenderScene(GSprite* pPlane)
{
	g_pMain->m_ViewPort.Apply(g_pMain->m_pImmediateContext, g_pMain->GetRenderTargetView(), g_pMain->GetDepthStencilView());

	pPlane->SetMatrix(&g_pMain->m_matPlaneWorld, g_pMain->m_pMainCamera->GetViewMatrix(), &g_pMain->m_ViewPort.m_matProj);
	pPlane->Render(g_pMain->m_pImmediateContext);
	return true;
}
GGameSingle::GGameSingle()
{
}


GGameSingle::~GGameSingle()
{
}
