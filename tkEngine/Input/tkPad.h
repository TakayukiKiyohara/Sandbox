/*!
 *@brief	�p�b�h
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	���z�{�^����`�B
	 */
	enum EnButton{
		enButtonUp,		//!<��B
		enButtonDown,		//!<���B
		enButtonLeft,		//!<���B
		enButtonRight,		//!<�E�B
		enButtonA,			//!<A�{�^���B
		enButtonB,			//!<B�{�^���B
		enButtonX,			//!<X�{�^���B
		enButtonY,			//!<Y�{�^���B
		enButtonSelect,	//!<�Z���N�g�{�^���B
		enButtonStart,		//!<�X�^�[�g�{�^���B
		enButtonRB1,		//!<RB1�{�^���B
		enButtonRB2,		//!<RB2�{�^���B
		enButtonRB3,		//!<RB3�{�^���B
		enButtonLB1,		//!<LB1�{�^���B
		enButtonLB2,		//!<LB2�{�^���B
		enButtonLB3,		//!<LB3�{�^���B
		enButtonNum,	//!<�{�^���̐��B
	};
	/*!
	 *@brief	�p�b�h
	 */
	class CPad : Noncopyable{
	public:
		/*!
		 *@brief	�p�b�h�X�e�[�g�B
		 */
		struct PAD_STATE
		{
			XINPUT_STATE state;
			bool bConnected;
		};
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CPad();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CPad();
		/*!
		*@brief	�������B
		*@param[in]	padNo	�p�b�h�ԍ��B
		*/
		void Init(int padNo)
		{
			m_padNo = padNo;
		}
		/*!
		*@brief	�X�V�B
		*/
		void Update();
		/*!
		 *@brief	�{�^���̃g���K�[����B
		 *@return	true���Ԃ��Ă�����g���K�[���́B
		 */
		bool IsTrigger(EnButton button) const
		{
			TK_ASSERT( button < enButtonNum, "button is invalid" );
			return m_trigger[button] != 0;
		}
		/*!
		 *@brief	�{�^����������Ă��邩����B
		 *@return	true���Ԃ��Ă����牟����Ă���B
		 */
		bool IsPress(EnButton button) const
		{
			TK_ASSERT(button < enButtonNum, "button is invalid");
			return m_press[button] != 0;
		}
		/*!
		*@brief	���X�e�B�b�N��X���̓��͗ʂ��擾�B
		*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
		*/
		float GetLStickXF() const
		{
			return m_lStickX;
		}
		/*!
		*@brief	���X�e�B�b�N��Y���̓��͗ʂ��擾�B
		*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
		*/
		float GetLStickYF() const
		{
			return m_lStickY;
		}
		/*!
		*@brief	�E�X�e�B�b�N��X���̓��͗ʂ��擾�B
		*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
		*/
		float GetRStickXF() const
		{
			return m_rStickX;
		}
		/*!
		*@brief	�E�X�e�B�b�N��Y���̓��͗ʂ��擾�B
		*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
		*/
		float GetRStickYF() const
		{
			return m_rStickY;
		}
	private:
		PAD_STATE m_state;	//!<�p�b�h�X�e�[�g�B
		int m_padNo;		//!<�p�b�h�ԍ��B
		int m_trigger[enButtonNum];	//!<�g���K�[���͂̃t���O�B
		int m_press[enButtonNum];	//!<press���͂̃t���O�B
		float m_lStickX;			//!<���X�e�B�b�N��X���̓��͗ʁB
		float m_lStickY;			//!<���X�e�B�b�N��Y���̓��͗ʁB
		float m_rStickX;			//!<�E�X�e�B�b�N��X���̓��͗ʁB
		float m_rStickY;			//!<�E�X�e�B�b�N��Y���̓��͗ʁB
	};
}