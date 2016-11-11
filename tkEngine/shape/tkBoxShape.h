/*!
 * @brief	�{�b�N�X
 */

#ifndef _CBOXSHAPE_H_
#define _CBOXSHAPE_H_

#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/shape/tkShapeBase.h"

namespace tkEngine{
	/*!
	 * @brief	�{�b�N�X
	 */
	class CBoxShape : public CShapeBase{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CBoxShape();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CBoxShape();
		/*!
		 *@brief	�J���[�{�b�N�X���쐬�B
		 *@param[in]	size		�T�C�Y�B
		 *@param[in]	color		�J���[�B
		 *@param[in]	hasNormal	�@�������邩�ǂ����̃t���O�B
		 */
		void Create( CVector3 size, int color, bool hasNormal );
	};
}

#endif // _CBOXSHAPE_H_