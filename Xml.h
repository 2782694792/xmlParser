#pragma once

#include <list>
#include <map>
#include <string>
#include <fstream>

using namespace std;

//enum ATTR_DATATYPE
//{
//	ATTR_INT = 1,
//	ATTR_DOUBLE,
//	ATTR_BOOL,
//	ATTR_STRING
//};

// 属性类
class Attribute
{
public:
	Attribute( );
	Attribute( bool attr );
	Attribute( int attr );
	Attribute( double attr );
	Attribute( const char * attr );
	Attribute( const string & attr );
	~Attribute( );

	Attribute & operator=( bool attr );
	Attribute & operator=( int attr );
	Attribute & operator=( double attr );
	Attribute & operator=( const char * attr );
	Attribute & operator=( const string & attr );
	Attribute & operator=( const Attribute & attr );

	bool operator==( const Attribute & another );
	bool operator!=( const Attribute & another );

	operator bool( );
	operator int( );
	operator double( );
	operator string( );

private:
	string m_attr;
};

// 树形结构
class Xml
{
private:
	// 节点
	list<Xml> * m_child;

	// 节点名称
	string * m_name;

	// 节点属性
	map<string, Attribute> * m_attrs;

	// 节点内容
	string * m_text;

public:
	Xml( );
	Xml( const char * name );
	Xml( const string & name );
	Xml( const Xml & another );
	~Xml( );

	string getName( ) const;
	void setName( const string & name );

	string getText( ) const;
	void setText( const string & text );

	Attribute & getAttr( const string & key );
	void setAttr( const string & key, const Attribute & value );

	// 子节点数量
	int size( ) const;

	// 序列化
	string serial( ) const;

	// 插入子节点
	void append( const Xml & child );

	// 添加指定节点
	Xml & operator[]( int index ); // 不存在对应索引，赋值添加子节点
	Xml & operator[]( const char * name );
	Xml & operator[]( const string & name ); // 不存在对应节点名称，添加子节点

	Xml & operator=( const Xml & another );


	// 删除指定节点
	void remove( int index );
	void remove( const char * name );
	void remove( const string & name );

	typedef std::list<Xml>::iterator iterator;
	iterator begin( );
	iterator end( );
	iterator erase( iterator it );

	bool load( const string & parseText );
	bool load( const ifstream & fileName );
	bool save( const string & file );
	bool parse( const string & str );

private:

	void clear( );

};

