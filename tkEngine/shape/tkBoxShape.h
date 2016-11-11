/*!
 * @brief	ボックス
 */

#ifndef _CBOXSHAPE_H_
#define _CBOXSHAPE_H_

#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/shape/tkShapeBase.h"

namespace tkEngine{
	/*!
	 * @brief	ボックス
	 */
	class CBoxShape : public CShapeBase{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CBoxShape();
		/*!
		 * @brief	デストラクタ。
		 */
		~CBoxShape();
		/*!
		 *@brief	カラーボックスを作成。
		 *@param[in]	size		サイズ。
		 *@param[in]	color		カラー。
		 *@param[in]	hasNormal	法線があるかどうかのフラグ。
		 */
		void Create( CVector3 size, int color, bool hasNormal );
	};
}

#endif // _CBOXSHAPE_H_