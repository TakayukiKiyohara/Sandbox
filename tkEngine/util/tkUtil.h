/*!
 * @brief	���[�e�B���e�B�֐��B
 */
#ifndef _TKUTIL_H_
#define _TKUTIL_H_

namespace tkEngine{
	class CUtil{
	public:
		/*!
		 * @brief	�����񂩂�32bit�̃n�b�V���l���쐬�B
		 */
		static int MakeHash( const char* string )
		{
			int hash = 0;
			int l = (int)strlen(string);
			for( int i = 0; i < l; i++ ){
				hash = hash * 37 + string[i];
			}
			return hash;
		}
	};
}

#endif // _TKUTIL_H_