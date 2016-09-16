#ifndef _TK_CRENDERCONTEXT_H_
#define _TK_CRENDERCONTEXT_H_

#include "tkEngine/graphics/tkRenderCommandBuffer.h"

namespace tkEngine{
	class CVertexBuffer;
	class CIndexBuffer;
	class CPrimitive;
	class CRenderTarget;
	class CSkinModel;
	/*!
	* @brief	レンダリングコンテキスト。
	*/
	class CRenderContext : Noncopyable{
	public:
		CRenderContext() {
			memset(m_pCurrentRT, 0, sizeof(m_pCurrentRT));
		}
		~CRenderContext() {}
		/*!
		 *@brief	インスタンスの取得。
		 */
		static CRenderContext* GetInstance()
		{
			static CRenderContext instance;
			return &instance;
		}
		/*!
		 * @brief	頂点定義を設定。IDirect3DDevice9::SetVertexDeclarationと同じ。
		 *@param[in]	vertexDecl		頂点定義。
		 */
		__inline void SetVertexDeclaration(SVertexDecralation* vertexDecl);
		/*!
		 * @brief	頂点ストリームを設定。IDirect3DDevice9::SetStreamSourceと同じ。
		 *@param[in]	streamNo		ストリーム番号。
		 *@param[in]	pVB				頂点バッファ。
		 */
		__inline void SetStreamSource(int streamNo, CVertexBuffer* pVB);
		/*!
		 * @brief	インデックスバッファを設定。IDirect3DDevice9::SetIndicesと同じ。
		 *@param[in]	pIB		インデックスバッファ。
		 */
		__inline void SetIndices(CIndexBuffer* pIB);
		/*!
		 * @brief	インデックスバッファを参照してプリミティブを描画。IDirect3DDevice9::DrawIndexPrimitiveと同じ。
		 */
		__inline void DrawIndexedPrimitive(CPrimitive* prim);
		/*!
		* @brief	レンダリングターゲットと深度ステンシルバッファのクリア。IDirect3DDevice9::Clearと同じ。
		*@param[in]	Count		クリアする領域のカウント。pRectsをNULLに設定する場合はこのパラメータを０にする。
		*@param[in] pRects		レンダリングターゲットをクリアする領域を示す矩形。</br>
		*						この引数にNULLを指定した場合はレンダリングターゲット全体がクリアされる。
		*@param[in]	Flags		クリアするサーフェイスを示すフラグ。
		*@param[in]	Color		レンダリングターゲットのクリアカラー。32bit ARGB。
		*@param[in]	Z			Zバッファのクリア値。
		*@param[in]	Stencil		ステンシルバッファのクリア値。
		*/
		__inline void Clear(
			int Count,
			const D3DRECT  *pRects,
			int Flags,
			int Color,
			float Z,
			int Stencil);
		/*!
		* @brief	ビューポートを設定。。IDirect3DDevice9::SetViewportと同じ。
		*/
		__inline void SetViewport(const tkEngine::SViewport& viewport);
		/*!
		* @brief	レンダーステートを設定。
		*@param[in]	renderStateType		設定するレンダリングステートの種類。
		*@param[in]	value				設定する値。
		*/
		__inline void SetRenderState(ERenderStateType renderStateType, int value);
		/*!
		* @brief	レンダリングターゲットを設定。
		*@param[in]	renderTargetIndex	レンダリングターゲットのインデックス。
		*@param[in]	pRT					レンダリングターゲット。
		*/
		__inline void SetRenderTarget(int renderTargetIndex, CRenderTarget* pRT);
		/*!
		 * @brief	メッシュを描画。
		 *@param[in]	mesh		メッシュ。
		 *@param[in]	attrId		
		 */
		__inline void MeshDrawSubset(LPD3DXMESH mesh, DWORD attrId);
		/*!
		* @brief	スキンモデルを描画。
		*@param[in]	skinMode		スキンモデル。
		*/
		__inline void DrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		* @brief	スキンモデルをシャドウマップに描画。
		*@param[in]	skinMode		スキンモデル。
		*/
		__inline void DrawSkinModelToShadowMap(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		* @brief	スキンモデルのインスタンシング描画。
		*@param[in]	skinMode		スキンモデル。
		*/
		__inline void InstancingDrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		 *@brief	初期化。
		 *@param[in]	pD3DDevice			Direct3Dデバイス
		 *@param[in]	commandBufferSize	コマンドバッファのサイズ。
		 */
		void Init(LPDIRECT3DDEVICE9 pD3DDevice, int commandBufferSize);
		/*!
		 *@brief	レンダリングコマンドを追加
		 */
		template<class T>
		void SetRenderCommand( T& command )
		{
			static_assert( sizeof(T) <= 64, "command size is over 32byte" );
			m_commandBuffer.WriteCommand64( command );
		}
		/*!
		 *@brief	コマンドバッファをサブミット
		 */
		void SubmitCommandBuffer()
		{
			m_commandBuffer.Submit(m_pD3DDevice);
		}
		/*!
		*@brief	コマンドバッファからアロケート
		*/
		void* AllocFromCommandBuffer(int sizeInByte)
		{
			return m_commandBuffer.Alloc(sizeInByte);
		}
		/*!
		 * @brief	現在のレンダリングターゲットを取得。
		 *@param[in]	rtIndex		レンダリングターゲットのインデックス。
		 */
		CRenderTarget* GetRenderTarget( int rtIndex ) const
		{
			TK_ASSERT( rtIndex < NUM_MRT, "rtIndex is invalid!!" );
			return m_pCurrentRT[rtIndex];
		}
	private:
		static const int NUM_MRT = 4;					//!<MRTはとりあえず４に制限。
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		CRenderCommandBuffer 	m_commandBuffer;
		CRenderTarget*			m_pCurrentRT[NUM_MRT];	//!<現在のレンダリングターゲット。
	};
}

#include "tkEngine/graphics/tkRenderCommand.h"
namespace tkEngine {
	__inline void CRenderContext::SetVertexDeclaration(SVertexDecralation* vertexDecl)
	{
		SetRenderCommand(CRenderCommand_SetVertexDeclaration(vertexDecl));
	}
	__inline void CRenderContext::SetStreamSource(int streamNo, CVertexBuffer* pVB)
	{
		SetRenderCommand(CRenderCommand_SetStreamSource(
			0,
			pVB
			));
	}
	__inline void CRenderContext::SetIndices(CIndexBuffer* pIB)
	{
		SetRenderCommand(CRenderCommand_SetIndices(
			pIB
			));
	}
	__inline void CRenderContext::DrawIndexedPrimitive(CPrimitive* prim)
	{
		SetRenderCommand(CRenderCommand_DrawIndexedPrimitive(
			prim
			));
	}
	__inline void CRenderContext::Clear(
		int Count,
		const D3DRECT *pRects,
		int Flags,
		int Color,
		float Z,
		int Stencil
		)
	{
		SetRenderCommand(CRenderCommand_Clear(
			Count, 
			pRects, 
			Flags,
			Color, 
			Z, 
			Stencil
		));
	}
	__inline void CRenderContext::SetViewport(const tkEngine::SViewport& viewport)
	{
		SetRenderCommand(CRenderCommand_SetViewport(viewport));
	}
	__inline void CRenderContext::SetRenderState(ERenderStateType renderStateType, int value)
	{
		SetRenderCommand(CRenderCommand_SetRenderState(renderStateType, value));
	}
	__inline void CRenderContext::SetRenderTarget(int renderTargetIndex, CRenderTarget* pRT)
	{
		TK_ASSERT(renderTargetIndex < NUM_MRT, "renderTargetIndex is invalid");
		m_pCurrentRT[renderTargetIndex] = pRT;
		SetRenderCommand(CRenderCommand_SetRenderTarget(renderTargetIndex, pRT));
		SetRenderCommand(CRenderCommand_SetDepthStencilSurface(renderTargetIndex, pRT));
	}
	/*!
	* @brief	メッシュを描画。
	*@param[in]	mesh		メッシュ。
	*/
	__inline void CRenderContext::MeshDrawSubset(LPD3DXMESH mesh, DWORD attrId)
	{
		SetRenderCommand(CRenderCommand_MeshDrawSubset(mesh, attrId));
	}
	/*!
	* @brief	スキンモデルを描画。
	*@param[in]	skinMode		スキンモデル。
	*/
	__inline void CRenderContext::DrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		SetRenderCommand(CRenderCommand_DrawSkinModel(*this, skinModel, viewMatrix, projMatrix));
	}
	/*!
	* @brief	スキンモデルをシャドウマップに描画。
	*@param[in]	skinMode		スキンモデル。
	*/
	__inline void CRenderContext::DrawSkinModelToShadowMap(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		SetRenderCommand(CRenderCommand_DrawSkinModelToShadowMap(*this, skinModel, viewMatrix, projMatrix));
	}
}


#endif // _TK_CRENDERCONTEXT_H_