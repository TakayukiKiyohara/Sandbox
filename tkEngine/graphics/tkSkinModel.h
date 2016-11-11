/*!
*@brief	スキンモデル
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
		enFogFuncNone,		//フォグなし。
		enFogFuncDist,		//距離フォグ。
		enFogFuncHeight,	//高さフォグ。
		enFogFuncNum,
	};
	/*!
	*@brief	スキンモデル
	*/
	class CSkinModel {
	public:
		
		CSkinModel();
		~CSkinModel();
		/*!
		*@brief	初期化
		*/
		void Init(CSkinModelData* modelData);
		/*!
		*@brief	描画
		*/
		void Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		*@brief	イミディエイト描画。
		*@details
		* この関数を実行すると即描画コマンドがGPUに送られます。
		*/
		void ImmidiateDraw(LPDIRECT3DDEVICE9 pd3ddevice, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, bool isDrawToShadowMap);
		/*!
		*@brief	更新。
		*@details
		*@param[in]		trans	平行移動。
		*@param[in]		rot		回転。
		*@param[in]		scale	拡大。
		*/
		void Update( const CVector3& trans, const CQuaternion& rot, const CVector3& scale );
		/*!
		* @brief	インスタンシング描画用のデータを更新。
		*@details
		* 頂点ストリーム１に流し込む頂点バッファに設定するデータになります。
		*@param[in]		data		頂点バッファにコピーするデータ。
		*/
		void UpdateInstancingDrawData(const void* data)
		{
			m_skinModelData->UpdateInstancingDrawData(data);
		}
		/*!
		*@brief	ライトを設定。
		*/
		void SetLight(CLight* light)
		{
			m_light = light;
		}
		/*!
		*@brief	速度マップへの書き込みフラグを設定。
		*/
		void SetWriteVelocityMap(bool flag)
		{
			m_isWriteVelocityMap = flag;
		}
		/*!
		*@brief	法線マップの保持フラグを設定。
		*/
		void SetHasNormalMap(bool hasNormalMap)
		{
			m_hasNormalMap = hasNormalMap;
		}
		/*!
		*@brief	スペキュラマップの保持フラグを設定。
		*/
		void SetHasSpeculerMap(bool hasSpecMap)
		{
			m_hasSpecMap = hasSpecMap;
		}
		/*!
		 * @brief	シャドウキャスターのフラグを設定。
		 *@param[in]	flag	シャドウキャスターのフラグ。
		 */
		void SetShadowCasterFlag( bool flag )
		{
			m_isShadowCaster = flag;
		}
		/*!
		* @brief	シャドウレシーバーのフラグを設定。
		*@param[in]	flag	シャドウレシーバーのフラグ。
		*/
		void SetShadowReceiverFlag(bool flag)
		{
			m_isShadowReceiver = flag;
		}
		/*!
		 *@brief	リフレクションマップへの描きこみフラグを設定。
		 */
		void SetReflectionCasterFlag( bool flag )
		{
			m_isReflectionCaster = flag;
		}
		/*!
		* @brief	フレネルフラグを設定。
		*@param[in]	flag	フレネルのフラグ。
		*/
		void SetFresnelFlag(bool flag)
		{
			m_isFresnel = flag;
		}
		/*!
		* @brief	フォグパラメータを設定。
		*@param[in]	fogFunc		フォグの種類。EFogFuncを参照。
		*@param[in]	param0		フォグパラメータ0
		* fogFuncがenFogFuncDistの場合はフォグが掛かり始める距離、fogFuncがenFogFuncHeightの場合はフォグが掛かり始める高さ。
		*@param[in]	param1		フォグパラメータ１
		* fogFuncがenFogFuncDistの場合はフォグが掛かり切る距離、fogFuncがenFogFuncHeightの場合はフォグが掛かり切る高さ。
		*/
		void SetFogParam(EFogFunc fogFunc, float param0, float param1)
		{
			TK_ASSERT(fogFunc < enFogFuncNum, "fogFunc is invalid");
			m_fogFunc = fogFunc;
			m_fogParam[0] = param0;
			m_fogParam[1] = param1;
		}
		/*!
		* @brief	ワールド行列を取得。
		*/
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		* @brief	先頭のメッシュを取得。
		*/
		LPD3DXMESH GetOrgMeshFirst() const
		{
			return m_skinModelData->GetOrgMeshFirst();
		}
		
		/*!
		* @brief	骨のワールド行列を検索
		*@details
		* 名前で検索を行っているため、遅いです。頻繁に呼ばないように。
		*@param[in]	boneName	骨の名前。
		*@return 見つかったら骨のワールド行列のアドレスを返す。見つからなかった場合はNULLを返す。
		*/
		CMatrix* FindBoneWorldMatrix(const char* boneName) 
		{
			return m_skinModelData->FindBoneWorldMatrix(boneName);
		}
	private:
		/*!
		* @brief	シェーダ定の数ハンドルを初期化。
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
		//DrawMeshContainerから呼ばれるインスタンシング描画の共通処理。
		void DrawMeshContainer_InstancingDrawCommon(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* meshContainer, int materialID);		
	public:
		/*!
		*@brief	シャドウマップに描画
		*@details
		* CShadowMapから呼び出されます。ユーザーは使用しないように。
		*/
		void DrawToShadowMap(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
	private:
		enum EnShaderHandle {
			enShaderHandleLastFrameViewProj,	//1フレーム前のビュープロジェクション。	
			enShaderHandleViewProj,				//ビュープロジェクション。	
			enShaderHandleLight,				//ライト。
			enShaderHandleLVP,					//ライトビュープロジェクション行列。
			enShaderHandleCameraPos,			//カメラの位置。
			enShaderHandleFlags,				//g_flags
			enShaderHandleFlags2,				//g_flags2
			enShaderHandleFarNear,				//FarNear
			enShaderHandleFogParam,				//Fogパラメータ。
			enShaderHandleWorldMatrixArray,		//ボーン行列
			enShaderHandleNumBone,				//ボーンの数。
			enShaderHandleCurNumBones,			//スキングを行なうボーンの数。
			enShaderHandleViewMatrixRotInv,		//ビュー行列の回転成分の逆行列。
			enShaderHandleWorldMatrix,			//ワールド行列
			enShaderHandleRotationMatrix,		//回転行列。
			enShaderHandleShadowMap_0,			//シャドウマップ
			enShaderHandleShadowMap_1,			//シャドウマップ
			enShaderHandleShadowMap_2,		//シャドウマップ
			enShaderHandleNormalTexture,	//法線マップ。
			enShaderHandleSpeculerMap,		//スペキュラマップ。
			enShaderHandleDiffuseTexture,	//ディフューズテクスチャ。
			enShaderHandleCameraDir,		//カメラの方向。
			enShaderHandleShadowRecieverParam,	//シャドウレシーバー用のパラメータ。
			enShaderHandleTec_SkinModelInstancingRenderToShadowMap,		//SkinModelInstancingRenderToShadowMapテクニック
			enShaderHandleTec_SkinModelInstancing,						//SkinModelInstancingテクニック。
			enShaderHandleTec_NoSkinModelInstancingRenderToShadowMap,	//NoSkinModelInstancingRenderToShadowMapテクニック。
			enShaderHandleTec_NoSkinModelInstancing,					//NoSkinModelInstancingテクニック。
			enShaderHandleTec_SkinModelRenderShadowMap,					//SkinModelRenderShadowMapテクニック。
			enShaderHandleTec_SkinModel,								//SkinModelテクニック。
			enShaderHandleTec_NoSkinModelRenderShadowMap,				//NoSkinModelRenderShadowMapテクニック。
			enShaderHandleTec_NoSkinModel,								//NoSkinModelテクニック。
			enShaderHandleNum,				//シェーダーハンドルの数。
		};
		CMatrix							m_worldMatrix;		//!<ワールド行列。
		CMatrix							m_rotationMatrix;	//!<回転行列。
		CSkinModelData*					m_skinModelData;	//!<スキンモデルデータ。
		CEffect*						m_pEffect;			//!<エフェクト。
		CAnimation						m_animation;		//!<アニメーション。
		CLight*							m_light;			//!<ライト。
		static const int MAX_MATRIX_PALLET = 128;	//!<マトリクスパレットの最大数。
		D3DXMATRIX						m_boneMatrixPallet[MAX_MATRIX_PALLET];	//!<マトリクスパレット。
		bool							m_isShadowReceiver;					//!<シャドウレシーバー。
		bool							m_isShadowCaster;					//!<シャドウキャスターフラグ。
		bool							m_isReflectionCaster;				//!<リフレクションマップに描きこむフラグ。
		bool							m_isFresnel;						//!<フレネル
		EFogFunc						m_fogFunc;							//!<フォグの種類。0ならフォグなし、1なら距離フォグ、2なら高さフォグ。
		float							m_fogParam[2];						//!<フォグのパラメータ
		CShadowCaster_SkinModel			m_shadowCaster;						//!<シャドウキャスター。
		D3DXHANDLE						m_hShaderHandle[enShaderHandleNum];	//!<シェーダーハンドル。
		bool							m_hasNormalMap;						//!<法線マップを保持している？
		bool							m_hasSpecMap;						//!<スペきゅらマップを保持している？
		bool							m_isWriteVelocityMap = true;		//!<速度マップに書き込む？
	};
}

#endif // #ifndef _TKSKINMODEL_H_