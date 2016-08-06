#include "Sample.h"

int Sample::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {     
		case WM_KEYDOWN:
        {
            switch( wParam )
            {
				// 와이어 프레임
                case VK_F3:
					{
						m_bWireFrameRender = !m_bWireFrameRender;
						(m_bWireFrameRender ) ? SetRasterizerState(D3D11_FILL_WIREFRAME) :
												SetRasterizerState(D3D11_FILL_SOLID);
					}break;
				// Primitive Types
                case VK_F4:
					{						
						++m_iPrimitiveType;	
						m_iPrimitiveType = min( m_iPrimitiveType, 5 );
						m_pImmediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType  );  
					}break;
				case VK_F5:
					{						
						--m_iPrimitiveType;				
						m_iPrimitiveType = max( m_iPrimitiveType, 1 ); 
						m_pImmediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType    );  
					}break;				
			}
			break;
		}
	}
	return -1;
}

HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;
	 // Create the effect
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	//WCHAR str[MAX_PATH];   
    ID3DBlob* pVSBuf = NULL;

#if defined( _DEBUG ) || defined( _DEBUG )
      dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    V_RETURN( D3DX11CompileFromFile( L"HLSLwithoutFX.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSBuf, NULL, NULL ) );
    V_RETURN( GetDevice()->CreateVertexShader( ( DWORD* )pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL,  &m_pVS ) );

    ID3DBlob* pPSBuf = NULL;
    V_RETURN( D3DX11CompileFromFile( L"HLSLwithoutFX.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSBuf, NULL,NULL ) );
    V_RETURN( GetDevice()->CreatePixelShader( ( DWORD* )pPSBuf->GetBufferPointer(),pPSBuf->GetBufferSize(), NULL, &m_pPS ) );

	// Create our vertex input layout
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    V_RETURN( GetDevice()->CreateInputLayout( layout, 1, pVSBuf->GetBufferPointer(),pVSBuf->GetBufferSize(), &m_pVertexLayout ) );
	
    SAFE_RELEASE( pVSBuf );
    SAFE_RELEASE( pPSBuf );

	SetRasterizerState();
	return hr;
}

HRESULT Sample::SetRasterizerState(D3D11_FILL_MODE d3d11FillMode )
{
	HRESULT hr =S_OK;
	SAFE_RELEASE( m_pRasterizerStateNoCull );
	D3D11_RASTERIZER_DESC RSDesc;
	memset(&RSDesc, 0, sizeof(D3D11_RASTERIZER_DESC) );
	RSDesc.DepthClipEnable = TRUE;
    RSDesc.FillMode = d3d11FillMode;
    RSDesc.CullMode = D3D11_CULL_NONE;
    V_RETURN( GetDevice()->CreateRasterizerState( &RSDesc, &m_pRasterizerStateNoCull ) );
	return hr;
}
HRESULT Sample::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	
    // Create vertex buffer
    P3VERTEX vertices[] =
    {
		-0.75f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,        
		-0.25f, -0.5f, 0.5f,

		 0.0f, 0.5f, 0.5f,
		 0.25f, -0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
    };

	UINT numVertices = sizeof( vertices ) / sizeof( vertices[0] );

    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( P3VERTEX ) * numVertices;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

	CD3D11_BUFFER_DESC cbc( sizeof( P3VERTEX ) * numVertices, D3D11_BIND_VERTEX_BUFFER );	
		
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    V_RETURN( GetDevice()->CreateBuffer( &cbc, &InitData, &m_pVertexBuffer ) ); 

	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateTrangle( D3D11_PRIMITIVE_TOPOLOGY iTopology )
{
    HRESULT hr = S_OK;
	//  정점 쉐이더 및 픽셀 쉐이더를 로딩 및 생성한다.
	V_RETURN( CreateVertexBuffer() );  
	V_RETURN( LoadShaderAndInputLayout() );   	 
	return hr;
}

bool Sample::Init()
{
	if( FAILED( CreateTrangle() ) )
	{
		MessageBox( 0, _T("CreateTrangle  실패"), _T("Fatal error"), MB_OK );
		return false;
	}
	return true;
}
bool Sample::Render()
{
	// Set the input layout
    m_pImmediateContext->VSSetShader( m_pVS, NULL, 0 );
    m_pImmediateContext->HSSetShader( NULL, NULL, 0 );
    m_pImmediateContext->DSSetShader( NULL, NULL, 0 );
    m_pImmediateContext->GSSetShader( NULL, NULL, 0 );
    m_pImmediateContext->PSSetShader( m_pPS, NULL, 0 );
	m_pImmediateContext->IASetInputLayout( m_pVertexLayout );

	UINT stride = sizeof( P3VERTEX );
    UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pImmediateContext->RSSetState( m_pRasterizerStateNoCull );
	m_pImmediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType ); 

   m_pImmediateContext->Draw( 6, 0 );
	return true;
}
bool Sample::Release()
{
	SAFE_RELEASE( m_pRasterizerStateNoCull );
	SAFE_RELEASE( m_pVertexLayout ); // 정정레이아웃 소멸
	SAFE_RELEASE( m_pVertexBuffer ); // 정점버퍼 소멸	
	SAFE_RELEASE( m_pVS );         // 정점쉐이더 소멸
	SAFE_RELEASE( m_pPS );         // 픽쉘쉐이더 소멸 
	return true;
}
Sample::Sample(void)
{
	m_pVertexLayout		= NULL;
	m_pVertexBuffer		= NULL;
	m_pVS				= NULL;
	m_pPS				= NULL;	
	m_pRasterizerStateNoCull = NULL;
	m_bWireFrameRender	= false;
	m_iPrimitiveType	= 4;//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	
}

Sample::~Sample(void)
{
}

GBASIS_RUN(L"Gurid");