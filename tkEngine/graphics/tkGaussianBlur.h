/*!
 * @brief	ガウシアンブラー。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	ガウシアンブラー。
	 */
	class CGaussianBlur{
	public:
		enum EnUseWeight {
			enUseWeight_2,		//!<ウェイト2本。
			enUseWeight_4,		//!<ウェイト4本。
			enUseWeight_6,		//!<ウェイト6本。
			enUseWeight_8,		//!<ウェイト8本。
			enNumUseWeight,
		};
		/*!
		 * @brief	コンストラクタ。
		 */
		CGaussianBlur();
		/*!
		 * @brief	デストラクタ。
		 */
		~CGaussianBlur();
		void Init( int w, int h, const CTexture& srcTexture );
		/*!
		* @brief	ソーステクスチャを設定。
		*@details
		* Initで使用したテクスチャと同じサイズのものを渡すように。
		*@param[in]	texture		ソーステクスチャ。
		*/
		void SetSrcTexture(const CTexture& texture)
		{
			m_srcTexture = &texture;
		}
		void Draw(CRenderContext& renderContext);
		const CTexture* GetTexture() const
		{
			return m_rt[1].GetTexture();
		}
		/*!
		* @brief	ブラーの強度を設定。
		*@details
		* Initで使用したテクスチャと同じサイズのものを渡すように。
		*@param[in]	texture		ソーステクスチャ。
		*/
		void SetBlurPower(float blurPower)
		{
			m_blurPower = blurPower;
		}
		/*!
		* @brief	使用するウェイトの数を設定。
		*@param[in]	w		EnUseWeightを参照。
		*/
		void SetUseWeights(EnUseWeight w)
		{
			m_useWeights = w;
		}
	private:
		void UpdateWeight(float dispersion);
	private:
		
		CRenderTarget		m_rt[2];			//!<ガウシアンブラーの結果を描きこむレンダリングターゲット。
		const CTexture*		m_srcTexture;		//!<ソースとなるテクスチャ。
		CPrimitive			m_prim;				//!<プリミティブ。
		CEffect*			m_effect;			//!<エフェクト。
		int					m_srcTexWH[2]; 
		EnUseWeight			m_useWeights;		//!<使用するウェイトの数。
		static const int	MAX_WEIGHTS = 8;
		float				m_weights[MAX_WEIGHTS];
		float				m_blurPower;
	};
}