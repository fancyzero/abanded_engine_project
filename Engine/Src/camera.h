#pragma once

namespace mania
{
	class MANIA_ENGINE_API camera : public tnode
	{
       
	public:		
		camera();
		virtual ~camera();
		float get_fov(){ return fov; }
		void set_fov( float _fov ){ fov = _fov; }
		float get_aspect(){ return aspect; }
		void set_aspect( float _aspect ){ aspect = _aspect; }
		void update( float lag );
	protected:
		float fov;
		float aspect;
		MANIA_DECLAR_CLASS( camera );
	};
}