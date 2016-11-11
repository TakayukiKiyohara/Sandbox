/*!
 *@brief	�X�v���C�g�\���N���X�B
 */

#pragma once

namespace tkEngine {
	/*!
	 *@brief	�X�v���C�g�\���N���X�B
	 */
	class CSprite {
	public:
		CSprite();
		~CSprite();
		/*!
		 *@brief	�������B
		 */
		void Init(CTexture* texture);
		/*!
		*@brief	�e�N�X�`����ݒ�B
		*/
		void SetTexture(CTexture* texture)
		{
			m_texture = texture;
		}
		/*!
		*@brief	�摜�̈ʒu��ݒ�B
		*@details
		* ��ʂ̒��S��0,0�ƂȂ�܂��B
		* ���W�n�͏������+y�A�E������+x�ɂȂ�܂��B
		* Unity��uGUI�ɏ����B
		*
		*@param[in]		pos		���W�B
		*/
		void SetPosition(const CVector2& pos)
		{
			m_pos = pos;
		}
		/*!
		*@brief	�摜�̈ʒu���擾�B
		*/
		const CVector2& GetPosition() const
		{
			return m_pos;
		}
		/*!
		*@brief	�X�v���C�g�̃T�C�Y��ݒ�B
		*@param[in]	size		�T�C�Y�B
		*/
		void SetSize(const CVector2& size)
		{
			m_size = size;
		}
		/*!
		*@brief	�X�v���C�g�̃T�C�Y���擾�B
		*/
		const CVector2& GetSize() const
		{
			return m_size;
		}
		/*!
		*@brief	�s�{�b�g��ݒ�B
		*@details
		* 0.5, 0.5�ŉ摜�̒��S����_�B
		* 0.0, 0.0�ŉ摜�̍����B
		* 1.0, 1.0�ŉ摜�̉E��B
		* Unity��uGUI�ɏ����B
		*@param[in]	pivot		�T�C�Y�B
		*/
		void SetPivot(const CVector2& pivot)
		{
			m_pivot = pivot;
		}
		void SetRotation( CQuaternion rot)
		{
			m_rotation = rot;
		}
		/*!
		 *@brief	�`��B
		 *@param[in]	renderContext	�����_�����O�R���e�L�X�g�B
		 */
		void Draw(CRenderContext& renderContext);
	private:
		CEffect*		m_effect = nullptr;		//!<�G�t�F�N�g�B
		CPrimitive		m_primitive;			//!<�v���~�e�B�u�B
		CTexture*		m_texture = nullptr;	//<!�e�N�X�`���B
		CVector2		m_pos = CVector2(0.0f, 0.0f);	//!<���W�B
		CVector2		m_size = CVector2(0.0f, 0.0f);	//!<�X�v���C�g�̃T�C�Y�B
		CVector2		m_pivot = CVector2(0.5f, 0.5f);	//!<�s�{�b�g
		CQuaternion		m_rotation = CQuaternion::Identity;
	};
}