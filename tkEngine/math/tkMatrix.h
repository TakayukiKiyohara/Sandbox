/*!
 * @brief	行列。
 */

#ifndef _TKMATRIX_H_
#define _TKMATRIX_H_

#include "tkEngine/math/tkVector.h"

namespace tkEngine{
	/*!
	 * @brief	行列。
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
		static const CMatrix Identity;	//!<単位行列。
	public:
		/*!
		*@brief	ベクトルと3x3行列の乗算
		*@param[in,out]		v	乗算されるベクトル。
		*/
		void Mul3x3(CVector3& vOut) const
		{
			CVector3 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2];
		}
		/*!
		*@brief	ベクトルと行列の乗算
		*@param[in,out]		v	乗算されるベクトル。
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
		 *@brief	平行移動行列を作成。
		 */
		void MakeTranslation( const CVector3& trans ) 
		{
			*this = Identity;
			m[3][0] = trans.x; 
			m[3][1] = trans.y;
			m[3][2] = trans.z;
		}
		/*!
		*@brief	Y軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationY(float angle)
		{
			D3DXMatrixRotationY(
				r_cast<D3DXMATRIX*>(this),
				angle
			);
		}
		/*!
		*@brief	Z軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationZ(float angle)
		{
			D3DXMatrixRotationZ(
				r_cast<D3DXMATRIX*>(this),
				angle
				);
		}
		/*!
		*@brief	X軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationX(float angle)
		{
			D3DXMatrixRotationX(
				r_cast<D3DXMATRIX*>(this),
				angle
			);
		}
		/*!
		 *@brief	クォータニオンから回転行列を作成。
		 *@param[in]	q		クォータニオン
		 */
		void MakeRotationFromQuaternion( const CQuaternion& q )
		{
			D3DXMatrixRotationQuaternion(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXQUATERNION*>(&q)
			);
		}
		/*!
		*@brief	任意の軸周りの回転行列を作成。
		*@param[in]	axis	回転軸。
		*@param[in]	angle	回転角度
		*/
		void MakeRotationAxis(const CVector3& axis, float angle )
		{
			D3DXMatrixRotationAxis(
				r_cast<D3DXMATRIX*>(this),
				r_cast<const D3DXVECTOR3*>(&axis),
				angle);
		}
		/*!
		*@brief	拡大行列を作成。
		*@param[in] scale		拡大率。
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
		* @brief	プロジェクション行列を作成。
		*@param[in]	viewAngle	画角。(ラジアン)。
		*@param[in]	aspect		アスペクト比。
		*@param[in]	fNear		近平面。
		*@param[in]	fFar		遠平面。
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
		* @brief	平行投影行列を作成。
		*@param[in]	w			ビューボリュームの幅。
		*@param[in]	h			ビューボリュームの高さ。
		*@param[in]	fNear		近平面。
		*@param[in]	fFar		遠平面。
		*/
		void MakeOrthoProjectionMatrix( float w, float h, float fNear, float fFar )
		{
			D3DXMatrixOrthoLH(r_cast<D3DXMATRIX*>(this), w, h, fNear, fFar);
		}
		/*!
		 * @brief	注視点、上方向、カメラ位置からカメラ行列を作成。
		 *@param[in]	position	カメラ位置。
		 *@param[in]	target		注視点。
		 *@param[in]	up			カメラの上方向。
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
		 *@brief	行列と行列の乗算
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
		 *@brief	逆行列を計算。
		 *@param[in]	m	元になる行列。
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