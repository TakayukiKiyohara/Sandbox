/*!
 *@brief	レンダリングターゲット
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkRenderTarget.h"
#include "tkEngine/tkEngine.h"

namespace tkEngine{
	CRenderTarget::CRenderTarget() :
		m_depthSurface(nullptr),
		m_textureDX(nullptr),
		m_surface(nullptr),
		m_width(0),
		m_height(0)
	{
	}
	CRenderTarget::~CRenderTarget()
	{
		Release();
	}
	void CRenderTarget::Create( 
		int w, 
		int h, 
		int mipLevel,
		EFormat colorFormat,
		EFormat depthStencilFormat,
		EMultisampleType multiSampleType,
		int multiSampleQuality
	)
	{
		Release();
		m_width = w;
		m_height = h;
		LPDIRECT3DDEVICE9 d3dDevice = CEngine::Instance().GetD3DDevice();
		HRESULT hr;
		if(depthStencilFormat != FMT_INVALID){
			//深度バッファの作成。
			hr = d3dDevice->CreateDepthStencilSurface(
				w,
				h,
				s_cast<D3DFORMAT>(depthStencilFormat),
				s_cast<D3DMULTISAMPLE_TYPE>(multiSampleType),
				multiSampleQuality,
				TRUE,
				&m_depthSurface,
				NULL
			);
			TK_ASSERT(SUCCEEDED(hr), "failed CreateDepthStencilSurface");
		}
		if (colorFormat != FMT_INVALID) {
			//カラーバッファを作成。
			hr = d3dDevice->CreateTexture(
				w,
				h,
				mipLevel,
				D3DUSAGE_RENDERTARGET,
				s_cast<D3DFORMAT>(colorFormat),
				D3DPOOL_DEFAULT,
				&m_textureDX,
				NULL
				);
			TK_ASSERT(SUCCEEDED(hr), "Failed CreateTexture");


			hr = m_textureDX->GetSurfaceLevel(0, &m_surface);
			TK_ASSERT(SUCCEEDED(hr), "Failed GetSurfaceLevel");
			m_texture.SetTextureDX(m_textureDX);
		}
	}
	void CRenderTarget::Release()
	{
		if (m_textureDX != nullptr) {
			m_textureDX->Release();
			m_textureDX = nullptr;
		}
		if (m_depthSurface != nullptr) {
			m_depthSurface->Release();
			m_depthSurface = nullptr;
		}
		if (m_surface != nullptr) {
			m_surface->Release();
			m_surface = nullptr;
		}
		m_texture.Release();
	}
}
