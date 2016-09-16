/*!
 * @brief	キー入力
 */
 
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Input/tkKeyInput.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CKeyInput::CKeyInput()
	{
		memset(m_keyPressFlag, 0, sizeof(m_keyPressFlag));
		memset(m_keyTrigerFlag, 0, sizeof(m_keyTrigerFlag));
		m_mousePositionX = 0;
		m_mousePositionY = 0;
		m_isMouseUp[0] = false;
		m_isMouseUp[1] = false;
		int padNo = 0;
		for (CPad& pad : m_pad) {
			pad.Init( padNo );
			padNo++;
		}
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CKeyInput::~CKeyInput()
	{
	}
	/*!
	 * @brief	キー情報の更新。
	 */
	void CKeyInput::Update()
	{
		memset(m_keyTrigerFlag, 0, sizeof(m_keyTrigerFlag));
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			m_keyPressFlag[enKeyUp] = true;
		}
		else {
			m_keyPressFlag[enKeyUp] = false;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			m_keyPressFlag[enKeyDown] = true;
		}
		else {
			m_keyPressFlag[enKeyDown] = false;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			m_keyPressFlag[enKeyRight] = true;
		}
		else {
			m_keyPressFlag[enKeyRight] = false;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			m_keyPressFlag[enKeyLeft] = true;
		}
		else {
			m_keyPressFlag[enKeyLeft] = false;
		}
		if ((GetAsyncKeyState('A') & 0x8000) | (GetAsyncKeyState('a') & 0x8000) ) {
			if (!m_keyPressFlag[enKeyA]) {
				m_keyTrigerFlag[enKeyA] = true;
			}
			m_keyPressFlag[enKeyA] = true;
		}
		else {
			m_keyPressFlag[enKeyA] = false;
		}
		if ((GetAsyncKeyState('B') & 0x8000) | (GetAsyncKeyState('b') & 0x8000)) {
			if (!m_keyPressFlag[enKeyB]) {
				m_keyTrigerFlag[enKeyB] = true;
			}
			m_keyPressFlag[enKeyB] = true;
		}
		else {
			m_keyPressFlag[enKeyB] = false;
		}
		//バックバッファの内容をフロントバッファにコピー。
		m_isMouseUp[0] = m_isMouseUp[1];
		m_isMouseUp[1] = false;

		for (CPad& pad : m_pad) {
			pad.Update();
		}
	}
}
