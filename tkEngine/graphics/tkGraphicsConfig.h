/*!
 * @brief	�O���t�B�b�N�X�R���t�B�O�B
 */

#ifndef _TKGRAPHICSCONFIG_H_
#define _TKGRAPHICSCONFIG_H_

namespace tkEngine{
	/*!
	 * @brief	�G�b�W���o�����̃R���t�B�O
	 */
	struct SEdgeRenderConfig{
		bool isEnable;		//!<�G�b�W���o�������L�����ǂ����̃t���O�B
		int	idMapWidth;		//!<ID�}�b�v�̕��B
		int idMapHeight;	//!<ID�}�b�v�̍����B
	};
	
	/*!
	 * @brief	�e�����̃R���t�B�O�B
	 */
	struct SShadowRenderConfig{
		void Init()
		{
			isEnable = false;
			shadowMapWidth = 512;
			shadowMapHeight = 512;
			isDisableSoftShadow = false;
			numShadowMap = 1;
		}
		bool 	isEnable;				//!<�e�𗎂Ƃ��������L�����ǂ����̃t���O�B
		int		shadowMapWidth;			//!<�V���h�E�}�b�v�̕��B
		int		shadowMapHeight;		//!<�V���h�E�}�b�v�̍����B
		bool	isDisableSoftShadow;	//!<�\�t�g�V���h�E���������ǂ����̃t���O�B
		int		numShadowMap;			//!<�V���h�E�}�b�v�̖����B(�ő�4��)
	};
	/*!
	* @brief	Bloom�̃R���t�B�O
	*/
	struct SBloomConfig {
		bool	isEnable;		//!<�u���[�����L�����ǂ����̃t���O�B
	};
	/*!
	* @brief	���t���N�V�����}�b�v�̃R���t�B�O
	*/
	struct SReflectionMapConfig {
		bool	isEnable;				//!<���t���N�V�����}�b�v���L�����ǂ����̃t���O�B
		int		reflectionMapWidth;		//!<���t���N�V�����}�b�v�̕��B
		int		reflectionMapHeight;	//!<���t���N�V�����}�b�v�̍����B
	};
	/*!
	* @brief	��ʊE�[�x�̃R���t�B�O�B
	*/
	struct SDofConfig {
		bool isEnable;					//��ʊE�[�x���L�����ǂ����̃t���O�B
	};
	/*!
	* @brief	�A���`�G�C���A�X�̃R���t�B�O�B
	*/
	struct SAAConfig {
		bool isEnable;					//�A���`�G�C���A�X���L�����ǂ����̃t���O�B
	};
	/*!
	* @brief	���[�V�����u���[�̃R���t�B�O�B
	*/
	struct SMotionBlurConfig {
		bool isEnable;
	};
	/*!
	 * @brief	�O���t�B�b�N�X�R���t�B�O�B
	 */
	struct SGraphicsConfig{
		SEdgeRenderConfig		edgeRenderConfig;		//!<�G�b�W���o�����̃R���t�B�O�B
		SShadowRenderConfig		shadowRenderConfig;		//!<�e�̏����̃R���t�B�O�B
		SBloomConfig			bloomConfig;			//!<Bloom�̃R���t�B�O�B
		SReflectionMapConfig	reflectionMapConfig;	//!<���t���N�V�����}�b�v�̃R���t�B�O�B
		SDofConfig				dofConfig;				//!<��ʊE�[�x�̃R���t�B�O�B
		SAAConfig				aaConfig;				//!<�A���`�G�C���A�X�̃R���t�B�O�B
		SMotionBlurConfig		motionBlurConfig;		//<!���[�V�����u���[�̃R���t�B�O�B
	};
}

#endif //_TKGRAPHICSCONFIG_H_