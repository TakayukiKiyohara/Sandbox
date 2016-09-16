/*!
 * @brief	フォント。
 */

#pragma once


namespace tkEngine{

	/*!
	 * @brief	フォントクラス。
	 */
	class CFont{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CFont();
		/*!
		 * @brief	デストラクタ。
		 */
		~CFont();
		/*!
		* @brief	初期化。
		*/
		void Init();
		/*!
		 * @brief	描画。
		 *@details
		 * 改行は未対応。
		 *@param[in]	text		表示するテキスト
		 *@param[in]	pos_x		表示するx座標。
		 *@param[in]	pos_y		表示するy座標。
		 */
		void Draw(const char* text, int pos_x, int pos_y);
		/*!
		* @brief	開放。
		*/
		void Release();
	private:
		LPD3DXFONT font;
	};

}