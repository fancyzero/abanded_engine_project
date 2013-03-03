#pragma once

namespace pug
{
struct t_xml_node_struct;
class xml_node;
class xml_parser;
}
namespace mania
{

class MANIA_BASIS_API xmlfile : public filesystem::file
{
public:
	class MANIA_BASIS_API node
	{
	public:

		node();
		node( struct pug::t_xml_node_struct* node ): m_node_struct( node ) {}

		//iterator
		class MANIA_BASIS_API child_iteartor
		{
		public:
			child_iteartor( node& a_node, const string& a_name );
			void next();
			node operator *();
		private:
			string	name;
			node*	nd;
			unsigned int current;
		};

		node& operator =( struct pug::t_xml_node_struct* node )
		{
			m_node_struct = node;
			return *this;
		}
		bool operator == ( struct pug::t_xml_node_struct* node )
		{
			return m_node_struct == node;
		}
		bool operator != ( struct pug::t_xml_node_struct* node )
		{
			return m_node_struct != node;
		}
		bool			valid() const { return m_node_struct != NULL; }
		unsigned int	get_child_count() const;
		node			get_child( unsigned int idx ) const;
		const TCHAR*	get_attribute( const string& attrname ) const;
		const TCHAR*	get_value( ) const ;
		const TCHAR*	get_name( ) const;

		node			find_child( const string& name ) const ;
		node			find_child( const string& name, const string& attr_name, const string& attr_value ) const ;

	private:
		struct pug::t_xml_node_struct* m_node_struct;
	};
public:
	xmlfile( void );
	~xmlfile( void );
	DWORD	open( const string& filename );
	node	get_root();
private:
	class pug::xml_parser*	m_parser;
};
}