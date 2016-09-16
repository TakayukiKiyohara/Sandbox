/*!
 *@brief	FXAA
 */

#pragma once

namespace tkEngine{
	class CPostEffect;
	/*!
	 *@brief	FXAA
	 *@details
	 * ポストエフェクトによるアンチエイリアス
	 */
	class CFxaa {
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CFxaa();
		/*!
		 *@brief	デストラクタ。
		 */
		~CFxaa();
		/*!
		* @brief	開放。
		*/
		void Release();
		/*!
		 *@brief	初期化。
		 *@param[in]	config		グラフィックコンフィグ
		 */
		void Init(const SGraphicsConfig& config);
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext, CPostEffect* postEffect);
	private:
		bool			m_isEnable;	//アンチが有効かどうかのフラグ。
		CEffect*		m_effect;
	};
}