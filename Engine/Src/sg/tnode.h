#pragma once

namespace mania
{
class MANIA_ENGINE_API tnode : public node
{
	MANIA_DECLAR_CLASS( tnode )
public:
	tnode( void );
	virtual ~tnode( void );

	//3d transform interface
	transform& get_local_transform() { return m_local_transform; }
	void set_local_transform( const transform& atransform ) { m_local_transform = atransform; }
	const transform& get_world_transform();
	void update_world_transform();
protected:
	transform	m_local_transform;
	transform	m_world_transform;
};
}