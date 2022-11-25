#include "XmL.h"
#include "XmlParser.h"

#include <sstream>
#include <iostream>


Attribute::Attribute( )
{

}

Attribute::Attribute( bool attr )
{
	*this = attr;
}

Attribute::Attribute( int attr )
{
	*this = attr;
}

Attribute::Attribute( double attr )
{
	*this = attr;
}

Attribute::Attribute( const char * attr ) : m_attr( attr )
{
}

Attribute::Attribute( const string & attr ) : m_attr( attr )
{
}

Attribute::~Attribute( )
{
}

Attribute & Attribute::operator=( bool attr )
{
	m_attr = attr ? "true" : "false";
	return *this;
}

Attribute & Attribute::operator=( int attr )
{
	stringstream ss;
	ss << attr;
	m_attr = ss.str( );
	return *this;
}

Attribute & Attribute::operator=( double attr )
{
	stringstream ss;
	ss << attr;
	m_attr = ss.str( );
	return *this;
}

Attribute & Attribute::operator=( const char * attr )
{
	m_attr = attr;
	return *this;
}

Attribute & Attribute::operator=( const string & attr )
{
	m_attr = attr;
	return *this;
}

Attribute & Attribute::operator=( const Attribute & attr )
{
	m_attr = attr.m_attr;
	return *this;
}

bool Attribute::operator==( const Attribute & another )
{
	return m_attr == another.m_attr;
}

bool Attribute::operator!=( const Attribute & another )
{
	return m_attr != another.m_attr;
}

Attribute::operator bool( )
{
	if ( m_attr == "true" )
		return true;
	else if ( m_attr == "false" )
		return false;
	return false;
}

Attribute::operator int( )
{

	return std::atoi( m_attr.c_str( ) );
}

Attribute::operator double( )
{
	return std::atof( m_attr.c_str( ) );
}

Attribute::operator string( )
{
	return m_attr;
}

//! ---------------------------------------------------------------

Xml::Xml( ) : m_attrs( nullptr ), m_child( nullptr ), m_name( nullptr ), m_text( nullptr )
{
}

Xml::Xml( const char * name ) : m_attrs( nullptr ), m_child( nullptr ), m_text( nullptr )
{
	m_name = new string( name );
}

Xml::Xml( const string & name ) : m_attrs( nullptr ), m_child( nullptr ), m_text( nullptr )
{
	m_name = new string( name );
}

Xml::Xml( const Xml & another )
{
	m_name = another.m_name;
	m_text = another.m_text;
	m_attrs = another.m_attrs;
	m_child = another.m_child;
}

Xml::~Xml( )
{
	//clear( );
}

void Xml::setName( const string & name )
{
	if ( m_name != nullptr )
	{
		delete m_name;
		m_name = nullptr;
	}
	m_name = new string( name );
}

string Xml::getName( ) const
{
	if ( m_name == nullptr )
	{
		return "";
	}
	return *m_name;
}

void Xml::setText( const string & text )
{
	if ( m_text != nullptr )
	{
		delete m_text;
		m_text = nullptr;
	}
	m_text = new string( text );
}

string Xml::getText( ) const
{
	if ( m_text == nullptr )
	{
		return "";
	}
	return *m_text;
}

void Xml::setAttr( const string & key, const Attribute & value )
{
	if ( m_attrs == nullptr )
	{
		//delete m_attrs;
		m_attrs = new std::map<string, Attribute>( );
	}
	( *m_attrs )[ key ] = value;
}

Attribute & Xml::getAttr( const string & key )
{
	if ( m_attrs == nullptr )
	{
		m_attrs = new std::map<string, Attribute>( );
	}
	return ( *m_attrs )[ key ];
}

int Xml::size( ) const
{
	if ( m_child == nullptr )
	{
		return 0;
	}
	return m_child->size( );
}

