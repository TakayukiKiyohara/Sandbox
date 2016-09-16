/*!
 *@brief	ShapeVertex。
 */

#ifndef _TKSHAPE_H_
#define _TKSHAPE_H_

namespace tkEngine{
	/*!
	*@brief	座標とUV座標
	*/
	struct SShapeVertex_PT {
		float		pos[4];
		float		uv[2];
	};
	static const SVertexElement scShapeVertex_PT_Element[] = {
		{ 0, 0 ,   DECLTYPE_FLOAT4		, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION	, 0 },
		{ 0, 16 ,  DECLTYPE_FLOAT2		, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD	, 0 },
		D3DDECL_END()
	};
	/*!
	 *@brief	カラー。
	 */
	struct SShapeVertex_PC{
		float		pos[4];
		int		color;
	}; 
	static const SVertexElement scShapeVertex_PC_Element[] = {
		{ 0, 0 ,   DECLTYPE_FLOAT4		, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION	, 0 },
		{ 0, 16 ,  DECLTYPE_D3DCOLOR		, DECLMETHOD_DEFAULT, DECLUSAGE_COLOR	, 0 },
		D3DDECL_END()
	};
	/*!
	*@brief	カラー＋法線。
	*/
	struct SShapeVertex_PNC {
		float		pos[4];
		float		normal[4];
		int		color;	
	};
	static const SVertexElement scShapeVertex_PNC_Element[] = {
		{ 0, 0 ,  DECLTYPE_FLOAT4		, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION	, 0 },
		{ 0, 16 ,  DECLTYPE_FLOAT4		, DECLMETHOD_DEFAULT, DECLUSAGE_NORMAL	, 0 },
		{ 0, 32 ,  DECLTYPE_D3DCOLOR		, DECLMETHOD_DEFAULT, DECLUSAGE_COLOR	, 0 },
		D3DDECL_END()
	};
}
#endif // _TKSHAPE_H_