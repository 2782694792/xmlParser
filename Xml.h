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

// ������
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

// ���νṹ
class Xml
{
private:
	// �ڵ�
	list<Xml> * m_child;

	// �ڵ�����
	string * m_name;

	// �ڵ�����
	map<string, Attribute> * m_attrs;

	// �ڵ�����
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

	// �ӽڵ�����
	int size( ) const;

	// ���л�
	string serial( ) const;

	// �����ӽڵ�
	void append( const Xml & child );

	// ���ָ���ڵ�
	Xml & operator[]( int index ); // �����ڶ�Ӧ��������ֵ����ӽڵ�
	Xml & operator[]( const char * name );
	Xml & operator[]( const string & name ); // �����ڶ�Ӧ�ڵ����ƣ�����ӽڵ�

	Xml & operator=( const Xml & another );


	// ɾ��ָ���ڵ�
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

