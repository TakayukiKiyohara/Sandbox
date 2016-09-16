/*!
 *@brief	レンダリングコマンドバッファクラス。
 */

#ifndef _TKRENDERCOMMANDBUFFER_H_
#define _TKRENDERCOMMANDBUFFER_H_

namespace tkEngine{
	/*!
	 *@brief	描画コマンドバッファ
	 */
	class CRenderCommandBuffer{
	public:
		/*!
		 *@brief	コンストラクタ
		 */
		CRenderCommandBuffer();
		/*!
		 *@brief	デストラクタ。
		 */
		~CRenderCommandBuffer();
		/*!
		 *@brief	初期化。
		 *@param[in]	size	コマンドバッファのサイズ。
		 */
		void Init(int size);
		/*!
		 *@brief	64バイトのコマンドを書き込み。
		 *@details	コマンドは必ず64byte
		 *@param[in]	command		コマンド。
		 */
		template<class T>
		void WriteCommand64(T& command)
		{
			static_assert( sizeof(T) <= 64, "command size error\n");
			TK_ASSERT( m_writePos + 64 <= m_size, "command buffer size over!!!");
			struct Write64Byte{
				int data[16];
			};
			Write64Byte* dst = reinterpret_cast<Write64Byte*>(&m_buffer[m_writePos]);
			Write64Byte* src = reinterpret_cast<Write64Byte*>(&command);
			*dst = *src;
			m_command.push_back(dst);
			m_writePos += 64;
		}
		/*!
		*@brief	コマンドバッファからアロケート
		*/
		void* Alloc(int sizeInByte)
		{
			TK_ASSERT(m_writePos + sizeInByte <= m_size, "command buffer size over!!!");
			void* p = &m_buffer[m_writePos];
			m_writePos += sizeInByte;
			return p;
		}
		/*!
		 *@brief	コマンドバッファをサブミット。
		 */
		void Submit(LPDIRECT3DDEVICE9 pD3DDevice);
	private:
		std::unique_ptr<unsigned char[]>	m_buffer;	//!<コマンドバッファ
		std::vector<void*>		m_command;	//!<コマンド。
		int						m_writePos;	//!<書き込み位置
		int						m_size;		//!<コマンドバッファのサイズ。
	};
}
#endif // _TKRENDERCOMMANDBUFFER_H_