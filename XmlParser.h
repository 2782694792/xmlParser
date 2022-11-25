#pragma once

#include "XmL.h"

class Parser
{
private:
	string m_str;
	int m_index;

public:
	Parser( );
	~Parser( );

	bool loadFile( const string & fileName );
	bool loadFile( const ifstream & fileName );
	bool loadStr( const string & str );

	Xml parse( );


private:
	void skip_whitespace( );
	bool parse_declaration( );
	bool parse_comment( );
	Xml parse_element( );

	string parse_element_Text( );
	string parse_element_Name( );
	string parse_element_attrKey( );
	string parse_element_attrValue( );

};
