/*!
 *@brief	�X�P���g��
 */
#ifndef _TKSKELETON_H_
#define _TKSKELETON_H_


namespace tkEngine{
	/*!
	 *@brief	�X�P���g���N���X�B
	 */
	class CSkeleton : Noncopyable{
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CSkeleton();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CSkeleton();
	private:
		LPD3DXFRAME m_frameRoot;		//�t���[�����[�g
	};
}

#endif //#ifndef _TKSKELETON_H_


