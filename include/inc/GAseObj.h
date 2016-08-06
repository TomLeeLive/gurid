#pragma once
#include "GModel.h"
#include "GAseParser.h"

class GAseObj : public GModel
{
public:
	GAseParser		m_Parser;
	TScene			m_Scene;
	vector<GMtrl>	m_Material;
	T_STR			m_szDirName;
	T_STR			m_szName;
public:
	bool					Init();
	bool					Frame();
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					Release();
	bool					ResetResource();
	bool					Convert(ID3D11Device* pDevice);
	bool					Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile );
	void					SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	HRESULT					SetInputLayout();
	HRESULT					CreateVertexBuffer();
	HRESULT					CreateIndexBuffer();
	HRESULT					LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile);
	HRESULT					CreateConstantBuffer();
	HRESULT					LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);

	void					SetTriangleBuffer(GMesh* pMesh,
		GAseMesh* pAseMesh,
		DWORD dwMask = 0x00000000);
	int						GetMapID(GMtrl* pMtrl, int iTexMapType = ID_GBASIS_DI); // �� �ε��� �˻�

																					//--------------------------------------------------------------------------------------
																					// ���ϸ��̼� ���� �Լ� 
																					//--------------------------------------------------------------------------------------
	GAnimTrack*				SetDoublyLinkedList(GAnimTrack* pCurrentTrack, GAnimTrack* pPrev);//���� ���� ����Ʈ ����
	bool					GetAnimationTrack(float fFrame,
		vector<shared_ptr<GAnimTrack>> pTrackList,
		GAnimTrack** ppStartTrack,
		GAnimTrack** ppEndTrack);
	void					Interpolate(GMesh* pMesh,
		D3DXMATRIX* matParent,
		float fFrameTick);
	bool					InheriteCollect();
	GMesh*		SearchToCollects(T_STR	m_strParentName);

public:
	GAseObj(void);
	virtual ~GAseObj(void);
};
