/*!
 * @brief	çsóÒÅB
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/math/tkMatrix.h"

namespace tkEngine{
	const CMatrix CMatrix::Identity = {
		 1.0f, 0.0f, 0.0f, 0.0f ,
		 0.0f, 1.0f, 0.0f, 0.0f ,
		 0.0f, 0.0f, 1.0f, 0.0f ,
		 0.0f, 0.0f, 0.0f, 1.0f 
	};
}