#pragma once

namespace XRE {
	class RenderPass {

	public:
		RenderPass() {};
		~RenderPass() {};

		virtual void Forward()=0;
	 
	
	};


}