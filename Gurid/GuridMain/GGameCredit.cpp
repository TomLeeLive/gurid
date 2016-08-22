#include "_StdAfx.h"
void		GGameCredit::SoundPlay() {
	if (g_pMain->m_nGameBeforePhase != g_pMain->m_nGamePhase) {
		g_pMain->m_pSound.Stop(SND_INTRO);
		g_pMain->m_pSound.Stop(SND_HOWTO);
		g_pMain->m_nGameBeforePhase = g_pMain->m_nGamePhase;
	}
}
void GGameCredit::KeyCheck() {

	if (I_Input.KeyCheck(DIK_RETURN) == KEY_HOLD || I_Input.KeyCheck(DIK_ESCAPE) == KEY_HOLD)
	{
		g_pMain->m_nGamePhase = ST_START;
		g_pMain->m_nGameBeforePhase = ST_CREDIT;
	}
}
bool	GGameCredit::Init() {
	I_Ime.ImeUi_Initialize(g_pMain->m_hWnd);

	GImageCtl* pImageCtl = new GImageCtl();
	pImageCtl->Set(g_pMain->m_SwapChainDesc.BufferDesc.Width, g_pMain->m_SwapChainDesc.BufferDesc.Height);
	pImageCtl->Create(g_pMain->GetDevice(), nullptr, L"data/menu/credit.png");
	pImageCtl->Scale(400 - 1.0f, 300 - 1.0f, 1.0f - 1.0f);
	pImageCtl->Move(0, 0, 100);
	m_pUIList.push_back(pImageCtl);

	//TButtonCtl* pBoxCtl = new TButtonCtl();
	//pBoxCtl->Set(m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
	//pBoxCtl->Create(GetDevice(), nullptr, L"_bak/ui/exit_lg.bmp");
	//pBoxCtl->Scale(100 - 1.0f, 50 - 1.0f, 1 - 1.0f);
	//pBoxCtl->Move(0, 0, 0);
	//m_pUIList.push_back(pBoxCtl);

#pragma region g_pTextureSRV_g_pSamplerState
	m_pTextureSRV[0].Attach(CreateShaderResourceView(g_pMain->GetDevice(), L"_bak/ui/Background.bmp"));
	m_pTextureSRV[1].Attach(CreateShaderResourceView(g_pMain->GetDevice(), L"_bak/ui/_connect.bmp"));
	m_pTextureSRV[2].Attach(CreateShaderResourceView(g_pMain->GetDevice(), L"_bak/ui/_exit_lg.bmp"));
#pragma endregion

	return true;
};

GGameCredit::GGameCredit()
{
}


GGameCredit::~GGameCredit()
{
}
