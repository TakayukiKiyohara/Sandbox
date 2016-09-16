/*!
 * @brief	IDマップ
 */

#ifndef _TKIDMAP_H_
#define _TKIDMAP_H_

#include "tkEngine/graphics/tkRenderTarget.h"

namespace tkEngine{
	/*!
	 * @brief	IDMapモデル。
	 */
	class CIDMapModel {
	public:
		CIDMapModel();
		~CIDMapModel();
		/*!
		 * @brief	プリミティブから作成。
		 */
		void Create(CPrimitive* prim);
		/*!
		* @brief	ワールドビュープロジェクション行列を設定。
		*/
		void SetWVPMatrix(const CMatrix& mWVP)
		{
			m_mWVP = mWVP;
		}
		/*!
		* @brief	描画。
		*@param[in]	renderContext	レンダーコンテキスト。
		*@param[in]	pEffect			エフェクト
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect );
	private:
		CPrimitive*		m_prim;		//!<プリミティブ。
		CMatrix			m_mWVP;		//!<ワールドビュープロジェクション行列。
	};
	class CIDMap : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ
		 */
		CIDMap();
		/*!
		 * @brief	デストラクタ。
		 */
		~CIDMap();
		/*!
		 * @brief	IDマップの作成。
		 */
		void Create(int w, int h);
		/*!
		 * @brief	開放。
		 */
		void Release();
		/*!
		 * @brief	モデルをエントリー。
		 */
		void Entry( CIDMapModel* model );
		/*!
		 * @brief	IDマップに書き込み
		 *@param[in]	renderContext	レンダリングコンテキスト。
		 */
		void RenderToIDMap( CRenderContext& renderContext );
		/*!
		* @brief	IDマップのテクスチャを取得。
		*/
		const CTexture* GetTexture() const
		{
			return m_idMapRT.GetTexture();
		}
		/*!
		* @brief	IDマップの幅を取得。
		*/
		int GetWitdh() const
		{
			return m_idMapRT.GetWidth();
		}
		/*!
		* @brief	IDマップの高さを取得。
		*/
		int GetHeight() const
		{
			return m_idMapRT.GetHeight();
		}
	private:
		bool						m_isEnable;		//!<有効？
		CRenderTarget				m_idMapRT;		//!<IDマップを書き込むレンダリングターゲット。
		std::vector<CIDMapModel*>	m_idMapModels;	//!<IDマップの描画を行うモデルのリスト。
		CEffect*					m_pIDMapEffect;	//!<
	};
}
#endif // _TKIDMAP_H_
