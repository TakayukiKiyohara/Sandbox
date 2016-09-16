/*!
 * @brief	被写界深度
 */

#pragma once

#include "tkEngine/graphics/tkGaussianBlur.h"

namespace tkEngine{
	class CPostEffect;
	/*!
	 * @brief	被写界深度
	 */
	class CDof : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CDof();
		/*!
		 * @brief	デストラクタ。
		 */
		~CDof();
		/*!
		 * @brief	描画。
		 */
		void Render( CRenderContext& renderContext, CPostEffect* postEffect );
		/*!
		* @brief	作成。
		*@param[in]	config		グラフィックコンフィグ。
		*/
		void Create(const SGraphicsConfig& config);
		/*!
		* @brief	深度を書き込むレンダリングターゲットの取得。
		*/
		CRenderTarget* GetDepthRenderTarget()
		{
			return &m_depthRT;
		}
		/*!
		* @brief	開放。
		*/
		void Release();
		/*!
		* @brief	ピントを設定。(カメラ座標系)。
		*@param[in]	pintPosition	ピント(単位mm)。
		*/
		void SetPint(float pint)
		{
			m_pint = pint;
		}
		/*!
		* @brief	焦点距離を設定。
		*@param[in]		length		焦点距離(単位はmm)
		*/
		void SetFocalLength(float length)
		{
			m_focalLength = length;
		}
		/*!
		* @brief	F値を設定。
		*@param[in]		f	F値
		*/
		void SetFParam(float f)
		{
			m_F = f;
		}
	private:
		CRenderTarget		m_depthRT;		//!<深度を書き込むレンダリングターゲット。
		bool				m_isEnable;		//!<DOF有効？
		CGaussianBlur		m_blurForward;	//!<前ボケ用のブラー。
		CGaussianBlur		m_blurBack;		//!<奥ボケ用のブラー。
		CEffect*			m_effect;		//!<エフェクト。
		float				m_focalLength;	//!<焦点距離。
		float				m_F;			//!<F値。
		float				m_pint;
	};
}
