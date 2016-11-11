/*!
 *@brief	レンダリングコマンド
 */

#ifndef _TKRENDERCOMMAND_H_
#define _TKRENDERCOMMAND_H_

#include "tkEngine/graphics/tkVertexBuffer.h"
#include "tkEngine/graphics/tkIndexBuffer.h"
#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/tkGraphicsType.h"
#include "tkEngine/graphics/tkRenderTarget.h"
#include "tkEngine/graphics/tkSkinModel.h"

namespace tkEngine{
	/*!
	 *@brief	レンダリングコマンドのタイプ
	 */
	enum ERenderCommand {
		eRenderCommand_Clear,
		eRenderCommand_EffectBegin,
		eRenderCommand_EffectEnd,
		eRenderCommand_EffectBeginPass,
		eRenderCommand_EffectEndPass,
		eRenderCommand_EffectSetValue,
		eRenderCommand_SetStreamSource,
		eRenderCommand_SetIndices,
		eRenderCommand_DrawIndexedPrimitive,
		eRenderCommand_EffectSetTechnique,
		eRenderCommand_SetFVF,
		eRenderCommand_SetViewport,
		eRenderCommand_SetRenderState,
		eRenderCommand_SetVertexDeclaration,
		eRenderCommand_SetRenderTarget,
		eRenderCommand_SetDepthStencilSurface,
		eRenderCommand_EffectSetTexture,
		eRenderCommand_EffectCommitChanges,
		eRendderCommand_MeshDrawSubset,
		enRenderCommand_DrawSkinModel,
		enRenderCommand_DrawSkinModelToShadowMap,
		enRenderCommand_PerfBeginEvent,
		enRenderCommand_PerfEndEvent,
		enRenderCommand_SetVertexShaderConstantF,
		enRenderCommand_SetPixelShaderConstantF,
		eRenderCommand_Undef
	};
	/*!
	 *@brief	レンダリングコマンドの基底クラス。
	 */
	class CRenderCommandBase{
	private:
		ERenderCommand m_commandType;
	public:
		/*!
		 *@brief	コンストラクタ
		 */
		CRenderCommandBase(ERenderCommand commandType ) :
			m_commandType(commandType)
		{
			
		}
		/*!
		 *@brief	型情報を取得
		 */
		ERenderCommand GetType() const
		{
			return m_commandType;
		}
	};
	/*!
	 *@brief	IDirect3DDevice9::Clear
	 */
	class CRenderCommand_Clear : public CRenderCommandBase {
	private:
		DWORD m_Count;
		const D3DRECT *m_pRects;
		DWORD m_Flags;
		D3DCOLOR m_Color;
		float m_Z;
		DWORD m_Stencil;
	public:
		/*!
		 *@brief	コンストラクタ。
		 *@details
		 * 引数の意味はIDirect3DDevice9::Clearと同じ。詳細はMSDNヘルプを参照してください。
		 */
		CRenderCommand_Clear( 
			DWORD Count,
			const D3DRECT *pRects,
			DWORD Flags,
			D3DCOLOR Color,
			float Z,
			DWORD Stencil

		) :
			CRenderCommandBase(eRenderCommand_Clear),
			m_Count(Count),
			m_pRects(pRects),
			m_Flags(Flags),
			m_Color(Color),
			m_Z(Z),
			m_Stencil(Stencil)
		{
		}
		void Execute( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			pD3DDevice->Clear(m_Count, m_pRects, m_Flags, m_Color, m_Z, m_Stencil);
		}
	};
	/*!
	 * @brief	ID3DXEffect::Begin
	 */
	class CRenderCommand_EffectBegin : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
	public:
		CRenderCommand_EffectBegin(ID3DXEffect* pEffect) :
			CRenderCommandBase(eRenderCommand_EffectBegin),
			m_pEffect(pEffect)
		{
			TK_ASSERT( m_pEffect != nullptr, "m_pEffect is null" );
		}
		void Execute( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		}
	};
	/*!
	 * @brief	ID3DXEffect::End
	 */
	class CRenderCommand_EffectEnd : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
	public:
		CRenderCommand_EffectEnd(ID3DXEffect* pEffect) :
			CRenderCommandBase(eRenderCommand_EffectEnd),
			m_pEffect(pEffect)
		{
			TK_ASSERT( m_pEffect != nullptr, "m_pEffect is null" );
		}
		void Execute( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			m_pEffect->End();
		}
	};
	/*!
	 * @brief	ID3DXEffect::BeginPass
	 */
	class CRenderCommand_EffectBeginPass : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
		int				m_pass;
	public:
		CRenderCommand_EffectBeginPass(ID3DXEffect* pEffect, int pass) :
			CRenderCommandBase(eRenderCommand_EffectBeginPass),
			m_pEffect(pEffect),
			m_pass(pass)
		{
			TK_ASSERT( m_pEffect != nullptr, "m_pEffect is null" );
		}
		void Execute( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			m_pEffect->BeginPass(m_pass);
		}
	};
	/*!
	 * @brief	ID3DXEffect::EndPass
	 */
	class CRenderCommand_EffectEndPass : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
	public:
		CRenderCommand_EffectEndPass(ID3DXEffect* pEffect) :
			CRenderCommandBase(eRenderCommand_EffectEndPass),
			m_pEffect(pEffect)
		{
			TK_ASSERT( m_pEffect != nullptr, "m_pEffect is null" );
		}
		void Execute( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			m_pEffect->EndPass();
		}
	};
	/*!
	* @brief	ID3DXEffect::CommitChanges
	*/
	class CRenderCommand_EffectCommitChanges : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
	public:
		CRenderCommand_EffectCommitChanges(ID3DXEffect* pEffect) :
			CRenderCommandBase(eRenderCommand_EffectCommitChanges),
			m_pEffect(pEffect)
		{
			TK_ASSERT(m_pEffect != nullptr, "m_pEffect is null");
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_pEffect->CommitChanges();
		}
	};
	/*!
	* @brief	ID3DXEffect::SetValue
	*/
	class CRenderCommand_EffectSetValue : public CRenderCommandBase
	{
		ID3DXEffect*	m_pEffect;
		const char*		m_pParameterName;
		void*			m_pData;
		int				m_sizeInByte;
	public:
		CRenderCommand_EffectSetValue(CRenderContext& renderContext, ID3DXEffect* pEffect, const char* pParameterName, const void* pData, int sizeInByte):
			CRenderCommandBase(eRenderCommand_EffectSetValue),
			m_pEffect(pEffect),
			m_pParameterName(pParameterName),
			m_sizeInByte(sizeInByte)
		{
			//コマンドバッファからアロック。
			m_pData = renderContext.AllocFromCommandBuffer(sizeInByte);
			//コピ
			memcpy(m_pData, pData, sizeInByte);
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_pEffect->SetValue(m_pParameterName, m_pData, m_sizeInByte);
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetStreamSource
	*/
	class CRenderCommand_SetStreamSource : public CRenderCommandBase
	{
		CVertexBuffer*	m_pVB;
		int				m_streamNo;
	public:
		CRenderCommand_SetStreamSource(int streamNo, CVertexBuffer* pVB) :
			CRenderCommandBase(eRenderCommand_SetStreamSource),
			m_pVB(pVB),
			m_streamNo( streamNo )
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			HRESULT hr = pD3DDevice->SetStreamSource(m_streamNo, m_pVB->GetBody(), 0, m_pVB->GetStride());
			TK_ASSERT(SUCCEEDED(hr), "error");
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetIndices
	*/
	class CRenderCommand_SetIndices : public CRenderCommandBase
	{
		CIndexBuffer* m_pIB;
	public:
		CRenderCommand_SetIndices(CIndexBuffer* pIB) :
			CRenderCommandBase(eRenderCommand_SetIndices),
			m_pIB(pIB)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			HRESULT hr = pD3DDevice->SetIndices(m_pIB->GetBody());
			TK_ASSERT(SUCCEEDED(hr), "error");
		}
	};
	/*!
	* @brief	IDirect3DDevice9::DrawIndexPrimitive
	*/
	class CRenderCommand_DrawIndexedPrimitive : public CRenderCommandBase
	{
		CPrimitive* m_primitive;
	public:
		CRenderCommand_DrawIndexedPrimitive(CPrimitive* prim) :
			CRenderCommandBase(eRenderCommand_DrawIndexedPrimitive),
			m_primitive(prim)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			HRESULT hr = pD3DDevice->DrawIndexedPrimitive(
				m_primitive->GetD3DPrimitiveType(), 
				0, 
				0,
				m_primitive->GetNumVertex(), 
				0, 
				m_primitive->GetNumPolygon() 
			);
			TK_ASSERT(SUCCEEDED(hr), "error");
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetFVF
	*/
	class CRenderCommand_SetFVF : public CRenderCommandBase
	{
		int m_fvf;
	public:
		CRenderCommand_SetFVF(int fvf) :
			CRenderCommandBase(eRenderCommand_SetFVF),
			m_fvf(fvf)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetFVF(m_fvf);
		}
	};
	/*!
	* @brief	ID3DXEffect::SetTechnique
	*/
	class CRenderCommand_EffectSetTechnique : public CRenderCommandBase
	{
		char* m_tecName;
		ID3DXEffect* m_pEffect;
	public:
		CRenderCommand_EffectSetTechnique(tkEngine::CRenderContext& renderContext, ID3DXEffect* pEffect, const char* tecName) :
			CRenderCommandBase(eRenderCommand_EffectSetTechnique),
			m_pEffect(pEffect)
		{
			size_t nameLen = strlen(tecName);
			m_tecName = s_cast<char*>(renderContext.AllocFromCommandBuffer((int)nameLen + 1));
			memcpy(m_tecName, tecName, nameLen + 1);
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_pEffect->SetTechnique(m_tecName);
		}
	};
	/*!
	* @brief	IDirect3DDevice9::Setviewport
	*/
	class CRenderCommand_SetViewport : public CRenderCommandBase
	{
		D3DVIEWPORT9 m_viewport;
	public:
		CRenderCommand_SetViewport(const tkEngine::SViewport& viewport) :
			CRenderCommandBase(eRenderCommand_SetViewport)
		{
			m_viewport = viewport;
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetViewport(&m_viewport);
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetRenderState
	*/
	class CRenderCommand_SetRenderState : public CRenderCommandBase
	{
		ERenderStateType m_renderStateType;
		int m_value;
	public:
		CRenderCommand_SetRenderState(ERenderStateType renderStateType, int value) :
			CRenderCommandBase(eRenderCommand_SetRenderState),
			m_renderStateType(renderStateType),
			m_value(value)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetRenderState(s_cast<D3DRENDERSTATETYPE>(m_renderStateType), m_value);
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetVertexDeclaration
	*/
	class CRenderCommand_SetVertexDeclaration : public CRenderCommandBase
	{
		IDirect3DVertexDeclaration9*	m_pVertexDecl;
	public:
		CRenderCommand_SetVertexDeclaration(SVertexDecralation* vertexDecl) :
			CRenderCommandBase(eRenderCommand_SetVertexDeclaration),
			m_pVertexDecl(vertexDecl) {}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetVertexDeclaration(m_pVertexDecl);
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetRenderTarget
	*/
	class CRenderCommand_SetRenderTarget : public CRenderCommandBase
	{
		CRenderTarget*	m_pRT;
		int	m_renderTargetIndex;
	public:
		CRenderCommand_SetRenderTarget( int renderTargetIndex, CRenderTarget* rt) :
			CRenderCommandBase(eRenderCommand_SetRenderTarget),
			m_pRT(rt),
			m_renderTargetIndex(renderTargetIndex)
		{

		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice )
		{
			if (m_pRT) {
				pD3DDevice->SetRenderTarget(m_renderTargetIndex, m_pRT->GetSurfaceDx());
			}
			else {
				pD3DDevice->SetRenderTarget(m_renderTargetIndex, nullptr);
			}
		}
	};
	/*!
	* @brief	IDirect3DDevice9::SetDepthStencilSurface
	*/
	class CRenderCommand_SetDepthStencilSurface : public CRenderCommandBase
	{
		CRenderTarget*	m_pRT;
		int	m_renderTargetIndex;
	public:
		CRenderCommand_SetDepthStencilSurface(int renderTargetIndex, CRenderTarget* pRT) :
			CRenderCommandBase(eRenderCommand_SetDepthStencilSurface),
			m_pRT(pRT),
			m_renderTargetIndex(renderTargetIndex)
		{
			if (pRT == NULL) {
				int hoge = 0;
			}
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			if (m_renderTargetIndex == 0) {
				pD3DDevice->SetDepthStencilSurface(m_pRT->GetDepthSurfaceDx());
			}
		}
	};
	/*!
	* @brief	ID3DXEffect::SetTexture
	*/
	class CRenderCommand_EffectSetTexture : public CRenderCommandBase
	{
		const CTexture*	m_pTex;
		char* m_textureName;
		ID3DXEffect*	m_pEffect;		//!<D3Dエフェクト
	public:
		CRenderCommand_EffectSetTexture(CRenderContext& renderContext, ID3DXEffect* pEffect, const char* parameterName, const CTexture* tex) :
			CRenderCommandBase(eRenderCommand_EffectSetTexture),
			m_pTex(tex),
			m_pEffect(pEffect)
		{
			size_t nameLen = strlen(parameterName);
			m_textureName = s_cast<char*>(renderContext.AllocFromCommandBuffer((int)nameLen + 1));
			memcpy(m_textureName, parameterName, nameLen + 1);
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_pEffect->SetTexture(m_textureName, m_pTex->GetTextureDX());
		}
	};
	/*!
	* @brief	ID3DXMesh::DrawSubset
	*/
	class CRenderCommand_MeshDrawSubset : public CRenderCommandBase
	{
		LPD3DXMESH	m_mesh;
		DWORD		m_attrId;
	public:
		CRenderCommand_MeshDrawSubset(LPD3DXMESH mesh, DWORD attrId) :
			CRenderCommandBase(eRendderCommand_MeshDrawSubset),
			m_mesh(mesh),
			m_attrId(attrId)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_mesh->DrawSubset(m_attrId);
		}
	};
	/*!
	* @brief	スキンモデルを描画。
	*/
	class CRenderCommand_DrawSkinModel : public CRenderCommandBase
	{
		CSkinModel* m_skinModel;
		CMatrix* m_viewMatrix;
		CMatrix* m_projMatrix;
	public:
		CRenderCommand_DrawSkinModel(CRenderContext& renderContext, CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix) :
			CRenderCommandBase(enRenderCommand_DrawSkinModel),
			m_skinModel(skinModel)
		{
			m_viewMatrix = s_cast<CMatrix*>(renderContext.AllocFromCommandBuffer(sizeof(CMatrix)));
			m_projMatrix = s_cast<CMatrix*>(renderContext.AllocFromCommandBuffer(sizeof(CMatrix)));
			*m_viewMatrix = viewMatrix;
			*m_projMatrix = projMatrix;
			
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_skinModel->ImmidiateDraw(
				pD3DDevice,
				r_cast<D3DXMATRIX*>(m_viewMatrix),
				r_cast<D3DXMATRIX*>(m_projMatrix),
				false
			);
		}
	};
	/*!
	* @brief	スキンモデルをシャドウマップに描画。
	*/
	class CRenderCommand_DrawSkinModelToShadowMap : public CRenderCommandBase
	{
		CSkinModel* m_skinModel;
		CMatrix* m_viewMatrix;
		CMatrix* m_projMatrix;
	public:
		CRenderCommand_DrawSkinModelToShadowMap(CRenderContext& renderContext, CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix) :
			CRenderCommandBase(enRenderCommand_DrawSkinModelToShadowMap),
			m_skinModel(skinModel)
		{
			m_viewMatrix = s_cast<CMatrix*>(renderContext.AllocFromCommandBuffer(sizeof(CMatrix)));
			m_projMatrix = s_cast<CMatrix*>(renderContext.AllocFromCommandBuffer(sizeof(CMatrix)));
			*m_viewMatrix = viewMatrix;
			*m_projMatrix = projMatrix;

		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			m_skinModel->ImmidiateDraw(
				pD3DDevice,
				r_cast<D3DXMATRIX*>(m_viewMatrix),
				r_cast<D3DXMATRIX*>(m_projMatrix),
				true
				);
		}
	};
	/*!
	* @brief	PIXにパフォーマンス計測用のタグを打ち込む。
	*/
	class CRenderCommand_PerfBeginEvent : public CRenderCommandBase{
		LPCWSTR name;
	public:
		CRenderCommand_PerfBeginEvent(LPCWSTR name) :
			CRenderCommandBase(enRenderCommand_PerfBeginEvent)
		{
			this->name = name;
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			D3DPERF_BeginEvent(D3DCOLOR_RGBA(0, 0, 0, 0), name);
		}
	};
	/*!
	* @brief	パフォーマンス計測終了
	*@details
	* 必ずCRenderCommand_PerfBeginEventと対になるようにすること。
	*/
	class CRenderCommand_PerfEndEvent : public CRenderCommandBase {
	public:
		CRenderCommand_PerfEndEvent() :
			CRenderCommandBase(enRenderCommand_PerfEndEvent)
		{

		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			D3DPERF_EndEvent();
		}
	};
	/*!
	*@brief	IDirect3DDevice9::SetVertexShaderConstantF
	*/
	class CRenderCommand_SetVertexShaderConstantF : public CRenderCommandBase {
		int m_startRegisterNo;
		float* m_constData;
		int m_vertexCount;
	public:
		/*!
		*@brief	コンストラクタ。
		* 引数はIDirect3DDevice9::SetVertexShaderConstantFと同じ。
		*/
		CRenderCommand_SetVertexShaderConstantF(int startRegisterNo, float* pConstData, int vertexCount) :
			CRenderCommandBase(enRenderCommand_SetVertexShaderConstantF),
			m_startRegisterNo(startRegisterNo),
			m_constData(pConstData),
			m_vertexCount(vertexCount)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetVertexShaderConstantF(m_startRegisterNo, m_constData, m_vertexCount);
		}
	};
	/*!
	*@brief	IDirect3DDevice9::SetPixelShaderConstantF
	*/
	class CRenderCommand_SetPixelShaderConstantF : public CRenderCommandBase {
		int m_startRegisterNo;
		float* m_constData;
		int m_vertexCount;
	public:
		/*!
		*@brief	コンストラクタ。
		* 引数はIDirect3DDevice9::SetVertexShaderConstantFと同じ。
		*/
		CRenderCommand_SetPixelShaderConstantF(int startRegisterNo, float* pConstData, int vertexCount) :
			CRenderCommandBase(enRenderCommand_SetPixelShaderConstantF),
			m_startRegisterNo(startRegisterNo),
			m_constData(pConstData),
			m_vertexCount(vertexCount)
		{
		}
		void Execute(LPDIRECT3DDEVICE9 pD3DDevice)
		{
			pD3DDevice->SetPixelShaderConstantF(m_startRegisterNo, m_constData, m_vertexCount);
		}
	};
}
#endif // _TKRENDERCOMMAND_H_