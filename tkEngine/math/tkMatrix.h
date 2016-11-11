/*!
 * @brief	�s��B
 */

#ifndef _TKMATRIX_H_
#define _TKMATRIX_H_

#include "tkEngine/math/tkVector.h"

namespace tkEngine{
	/*!
	 * @brief	�s��B
	 */
	class CMatrix{
	public:
		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
		static const CMatrix Identity;	//!<�P�ʍs��B
	public:
		/*!
		*@brief	�x�N�g����3x3�s��̏�Z
		*@param[in,out]		v	��Z�����x�N�g���B
		*/
		void Mul3x3(CVector3& vOut) const
		{
			CVector3 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2];
		}
		/*!
		*@brief	�x�N�g���ƍs��̏�Z
		*@param[in,out]		v	��Z�����x�N�g���B
		*/
		void Mul(CVector3& vOut) const
		{
			CVector3 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0] + m[3][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1] + m[3][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2] + m[3][2];
		}
		void Mul(CVector4& vOut) const
		{
			CVector4 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0] + vTmp.w * m[3][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1] + vTmp.w * m[3][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2] + vTmp.w * m[3][2];
			vOut.w = vTmp.x * m[0][3] + vTmp.y * m[1][3] + vTmp.z * m[2][3] + vTmp.w * m[3][3];
		}
		/*!
		 *@brief	���s�ړ��s����쐬�B
		 */
		void MakeTranslation( const CVector3& trans ) 
		{
			*this = Identity;
			m[3][0] = trans.x; 
			m[3][1] = trans.y;
			m[3][2] = trans.z;
		}
		/*!
		*@brief	Y������̉�]�s����쐬�B
		*@param[in]	angle	��]�p�x(�P�ʃ��W�A��)
		*/
		void MakeRotationY(float angle)
		{
			D3DXMatrixRotationY(
				r_cast<D3DXMATRIX*>(this),
				angle
			);
		}
		/*!
		*@brief	Z������̉�]�s����쐬�B
		*@param[in]	angle	��]�p�x(�P�ʃ��W�A��)
		*/
		void MakeRotationZ(float angle)
		{
			D3DXMatrixRotationZ(
				r_cast<D3DXMATRIX*>(this),
				angle
				);
		}
		/*!
		*@brief	X������̉�]�s����쐬�B
		*@param[in]	angle	��]�p�x(�P�ʃ��W�A��)
		*/
		void MakeRotationX(float angle)
		{
			D3DXMatrixRotationX(
				r_cast<D3DXMATRIX*>(this),
				angle
			);
		}
		/*!
		 *@brief	�N�H�[�^�j�I�������]�s����쐬�B
		 *@param[in]	q		�N�H�[�^�j�I��
		 */
		void MakeRotationFromQuaternion( const CQuaternion& q )
		{
			D3DXMatrixRotationQuaternion(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXQUATERNION*>(&q)
			);
		}
		/*!
		*@brief	�C�ӂ̎�����̉�]�s����쐬�B
		*@param[in]	axis	��]���B
		*@param[in]	angle	��]�p�x
		*/
		void MakeRotationAxis(const CVector3& axis, float angle )
		{
			D3DXMatrixRotationAxis(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXVECTOR3*>(&axis),
				angle);
		}
		/*!
		*@brief	�g��s����쐬�B
		*@param[in] scale		�g�嗦�B
		*/
		void MakeScaling(const CVector3& scale)
		{
			D3DXMatrixScaling(
				r_cast<D3DXMATRIX*>(this),
				scale.x,
				scale.y,
				scale.z
			);
		}
		/*!
		* @brief	�v���W�F�N�V�����s����쐬�B
		*@param[in]	viewAngle	��p�B(���W�A��)�B
		*@param[in]	aspect		�A�X�y�N�g��B
		*@param[in]	fNear		�ߕ��ʁB
		*@param[in]	fFar		�����ʁB
		*/
		void MakeProjectionMatrix(
			float viewAngle,
			float aspect,
			float fNear,
			float fFar
			)
		{
			D3DXMatrixPerspectiveFovLH(
				r_cast<D3DXMATRIX*>(this),
				viewAngle,
				aspect,
				fNear,
				fFar
			);
		}
		/*!
		* @brief	���s���e�s����쐬�B
		*@param[in]	w			�r���[�{�����[���̕��B
		*@param[in]	h			�r���[�{�����[���̍����B
		*@param[in]	fNear		�ߕ��ʁB
		*@param[in]	fFar		�����ʁB
		*/
		void MakeOrthoProjectionMatrix( float w, float h, float fNear, float fFar )
		{
			D3DXMatrixOrthoLH(r_cast<D3DXMATRIX*>(this), w, h, fNear, fFar);
		}
		/*!
		 * @brief	�����_�A������A�J�����ʒu����J�����s����쐬�B
		 *@param[in]	position	�J�����ʒu�B
		 *@param[in]	target		�����_�B
		 *@param[in]	up			�J�����̏�����B
		 */
		void MakeLookAt( const CVector3& position, const CVector3& target, const CVector3& up )
		{
			D3DXMatrixLookAtLH(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXVECTOR3*>(&position),
				r_cast<const D3DXVECTOR3*>(&target),
				r_cast<const D3DXVECTOR3*>(&up)
			);
		}
		/*!
		 *@brief	�s��ƍs��̏�Z
		 *@details
		 * *this = m0 * m1
		 */
		void Mul( const CMatrix& m0, const CMatrix& m1 )
		{
			D3DXMatrixMultiply(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXMATRIX*>(&m0),
				r_cast<const D3DXMATRIX*>(&m1)
			);
		}
		/*!
		 *@brief	�t�s����v�Z�B
		 *@param[in]	m	���ɂȂ�s��B
		 */
		void Inverse( const CMatrix& m )
		{
			D3DXMatrixInverse(
				r_cast<D3DXMATRIX*>(this),
				NULL,
				r_cast<const D3DXMATRIX*>(&m)
			);
		}
		void Transpose()
		{
			D3DXMatrixTranspose(r_cast<D3DXMATRIX*>(this), r_cast<D3DXMATRIX*>(this));
		}
		
	};
};
#endif // _TKMATRIX_H_