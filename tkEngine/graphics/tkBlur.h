/*!
 * @brief	ブラー。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	ブラー。
	 */
	class CBlur{
	public:
		CBlur();
		~CBlur();
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

	private:
		CRenderTarget		m_rt[2];			//!<ガウシアンブラーの結果を描きこむレンダリングターゲット。
		const CTexture*		m_srcTexture;		//!<ソースとなるテクスチャ。
		CPrimitive			m_prim;				//!<プリミティブ。
		CEffect*			m_effect;			//!<エフェクト。
		int					m_srcTexWH[2];
	};
}