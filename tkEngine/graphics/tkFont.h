/*!
 * @brief	�t�H���g�B
 */

#pragma once


namespace tkEngine{

	/*!
	 * @brief	�t�H���g�N���X�B
	 */
	class CFont{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CFont();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CFont();
		/*!
		* @brief	�������B
		*/
		void Init();
		/*!
		 * @brief	�`��B
		 *@details
		 * ���s�͖��Ή��B
		 *@param[in]	text		�\������e�L�X�g
		 *@param[in]	pos_x		�\������x���W�B
		 *@param[in]	pos_y		�\������y���W�B
		 */
		void Draw(const char* text, int pos_x, int pos_y);
		/*!
		* @brief	�J���B
		*/
		void Release();
	private:
		LPD3DXFONT font;
	};

}