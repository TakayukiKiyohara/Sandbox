/*!
*@brief	�X�L�����f��
*/
#ifndef _TKSKINMODEL_H_
#define _TKSKINMODEL_H_

#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/prerender/tkShadowCaster.h"

namespace tkEngine {
	class CEffect;
	class CSkinModelData;
	class CRenderContext;
	class CLight;
	class CShadowMap;
	enum EFogFunc {
		enFogFuncNone,		//�t�H�O�Ȃ��B
		enFogFuncDist,		//�����t�H�O�B
		enFogFuncHeight,	//�����t�H�O�B
		enFogFuncNum,
	};
	/*!
	*@brief	�X�L�����f��
	*/
	class CSkinModel {
	public:
		
		CSkinModel();
		~CSkinModel();
		/*!
		*@brief	������
		*/
		void Init(CSkinModelData* modelData);
		/*!
		*@brief	�`��
		*/
		void Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		*@brief	�C�~�f�B�G�C�g�`��B
		*@details
		* ���̊֐������s����Ƒ��`��R�}���h��GPU�ɑ����܂��B
		*/
		void ImmidiateDraw(LPDIRECT3DDEVICE9 pd3ddevice, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, bool isDrawToShadowMap);
		/*!
		*@brief	�X�V�B
		*@details
		*@param[in]		trans	���s�ړ��B
		*@param[in]		rot		��]�B
		*@param[in]		scale	�g��B
		*/
		void Update( const CVector3& trans, const CQuaternion& rot, const CVector3& scale );
		/*!
		* @brief	�C���X�^���V���O�`��p�̃f�[�^���X�V�B
		*@details
		* ���_�X�g���[���P�ɗ������ޒ��_�o�b�t�@�ɐݒ肷��f�[�^�ɂȂ�܂��B
		*@param[in]		data		���_�o�b�t�@�ɃR�s�[����f�[�^�B
		*/
		void UpdateInstancingDrawData(const void* data)
		{
			m_skinModelData->UpdateInstancingDrawData(data);
		}
		/*!
		*@brief	���C�g��ݒ�B
		*/
		void SetLight(CLight* light)
		{
			m_light = light;
		}
		/*!
		*@brief	���x�}�b�v�ւ̏������݃t���O��ݒ�B
		*/
		void SetWriteVelocityMap(bool flag)
		{
			m_isWriteVelocityMap = flag;
		}
		/*!
		*@brief	�@���}�b�v�̕ێ��t���O��ݒ�B
		*/
		void SetHasNormalMap(bool hasNormalMap)
		{
			m_hasNormalMap = hasNormalMap;
		}
		/*!
		*@brief	�X�y�L�����}�b�v�̕ێ��t���O��ݒ�B
		*/
		void SetHasSpeculerMap(bool hasSpecMap)
		{
			m_hasSpecMap = hasSpecMap;
		}
		/*!
		 * @brief	�V���h�E�L���X�^�[�̃t���O��ݒ�B
		 *@param[in]	flag	�V���h�E�L���X�^�[�̃t���O�B
		 */
		void SetShadowCasterFlag( bool flag )
		{
			m_isShadowCaster = flag;
		}
		/*!
		* @brief	�V���h�E���V�[�o�[�̃t���O��ݒ�B
		*@param[in]	flag	�V���h�E���V�[�o�[�̃t���O�B
		*/
		void SetShadowReceiverFlag(bool flag)
		{
			m_isShadowReceiver = flag;
		}
		/*!
		 *@brief	���t���N�V�����}�b�v�ւ̕`�����݃t���O��ݒ�B
		 */
		void SetReflectionCasterFlag( bool flag )
		{
			m_isReflectionCaster = flag;
		}
		/*!
		* @brief	�t���l���t���O��ݒ�B
		*@param[in]	flag	�t���l���̃t���O�B
		*/
		void SetFresnelFlag(bool flag)
		{
			m_isFresnel = flag;
		}
		/*!
		* @brief	�t�H�O�p�����[�^��ݒ�B
		*@param[in]	fogFunc		�t�H�O�̎�ށBEFogFunc���Q�ƁB
		*@param[in]	param0		�t�H�O�p�����[�^0
		* fogFunc��enFogFuncDist�̏ꍇ�̓t�H�O���|����n�߂鋗���AfogFunc��enFogFuncHeight�̏ꍇ�̓t�H�O���|����n�߂鍂���B
		*@param[in]	param1		�t�H�O�p�����[�^�P
		* fogFunc��enFogFuncDist�̏ꍇ�̓t�H�O���|����؂鋗���AfogFunc��enFogFuncHeight�̏ꍇ�̓t�H�O���|����؂鍂���B
		*/
		void SetFogParam(EFogFunc fogFunc, float param0, float param1)
		{
			TK_ASSERT(fogFunc < enFogFuncNum, "fogFunc is invalid");
			m_fogFunc = fogFunc;
			m_fogParam[0] = param0;
			m_fogParam[1] = param1;
		}
		/*!
		* @brief	���[���h�s����擾�B
		*/
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		* @brief	�擪�̃��b�V�����擾�B
		*/
		LPD3DXMESH GetOrgMeshFirst() const
		{
			return m_skinModelData->GetOrgMeshFirst();
		}
		
