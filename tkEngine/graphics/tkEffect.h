/*!
 * @brief	シェーダーエフェクト。
 */
 
#ifndef _TKEFFECT_H_
#define _TKEFFECT_H_

#include "tkEngine/graphics/tkTexture.h"

namespace tkEngine{
	/*!
	 * @brief	シェーダーエフェクト。
	 */
	class CEffect{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CEffect(ID3DXEffect* effect);
		/*!
		 * @brief	デストラクタ。
		 */
		~CEffect();
		/*!
		*@brief	テクニックを設定。
		*@param[in]	parameterName	パラメータの名前。
		*@param[in]	pData			設定する値。
		*@param[in]	sizeInByte		サイズ(単位：バイト)
		*/
		void SetTechnique(tkEngine::CRenderContext& renderContext, const char* tecName)
		{
			TK_ASSERT(tecName != nullptr, "tecName is null");
			renderContext.SetRenderCommand(CRenderCommand_EffectSetTechnique(renderContext, m_pEffect,tecName));
		}
		/*!
		 * @brief	値を設定。
		 *@param[in]	parameterName	パラメータの名前。
		 *@param[in]	pData			設定する値。
		 *@param[in]	sizeInByte		サイズ(単位：バイト)
		 */
		void SetValue(tkEngine::CRenderContext& renderContext, const char* parameterName, const void* pData, int sizeInByte )
		{
			TK_ASSERT( m_pEffect != nullptr, "effect is null" );
			renderContext.SetRenderCommand( CRenderCommand_EffectSetValue(renderContext, m_pEffect, parameterName, pData, sizeInByte ) );
		}
		void SetTexture(tkEngine::CRenderContext& renderContext, const char* parameterName, const CTexture* tex)
		{
			renderContext.SetRenderCommand(CRenderCommand_EffectSetTexture(renderContext, m_pEffect, parameterName, tex ));
		}
		/*!
		 * @brief	テクニックの適用を開始。
		 */
		void Begin(tkEngine::CRenderContext& renderContext)
		{
			TK_ASSERT(m_pEffect != nullptr, "effect is null");
			renderContext.SetRenderCommand(CRenderCommand_EffectBegin(m_pEffect));
		}
		/*!
		 * @brief	テクニックの適用を終了。
		 */
		void End(tkEngine::CRenderContext& renderContext)
		{
			TK_ASSERT(m_pEffect != nullptr, "effect is null");
			renderContext.SetRenderCommand(CRenderCommand_EffectEnd(m_pEffect));
		}
		/*!
		 * @brief	使用するテクニックのパスを設定して描画開始。
		 */
		void BeginPass(tkEngine::CRenderContext& renderContext, int pass )
		{
			TK_ASSERT(m_pEffect != nullptr, "effect is null");
			renderContext.SetRenderCommand(CRenderCommand_EffectBeginPass(m_pEffect, pass));
		}
		/*!
		 * @brief	描画終了。
		 */
		void EndPass(tkEngine::CRenderContext& renderContext)
		{
			renderContext.SetRenderCommand(CRenderCommand_EffectEndPass(m_pEffect));
		}
		/*!
		* @brief	ID3DXEffect::CommitChangesと同じ。
		*/
		void CommitChanges(CRenderContext& renderContext)
		{
			renderContext.SetRenderCommand(CRenderCommand_EffectCommitChanges(m_pEffect));
		}
		ID3DXEffect* GetD3DXEffect()
		{
			return m_pEffect;
		}
	private:
		ID3DXEffect*	m_pEffect;		//!<D3Dエフェクト
	};
}

#endif 
