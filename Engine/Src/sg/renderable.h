#pragma once

namespace mania
{
	class MANIA_ENGINE_API renderable : public tnode
	{
		MANIA_DECLAR_ABSTRACTL_CLASS( renderable )
	public:
		renderable( void );
		~renderable( void );

		//render interface
		virtual gresult render( render_context& render_context ) = 0;
	};

}