string Xml::serial( ) const
{
	if ( m_name == nullptr )
	{
		return "";
	}

	stringstream ss;

	// 开始节点
	ss << "<" << *m_name;
	if ( m_attrs != nullptr )
	{
		for ( auto it = m_attrs->begin( ); it != m_attrs->end( ); it++ )
		{
			/*if ( typeid(it->second) == typeid(string) )
			{
				it->second = "\"" + string(it->second) + "\"";
			}*/
			ss << " " << string( it->first ) << "=\"" << string( it->second ) << "\"";
		}
	}
	ss << ">";

	// 子节点或中间内容
	if ( m_child != nullptr )
	{
		for ( auto itchild = m_child->begin( ); itchild != m_child->end( ); itchild++ )
		{
			ss << itchild->serial( );
		}
	}
	if ( m_text != nullptr )
	{
		ss << *m_text;
	}

	// 结束节点
	ss << "</";
	ss << *m_name << ">";

	return ss.str( );
}

void Xml::append( const Xml & child )
{
	if ( m_child == nullptr )
	{
		m_child = new std::list<Xml>( );
	}
	m_child->push_back( child );
}

Xml & Xml::operator[]( int index )
{
	if ( index < 0 )
	{
		throw std::logic_error( "current index is invalid!" );
	}
	if ( m_child == nullptr )
	{
		throw std::logic_error( "child is not exists!" );
	}
	int len = m_child->size( );
	if ( index >= len )
	{
		throw std::logic_error( "out of range!" );
	}
	// 列表属于非连续的存储单元，不可指定遍历
	auto it = m_child->begin( );
	for ( int i = 0; i < index; i++ )
	{
		it++;
	}

	return *it;
}

Xml & Xml::operator[]( const char * name )
{
	return ( *this )[ string( name ) ];
}

Xml & Xml::operator[]( const string & name )
{
	if ( m_child == nullptr )
	{
		m_child = new list<Xml>( );
	}
	for ( auto it = m_child->begin( ); it != m_child->end( ); it++ )
	{
		if ( it->getName( ) == name )
		{
			return *it;
		}
	}
	m_child->push_back( Xml( name ) );
	return m_child->back( );
}

Xml & Xml::operator=( const Xml & another )
{
	clear( );
	m_text = another.m_text;
	m_attrs = another.m_attrs;
	m_name = another.m_name;
	m_child = another.m_child;
	return *this;
}

void Xml::clear( )
{
	if ( m_text != nullptr )
	{
		delete m_text;
		m_text = nullptr;
	}
	if ( m_attrs != nullptr )
	{
		delete m_attrs;
		m_attrs = nullptr;
	}
	if ( m_name != nullptr )
	{
		delete m_name;
		m_name = nullptr;
	}
	if ( m_child != nullptr )
	{
		for ( auto it = m_child->begin( ); it != m_child->end( ); it++ )
		{
			it->clear( );
		}
		delete m_child;
		m_child = nullptr;
	}
}

void Xml::remove( int index )
{
	if ( index < 0 || m_child == nullptr )
	{
		return;
	}
	int len = m_child->size( );
	if ( index >= len )
	{
		return;
	}
	auto it = m_child->begin( );
	for ( int i = 0; i < index; i++ )
	{
		it++;
	}
	it->clear( );
	m_child->erase( it );
}

void Xml::remove( const char * name )
{
	remove( string( name ) );
}

void Xml::remove( const string & name )
{
	for ( auto it = m_child->begin( ); it != m_child->end( );)
	{
		if ( it->getName( ) == name )
		{
			it->clear( );
			it = m_child->erase( it );
		}
		else
		{
			it++;
		}
	}
}

Xml::iterator Xml::begin( )
{
    return m_child->begin( );
}

Xml::iterator Xml::end( )
{
    return m_child->end( );
}

Xml::iterator Xml::erase( iterator it )
{
    it->clear( );
    return m_child->erase( it );
}

bool Xml::load( const string & fileName )
{
	ifstream ifile( fileName );
	if ( load( ifile ) )
	{
		return true;
	}

	return false;
}

bool Xml::load( const ifstream & fileName )
{
	Parser p;
	if ( !p.loadFile( fileName ) )
	{
		return false;
	}

	*this = p.parse( );
	return true;
}

bool Xml::save( const string & file )
{
	ofstream fout( file );
	if ( fout.fail( ) )
	{
		return false;
	}
	fout << serial( );

	fout.close( );
	return true;
}

bool Xml::parse( const string & str )
{
	Parser p;
	if ( !p.loadStr(str) )
	{
		return false;
	}
	*this = p.parse( );
	return true;
}
