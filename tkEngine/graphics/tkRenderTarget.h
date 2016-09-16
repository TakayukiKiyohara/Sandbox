/*!
 *@brief	�����_�����O�^�[�Q�b�g
 */
#ifndef _TKRENDERTARGET_H_
#define _TKRENDERTARGET_H_

#include "tkEngine/graphics/tkTexture.h"

namespace tkEngine{
	/*!
	 *@brief	�����_�����O�^�[�Q�b�g
	 */
	class CRenderTarget : Noncopyable{
	public:
		/*!
		 *@brief	�R���X�g���N�^
		 */
		CRenderTarget();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CRenderTarget();
		/*!
		 *@brief	�����_�����O�^�[�Q�b�g�̍쐬�B
		 *@param[in]	w					�����_�����O�^�[�Q�b�g�̕��B
		 *@param[in]	h					�����_�����O�^�[�Q�b�g�̍����B
		 *@param[in]	mipLevel			�~�b�v�}�b�v���x���B0���w�肵���ꍇ�̓~�b�v�}�b�v���T�|�[�g����Ă���GPU�ł�1*1�s�N�Z���܂ł̃~�b�v�}�b�v���쐬�����B
		 *@param[in]	colorFormat			�J���[�o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	depthStencilFormat	�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	multiSampleType		�}���`�T���v�����O�̎�ށB
		 *@param[in]	multiSampleQuality	�}���`�T���v�����O�̃N�I���e�B�B
		 */
		void Create(
			int w,
			int h,
			int mipLevel,
			EFormat colorFormat,
			EFormat depthStencilFormat,
			EMultisampleType multiSampleType,
			int multiSampleQuality
		);
		/*!
		 *@brief	�����_�����O�^�[�Q�b�g�̔j���B
		 */
		void Release();
		/*!
		*@brief	�e�N�X�`�����擾�B
		*/
		const CTexture* GetTexture() const
		{
			return &m_texture;
		}
		/*!
		*@brief	�����_�����O�^�[�Q�b�g�̕����擾�B
		*/
		int GetWidth() const
		{
			return m_width;
		}
		/*!
		*@brief	�����_�����O�^�[�Q�b�g�̍������擾�B
		*/
		int GetHeight() const
		{
			return m_height;
		}
		LPDIRECT3DSURFACE9 GetDepthSurfaceDx()
		{
			return m_depthSurface;
		}
		LPDIRECT3DSURFACE9 GetSurfaceDx()
		{
			return m_surface;
		}
		void SetDepthSurfaceDX(LPDIRECT3DSURFACE9 depthSurface)
		{
			m_depthSurface = depthSurface;
		}
		void SetSurfaceDX(LPDIRECT3DSURFACE9 surface)
		{
			m_surface = surface;
		}

	private:
		LPDIRECT3DSURFACE9		m_depthSurface;		//!<�[�x�o�b�t�@�p�̃T�[�t�F�C�X
		LPDIRECT3DTEXTURE9		m_textureDX;		//!<�������ݐ�̃e�N�X�`���B
		LPDIRECT3DSURFACE9		m_surface;			//!<�T�[�t�F�C�X
		CTexture				m_texture;			//!<�e�N�X�`���B
		int						m_width;			//!<���B
		int						m_height;			//!<�����B
	};
}
#endif // _TKRENDERTARGET_H_