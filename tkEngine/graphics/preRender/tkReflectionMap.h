/*!
 *@brief	リフレクションマップ
 */

#ifndef TKREFLECTIONMAP_H
#define TKREFLECTIONMAP_H

namespace tkEngine{
	class CSkinModel;
	/*!
	 *@brief	リフレクションマップ。
	 */
	class CReflectionMap : Noncopyable{
	public: 
		/*!
		 *@brief	コンストラクタ。
		 */
		CReflectionMap();
		/*!
		 *@brief	デストラクタ。
		 */
		~CReflectionMap();
		/*!
		 *@brief	初期化。
		 */
		void Init(int w, int h);
		/*!
		*@brief	開放。
		*/
		void Release();
		/*!
		 *@brief	描画。
		 */
		void Render(CRenderContext& renderContext);
		/*!
		*@brief	リフレクションマップが有効か判定。
		*/
		bool IsEnable() const
		{
			return m_isEnable;
		}
		/*!
		*@brief	リフレクションマップへの描画エントリー。
		*/
		void Entry(CSkinModel* model)
		{
			m_models.push_back(model);
		}
		/*!
		*@brief	カメラを設定。
		*/
		void SetCamera(const CCamera& camera)
		{
			m_camera = &camera;
		}
	private:
		CRenderTarget				m_renderTarget;		//!<レンダリングターゲット。
		std::vector<CSkinModel*>	m_models;			//!<リフレクションマップに描画するスキンモデル。
		bool						m_isEnable;			//!<リフレクションマップの有効フラグ。
		const CCamera*				m_camera;			//!<カメラ。
	};
}

#endif //TKREFLECTIONMAP_H