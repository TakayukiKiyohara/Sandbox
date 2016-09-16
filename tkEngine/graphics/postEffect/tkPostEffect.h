/*!
 * @brief	ポストエフェクト
 */

#ifndef _TKPOSTEFFECT_H_
#define _TKPOSTEFFECT_H_

#include "tkEngine/graphics/postEffect/tkEdgeRender.h"
#include "tkEngine/graphics/postEffect/tkBloomRender.h"
#include "tkEngine/graphics/postEffect/tkDOF.h"
#include "tkEngine/graphics/postEffect/tkFxaa.h"

namespace tkEngine{
	/*!
	 * @brief	ポストエフェクト。
	 */
	class CPostEffect{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPostEffect();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPostEffect();
		/*!
		* @brief	開放。
		*/
		void Release();
		/*!
		 * @brief
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext);
		/*!
		* @brief	フルスクリーン描画。
		*/
		void RenderFullScreen(CRenderContext& renderContext);
		/*!
		* @brief	DOFを取得。
		*/
		CDof& GetDof()
		{
			return m_dof;
		}
	private:
		CEdgeRender		m_edgeRender;		//!<エッジ描画。
		CBloomRender	m_bloomRender;		//!<Bloom。
		CDof			m_dof;				//!<被写界深度
		CFxaa			m_fxaa;
		CPrimitive		m_fullscreenRenderPrim;	//!<フルスクリーンをレンダリングするためのプリミティブ。
	};
}

#endif // _TKPOSTEFFECT_H_
