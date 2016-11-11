/*!
 * @brief	フォント
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkFont.h"


namespace tkEngine{

	namespace {
		const int FONT_WIDTH = 16;	//フォントの幅。
		const int FONT_HEIGHT = 24;	//フォントの高さ。
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
			Engine().GetD3DDevice(),	// Direct3Dデバイス
			FONT_HEIGHT,				// 高さ
			FONT_WIDTH,					// 幅
			FW_REGULAR,					// フォントの太さ 普通
			NULL,						// 下線
			FALSE,						// 斜体
			SHIFTJIS_CHARSET,			// 文字セット
			OUT_DEFAULT_PRECIS,			// 出力制度は普通
			PROOF_QUALITY,				// 文字品質を重視
			FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
			TEXT("ＭＳ　Ｐゴシック"),		// フォント名
			&font);
	}
	/*!
	* @brief	開放。
	*/
	void CFont::Release()
	{
		if (font != nullptr) {
			font->Release();
			font = nullptr;
		}
	}
	/*!
	* @brief	描画。
	*@param[in]	text		表示するテキスト
	*@param[in]	pos_x		表示するx座標。
	*@param[in]	pos_y		表示するy座標。
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
				D3DCOLOR_XRGB(255, 255, 255)	// 白色
				);
		}
	}

}
