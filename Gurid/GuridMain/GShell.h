#pragma once
class GShell : public GCylinder
{
public:
	D3DVECTOR m_vLook;

	bool Create() {

		if (FAILED(GCylinder::Create(g_pd3dDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
		{
			MessageBox(0, _T("GShell ½ÇÆÐ"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	GShell();
	GShell(D3DVECTOR vLook) {
		m_vLook = vLook;
		Create();
	}
	virtual ~GShell();
};

