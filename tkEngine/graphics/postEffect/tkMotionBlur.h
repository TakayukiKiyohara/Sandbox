/*!
 * @brief	速度マップを使用したモーションブラー。
 */

#pragma once

#include "tkEngine/Graphics/tkCamera.h"

namespace tkEngine{
	/*!
	 * @brief	速度マップを使用したモーションブラー。
	 */
	class CMotionBlur : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CMotionBlur();
		/*!
		 * @brief	デストラクタ。
		 */
		~CMotionBlur();
		/*!
		* @brief	作成。
		*@param[in]	config		グラフィックコンフィグ。
		*/
		void Create(const SGraphicsConfig& config);
		/*!
		* @brief	更新。
		*/
		void Update()
		{
			if (m_isEnable) {
				m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
			}
		}
		/*!
		* @brief	モーションブラーの有効、無効を設定。
		*/
		void SetEnable(bool flag)
		{
			if (flag && !m_isEnable) {
				//ブラーが無効から有効に変更される場合はビュープロジェクション行列を更新する。
				m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
			}
			m_isEnable = flag;
		}
		/*!
		* @brief	モーションブラーで使用するカメラを設定する。
		* @details
		* 　ブラーをかけたいシーンを描画するカメラを設定してください。
		*/
		void SetCamera(const CCamera* camera)
		{
			m_camera = camera;
			m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
		}
		/*!
		* @brief	1フレーム前のビュープロジェクション行列。
		*/
		const CMatrix& GetLastFrameViewProjectionMatrix()
		{
			return m_lastFrameViewProjMatrix;
		}
		/*!
		 * @brief	速度マップを書き込むためのレンダリングターゲット。
		 */
		CRenderTarget* GetVelocityMapRenderTarget()
		{
			return &m_velocityMapRT;
		}
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext, CPostEffect* postEffect);
	private:
		const CCamera*		m_camera = nullptr;		//!<カメラ。
		CRenderTarget		m_velocityMapRT;		//!<速度マップを書き込むためのレンダリングターゲット。
		bool				m_isEnable = false;		//!<速度マップ有効？
		CMatrix				m_lastFrameViewProjMatrix = CMatrix::Identity;	//!<1フレーム前のビュープロジェクション行列。
		CEffect*			m_effect = nullptr;		//!<エフェクト。
	};
}