		/*!
		* @brief	���̃��[���h�s�������
		*@details
		* ���O�Ō������s���Ă��邽�߁A�x���ł��B�p�ɂɌĂ΂Ȃ��悤�ɁB
		*@param[in]	boneName	���̖��O�B
		*@return ���������獜�̃��[���h�s��̃A�h���X��Ԃ��B������Ȃ������ꍇ��NULL��Ԃ��B
		*/
		CMatrix* FindBoneWorldMatrix(const char* boneName) 
		{
			return m_skinModelData->FindBoneWorldMatrix(boneName);
		}
	private:
		/*!
		* @brief	�V�F�[�_��̐��n���h�����������B
		*/
		void InitShaderConstHandle();
		void DrawMeshContainer(
			IDirect3DDevice9* pd3dDevice,
			LPD3DXMESHCONTAINER pMeshContainerBase,
			LPD3DXFRAME pFrameBase,
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix,
			D3DXMATRIX* projMatrix,
			CLight* light,
			bool isInstancingDraw,
			bool isDrawToShadowMap
		);
		void DrawFrame(
			IDirect3DDevice9* pd3dDevice,
			LPD3DXFRAME pFrame,
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix,
			D3DXMATRIX* projMatrix,
			bool isInstancingDraw,
			bool isDrawToShadowMap
		);
		//DrawMeshContainer����Ă΂��C���X�^���V���O�`��̋��ʏ����B
		void DrawMeshContainer_InstancingDrawCommon(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* meshContainer, int materialID);		
	public:
		/*!
		*@brief	�V���h�E�}�b�v�ɕ`��
		*@details
		* CShadowMap����Ăяo����܂��B���[�U�[�͎g�p���Ȃ��悤�ɁB
		*/
		void DrawToShadowMap(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
	private:
		enum EnShaderHandle {
			enShaderHandleLastFrameViewProj,	//1�t���[���O�̃r���[�v���W�F�N�V�����B	
			enShaderHandleViewProj,				//�r���[�v���W�F�N�V�����B	
			enShaderHandleLight,				//���C�g�B
			enShaderHandleLVP,					//���C�g�r���[�v���W�F�N�V�����s��B
			enShaderHandleCameraPos,			//�J�����̈ʒu�B
			enShaderHandleFlags,				//g_flags
			enShaderHandleFlags2,				//g_flags2
			enShaderHandleFarNear,				//FarNear
			enShaderHandleFogParam,				//Fog�p�����[�^�B
			enShaderHandleWorldMatrixArray,		//�{�[���s��
			enShaderHandleNumBone,				//�{�[���̐��B
			enShaderHandleCurNumBones,			//�X�L���O���s�Ȃ��{�[���̐��B
			enShaderHandleViewMatrixRotInv,		//�r���[�s��̉�]�����̋t�s��B
			enShaderHandleWorldMatrix,			//���[���h�s��
			enShaderHandleRotationMatrix,		//��]�s��B
			enShaderHandleShadowMap_0,			//�V���h�E�}�b�v
			enShaderHandleShadowMap_1,			//�V���h�E�}�b�v
			enShaderHandleShadowMap_2,		//�V���h�E�}�b�v
			enShaderHandleNormalTexture,	//�@���}�b�v�B
			enShaderHandleSpeculerMap,		//�X�y�L�����}�b�v�B
			enShaderHandleDiffuseTexture,	//�f�B�t���[�Y�e�N�X�`���B
			enShaderHandleCameraDir,		//�J�����̕����B
			enShaderHandleShadowRecieverParam,	//�V���h�E���V�[�o�[�p�̃p�����[�^�B
			enShaderHandleTec_SkinModelInstancingRenderToShadowMap,		//SkinModelInstancingRenderToShadowMap�e�N�j�b�N
			enShaderHandleTec_SkinModelInstancing,						//SkinModelInstancing�e�N�j�b�N�B
			enShaderHandleTec_NoSkinModelInstancingRenderToShadowMap,	//NoSkinModelInstancingRenderToShadowMap�e�N�j�b�N�B
			enShaderHandleTec_NoSkinModelInstancing,					//NoSkinModelInstancing�e�N�j�b�N�B
			enShaderHandleTec_SkinModelRenderShadowMap,					//SkinModelRenderShadowMap�e�N�j�b�N�B
			enShaderHandleTec_SkinModel,								//SkinModel�e�N�j�b�N�B
			enShaderHandleTec_NoSkinModelRenderShadowMap,				//NoSkinModelRenderShadowMap�e�N�j�b�N�B
			enShaderHandleTec_NoSkinModel,								//NoSkinModel�e�N�j�b�N�B
			enShaderHandleNum,				//�V�F�[�_�[�n���h���̐��B
		};
		CMatrix							m_worldMatrix;		//!<���[���h�s��B
		CMatrix							m_rotationMatrix;	//!<��]�s��B
		CSkinModelData*					m_skinModelData;	//!<�X�L�����f���f�[�^�B
		CEffect*						m_pEffect;			//!<�G�t�F�N�g�B
		CAnimation						m_animation;		//!<�A�j���[�V�����B
		CLight*							m_light;			//!<���C�g�B
		static const int MAX_MATRIX_PALLET = 128;	//!<�}�g���N�X�p���b�g�̍ő吔�B
		D3DXMATRIX						m_boneMatrixPallet[MAX_MATRIX_PALLET];	//!<�}�g���N�X�p���b�g�B
		bool							m_isShadowReceiver;					//!<�V���h�E���V�[�o�[�B
		bool							m_isShadowCaster;					//!<�V���h�E�L���X�^�[�t���O�B
		bool							m_isReflectionCaster;				//!<���t���N�V�����}�b�v�ɕ`�����ރt���O�B
		bool							m_isFresnel;						//!<�t���l��
		EFogFunc						m_fogFunc;							//!<�t�H�O�̎�ށB0�Ȃ�t�H�O�Ȃ��A1�Ȃ狗���t�H�O�A2�Ȃ獂���t�H�O�B
		float							m_fogParam[2];						//!<�t�H�O�̃p�����[�^
		CShadowCaster_SkinModel			m_shadowCaster;						//!<�V���h�E�L���X�^�[�B
		D3DXHANDLE						m_hShaderHandle[enShaderHandleNum];	//!<�V�F�[�_�[�n���h���B
		bool							m_hasNormalMap;						//!<�@���}�b�v��ێ����Ă���H
		bool							m_hasSpecMap;						//!<�X�y�����}�b�v��ێ����Ă���H
		bool							m_isWriteVelocityMap = true;		//!<���x�}�b�v�ɏ������ށH
	};
}

#endif // #ifndef _TKSKINMODEL_H_