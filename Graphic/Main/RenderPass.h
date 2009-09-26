#ifndef _RENDERPASS_H_
#define _RENDERPASS_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class RenderQueue;

	class _DLL_Export RenderPass
	{
	public:
		RenderPass();
		virtual ~RenderPass();

		enum
		{
			RenderPass_PostShadow,
			RenderPass_Light,
			RenderPass_Color,
			RenderPass_Amibent,
		};

		// ‰÷»æ
		void    Apply( BYTE byRenderPass, RenderQueue * pRenderQueue );

	protected:
		
	private:

	};
}

#endif