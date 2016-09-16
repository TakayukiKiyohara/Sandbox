#pragma once

namespace tkEngine {
	class CPIXPerfTag {
		CRenderContext& renderContext;
	public:
		CPIXPerfTag(CRenderContext& rc, LPCWSTR name) :
			renderContext(rc)
		{
#ifdef _DEBUG
			renderContext.SetRenderCommand(CRenderCommand_PerfBeginEvent(name));
#endif
		}
		~CPIXPerfTag()
		{
#ifdef _DEBUG
			renderContext.SetRenderCommand(CRenderCommand_PerfEndEvent());
#endif
		}
	};
}
