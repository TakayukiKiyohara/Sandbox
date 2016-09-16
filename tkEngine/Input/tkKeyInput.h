/*!
 * @brief	キー入力。
 */

#ifndef _TKKEYINPUT_H_ 
#define _TKKEYINPUT_H_ 

#include "tkEngine/Input/tkPad.h"
namespace tkEngine{
	class CKeyInput{
		
	public:
		static const int NUM_PAD = 4;	//パッドの数。
		enum EnKey {
			enKeyUp,
			enKeyDown,
			enKeyRight,
			enKeyLeft,
			enKeyA,
			enKeyB,
			enKeyNum,
		};
		/*!
		 * @brief	コンストラクタ。
		 */
		CKeyInput();
		/*!
		 * @brief	デストラクタ。
		 */
		~CKeyInput();
		/*!
		 * @brief	キー情報の更新。
		 */
		void Update();
		/*!
		 * @brief	上キーが押されている。
		 */
		bool IsUpPress() const
		{
			return m_keyPressFlag[enKeyUp];
		}
		/*!
		 * @brief	右キーが押されている。
		 */
		bool IsRightPress() const
		{
			return m_keyPressFlag[enKeyRight];
		}
		/*!
		 * @brief	左キーが押されている。
		 */
		bool IsLeftPress() const
		{
			return m_keyPressFlag[enKeyLeft];
		}
		/*!
		 * @brief	下キーが押されている。
		 */
		bool IsDownPress() const
		{
			return m_keyPressFlag[enKeyDown];
		}
		/*!
		* @brief	キーのプレス判定。
		*/
		bool IsPress(EnKey key) const
		{
			return m_keyPressFlag[key];
		}
		/*!
		* @brief	キーのトリガー判定。
		*/
		bool IsTrgger(EnKey key) const
		{
			return m_keyTrigerFlag[key];
		}
		/*!
		* @brief	マウスの左ボタンが離されたときの処理。
		*/
		void OnMouseLButtonUp(int x, int y)
		{
			m_mousePositionX = x;
			m_mousePositionY = y;
			m_isMouseUp[1] = true;
		}
		/*!
		* @brief	マウスの左ボタンが離されたときか判定。
		*/
		bool IsMouseLButtonUp() const
		{
			return m_isMouseUp[0];
		}
		/*!
		* @brief	マウスのX座標を取得。
		*/
		int GetMousePositionX() const
		{
			return m_mousePositionX;
		}
		/*!
		* @brief	マウスのY座標を取得。
		*/
		int GetMousePositionY() const
		{
			return m_mousePositionY;
		}
		/*!
		* @brief	ゲームパッドを取得。
		*/
		const CPad& GetPad(int padNo) const
		{
			TK_ASSERT(padNo < NUM_PAD, "padNo is invalid");
			return m_pad[padNo];
		}
	private:
		bool	m_keyPressFlag[enKeyNum];
		bool	m_keyTrigerFlag[enKeyNum];
		bool	m_isMouseUp[2];
		int		m_mousePositionX;
		int		m_mousePositionY;
		CPad	m_pad[NUM_PAD];		//!<パッド。
	};
}
#endif //_TKKEYINPUT_H_ 