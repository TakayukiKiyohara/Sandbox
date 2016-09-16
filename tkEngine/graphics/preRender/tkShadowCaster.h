/*!
 * @brief	�V���h�E�L���X�^�[�B
 */
 
#pragma once

namespace tkEngine{
	class CEffect;
	/*!
	 * @brief	�V���h�E�L���X�^�[�̃C���^�[�t�F�[�X�N���X�B
	 */
	class IShadowCaster{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		IShadowCaster()
		{
		}
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		virtual ~IShadowCaster()
		{
		}
		/*!
		* @brief	�`��B
		*/
		virtual void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP) = 0;
	};
	/*!
	 * @brief	�V���h�E�L���X�^�[(�v���~�e�B�u��)
	 */
	class CShadowCaster_Primitive : public IShadowCaster{
	public:
		CShadowCaster_Primitive();
		~CShadowCaster_Primitive();
		/*!
		 * @brief	�v���~�e�B�u����쐬�B
		 */
		void Create( CPrimitive* prmi );
		
		/*!
		 * @brief	���[���h�s���ݒ�B
		 */
		void SetWorldMatrix( const CMatrix& mWorld )
		{
			m_mWorld = mWorld;
		}
		/*!
		* @brief	�`��B
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP);
	private:
		CPrimitive*		m_prim;			//!<�v���~�e�B�u�B
		CMatrix			m_mWorld;		//!<���[���h�s���ݒ�B
	};
	/*!
	* @brief	�V���h�E�L���X�^�[(CSkinModel��)
	*/
	class CShadowCaster_SkinModel : public IShadowCaster {
	public:
		CShadowCaster_SkinModel();
		~CShadowCaster_SkinModel();
		/*!
		* @brief	�X�L�����f������쐬�B
		*/
		void Create(CSkinModel* model);
		/*!
		* @brief	�`��B
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP);
	private:
		CSkinModel*		m_skinModel;	//!<�X�L�����f���B
	};
}
