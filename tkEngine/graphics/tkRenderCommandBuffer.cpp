/*!
 *@brief	レンダリングコマンドバッファクラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkRenderCommandBuffer.h"

namespace tkEngine{
	CRenderCommandBuffer::CRenderCommandBuffer() :
		m_writePos(0),
		m_size(0)
	{
	}
	CRenderCommandBuffer::~CRenderCommandBuffer()
	{
	}
	void CRenderCommandBuffer::Init(int size)
	{
		m_buffer.reset( new unsigned char[size] );
		m_writePos = 0;
		m_size = size;
	}
	void CRenderCommandBuffer::Submit(LPDIRECT3DDEVICE9 pD3DDevice)
	{
		for( void* cmd : m_command ){
			CRenderCommandBase* cmdBase = reinterpret_cast<CRenderCommandBase*>(cmd);
			switch(cmdBase->GetType()){
			case eRenderCommand_Clear:
				((CRenderCommand_Clear*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectBegin:
				((CRenderCommand_EffectBegin*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectEnd:
				((CRenderCommand_EffectEnd*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectBeginPass:
				((CRenderCommand_EffectBeginPass*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectEndPass:
				((CRenderCommand_EffectEndPass*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectSetValue:
				((CRenderCommand_EffectSetValue*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetStreamSource:
				((CRenderCommand_SetStreamSource*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetIndices:
				((CRenderCommand_SetIndices*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_DrawIndexedPrimitive:
				((CRenderCommand_DrawIndexedPrimitive*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectSetTechnique:
				((CRenderCommand_EffectSetTechnique*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetFVF:
				((CRenderCommand_SetFVF*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetViewport:
				((CRenderCommand_SetViewport*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetRenderState:
				((CRenderCommand_SetRenderState*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetVertexDeclaration:
				((CRenderCommand_SetVertexDeclaration*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetRenderTarget:
				((CRenderCommand_SetRenderTarget*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_SetDepthStencilSurface:
				((CRenderCommand_SetDepthStencilSurface*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectSetTexture:
				((CRenderCommand_EffectSetTexture*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRenderCommand_EffectCommitChanges:
				((CRenderCommand_EffectCommitChanges*)(cmdBase))->Execute(pD3DDevice);
				break;
			case eRendderCommand_MeshDrawSubset:
				((CRenderCommand_MeshDrawSubset*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_DrawSkinModel:
				((CRenderCommand_DrawSkinModel*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_DrawSkinModelToShadowMap:
				((CRenderCommand_DrawSkinModelToShadowMap*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_PerfBeginEvent:
				((CRenderCommand_PerfBeginEvent*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_PerfEndEvent:
				((CRenderCommand_PerfEndEvent*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_SetVertexShaderConstantF:
				((CRenderCommand_SetVertexShaderConstantF*)(cmdBase))->Execute(pD3DDevice);
				break;
			case enRenderCommand_SetPixelShaderConstantF:
				((CRenderCommand_SetPixelShaderConstantF*)(cmdBase))->Execute(pD3DDevice);
				break;
			default:
				TK_ASSERT(false, "not implement renderCommand");
				break;
			}
		}

		m_command.clear();
		m_writePos = 0;
	}
}