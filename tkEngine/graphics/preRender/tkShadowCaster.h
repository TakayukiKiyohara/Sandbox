/*!
 * @brief	シャドウキャスター。
 */
 
#pragma once

namespace tkEngine{
	class CEffect;
	/*!
	 * @brief	シャドウキャスターのインターフェースクラス。
	 */
	class IShadowCaster{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		IShadowCaster()
		{
		}
		/*!
		 * @brief	デストラクタ。
		 */
		virtual ~IShadowCaster()
		{
		}
		/*!
		* @brief	描画。
		*/
		virtual void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP) = 0;
	};
	/*!
	 * @brief	シャドウキャスター(プリミティブ版)
	 */
	class CShadowCaster_Primitive : public IShadowCaster{
	public:
		CShadowCaster_Primitive();
		~CShadowCaster_Primitive();
		/*!
		 * @brief	プリミティブから作成。
		 */
		void Create( CPrimitive* prmi );
		
		/*!
		 * @brief	ワールド行列を設定。
		 */
		void SetWorldMatrix( const CMatrix& mWorld )
		{
			m_mWorld = mWorld;
		}
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP);
	private:
		CPrimitive*		m_prim;			//!<プリミティブ。
		CMatrix			m_mWorld;		//!<ワールド行列を設定。
	};
	/*!
	* @brief	シャドウキャスター(CSkinModel版)
	*/
	class CShadowCaster_SkinModel : public IShadowCaster {
	public:
		CShadowCaster_SkinModel();
		~CShadowCaster_SkinModel();
		/*!
		* @brief	スキンモデルから作成。
		*/
		void Create(CSkinModel* model);
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP);
	private:
		CSkinModel*		m_skinModel;	//!<スキンモデル。
	};
}
