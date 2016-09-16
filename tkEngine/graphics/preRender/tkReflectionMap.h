/*!
 *@brief	���t���N�V�����}�b�v
 */

#ifndef TKREFLECTIONMAP_H
#define TKREFLECTIONMAP_H

namespace tkEngine{
	class CSkinModel;
	/*!
	 *@brief	���t���N�V�����}�b�v�B
	 */
	class CReflectionMap : Noncopyable{
	public: 
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CReflectionMap();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CReflectionMap();
		/*!
		 *@brief	�������B
		 */
		void Init(int w, int h);
		/*!
		*@brief	�J���B
		*/
		void Release();
		/*!
		 *@brief	�`��B
		 */
		void Render(CRenderContext& renderContext);
		/*!
		*@brief	���t���N�V�����}�b�v���L��������B
		*/
		bool IsEnable() const
		{
			return m_isEnable;
		}
		/*!
		*@brief	���t���N�V�����}�b�v�ւ̕`��G���g���[�B
		*/
		void Entry(CSkinModel* model)
		{
			m_models.push_back(model);
		}
		/*!
		*@brief	�J������ݒ�B
		*/
		void SetCamera(const CCamera& camera)
		{
			m_camera = &camera;
		}
	private:
		CRenderTarget				m_renderTarget;		//!<�����_�����O�^�[�Q�b�g�B
		std::vector<CSkinModel*>	m_models;			//!<���t���N�V�����}�b�v�ɕ`�悷��X�L�����f���B
		bool						m_isEnable;			//!<���t���N�V�����}�b�v�̗L���t���O�B
		const CCamera*				m_camera;			//!<�J�����B
	};
}

#endif //TKREFLECTIONMAP_H