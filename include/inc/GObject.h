#pragma once
#include "GAseObj.h"

class GObject : public GShape
{
public:
	vector<int>			m_pModelList;
public:
	bool		Init();
	// ����Ϸ��� ��Ų�޽� ���ϸ�, ���ϸ��̼Ǹ�Ʈ���� �ε���, ���̴� �ε���
	bool		Load(ID3D11Device* pd3dDevice,
		const TCHAR* strFileName,
		const TCHAR* strShaderName,
		bool bThread = false);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
	bool		ResetResource();
	//void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	GModel*		SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd);
	GModel*		SetActionFrame(DWORD dwIndex, DWORD dwStrat, DWORD dwEnd);
	HRESULT					SetInputLayout();
	HRESULT					CreateVertexBuffer();
	HRESULT					CreateIndexBuffer();
	//HRESULT					LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile);
	//HRESULT					CreateConstantBuffer();
	HRESULT					LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
public:
	GObject(void);
	virtual ~GObject(void);
};
