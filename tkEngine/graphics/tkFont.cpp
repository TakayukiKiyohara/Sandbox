/*!
 * @brief	�t�H���g
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkFont.h"


namespace tkEngine{

	namespace {
		const int FONT_WIDTH = 16;	//�t�H���g�̕��B
		const int FONT_HEIGHT = 24;	//�t�H���g�̍����B
	}
	CFont::CFont() :
		font(nullptr)
	{
	}
	CFont::~CFont()
	{
		Release();
	}
	
	void CFont::Init()
	{

		D3DXCreateFont(
			Engine().GetD3DDevice(),	// Direct3D�f�o�C�X
			FONT_HEIGHT,				// ����
			FONT_WIDTH,					// ��
			FW_REGULAR,					// �t�H���g�̑��� ����
			NULL,						// ����
			FALSE,						// �Α�
			SHIFTJIS_CHARSET,			// �����Z�b�g
			OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
			PROOF_QUALITY,				// �����i�����d��
			FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
			TEXT("�l�r�@�o�S�V�b�N"),		// �t�H���g��
			&font);
	}
	/*!
	* @brief	�J���B
	*/
	void CFont::Release()
	{
		if (font != nullptr) {
			font->Release();
			font = nullptr;
		}
	}
	/*!
	* @brief	�`��B
	*@param[in]	text		�\������e�L�X�g
	*@param[in]	pos_x		�\������x���W�B
	*@param[in]	pos_y		�\������y���W�B
	*/
	void CFont::Draw(const char* text, int pos_x, int pos_y)
	{
		if (font != nullptr) {
			int textLen = (int)strlen(text);
			RECT rc = {
				pos_x,
				pos_y,
				pos_x + textLen * FONT_WIDTH,
				pos_y + FONT_HEIGHT
			};
			font->DrawText(
				nullptr,
				text,
				-1,
				&rc,
				DT_LEFT,
				D3DCOLOR_XRGB(255, 255, 255)	// ���F
				);
		}
	}

}
