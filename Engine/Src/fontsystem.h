#pragma once

namespace mania
{
//abstract classes
class fontface : public object
{
public:
	fontface();
	virtual ~fontface() {};
};

class fontsystem
{
public:
	virtual gresult initialize() = 0;
	virtual gresult uninitialize() = 0;
	virtual gresult create_face( sptr<fontface>& face, const string&  url ) = 0;
};

//freetype implementation

class fontsystem_freetype : public fontsystem
{
public:
	fontsystem_freetype( void );
	~fontsystem_freetype( void );

	virtual gresult initialize();
	virtual gresult uninitialize();
	virtual gresult create_face( sptr<fontface>& face, const string&  url );
};

//////////////////
//quad management
///////////////
struct quad_identity
{
	string	face;
	int		size;
	int operator==( const quad_identity& id ) const
	{
		return (( face == id.face ) && ( size == id.size ) );
	}
};

struct quad
{
	int x;
	int y;
	int w;
	int h;
	int priority;
	class quad_store* store;
	TCHAR data;
};

class quad_store
{
public:
	math::point m_size;
	std::vector<quad>	quads;
	unsigned int	m_max_count;
	const quad_identity& get_identity()
	{
		return m_id;
	}
	void set_identity( const quad_identity& id );
private:
	quad_identity		m_id;
};

struct quad_batch
{
	int	store;
	std::vector<quad*> quads;
};

struct quad_batch_group
{
	std::vector<quad_batch> batchs;
};

class quad_manager
{
public:
	void			init( math::point store_size );
	void			addquad( const quad_identity& id, TCHAR data );
	void			removequad( const quad_identity& id, TCHAR data );
	void			get_process_batch( const string&, quad_batch_group& qbg );

private:

	gresult			store_quad( const quad_identity& id, quad& in_quad, quad& out_quad );
	BOOL			store_quad( const quad_identity& id, quad_store& store, quad& in_quad );
	quad_store&		add_store( const quad_identity& id );

private:
	math::point		m_store_size;
	typedef std::vector<quad_store> storetype ;
	storetype m_stores;
};
}