#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkRenderContext.h"


namespace tkEngine{
	void CRenderContext::Init(LPDIRECT3DDEVICE9 pD3DDevice, int commandBufferSize)
	{
		TK_ASSERT( pD3DDevice != nullptr, "pD3DDevice is nullptr!!!" );
		m_pD3DDevice = pD3DDevice;
		m_commandBuffer.Init( commandBufferSize );
	}
}

