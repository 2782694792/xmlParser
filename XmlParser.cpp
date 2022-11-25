#include "XmlParser.h"

#include <iostream>
#include <sstream>

Parser::Parser( ) : m_str( "" ), m_index( -1 )
{
}

Parser::~Parser( )
{
}

void Parser::skip_whitespace( )
{
	while ( m_str[ m_index ] == ' ' || m_str[ m_index ] == '\r' || m_str[ m_index ] == '\n' || m_str[ m_index ] == '\t' )
	{
		m_index++;
	}
}

bool Parser::parse_declaration( )
{
	m_index += 5;
	size_t pos = m_str.find( "?>", m_index );
	if ( pos == std::string::npos )
	{
		return false;
	}
	m_index = pos + 2;
	return true;
}

bool Parser::parse_comment( )
{
	m_index += 4;
	size_t pos = m_str.find( "-->", m_index );
	if ( pos == std::string::npos )
	{
		return false;
	}
	m_index = pos + 3;
	return true;
}

string Parser::parse_element_Name( )
{
	int pos = m_index;
	if ( isalpha( m_str[ m_index ] ) || m_str[ m_index ] == '_' )
	{
		while ( m_str[ m_index ] == '-' || m_str[ m_index ] == '_' || m_str[ m_index ] == ':' || m_str[ m_index ] == '.' || isalnum( m_str[ m_index ] ) )
		{
			m_index++;
		}
	}

	return m_str.substr( pos, m_index - pos );
}

string Parser::parse_element_Text( )
{
	int pos = m_index;

	// 文本：直到新节点或节点结束标志 “<” ，若不存在，返回空（存在异常或者无文本），存在返回去空的字符串（可能为空）
	while ( m_str[ m_index ] != '<' )
	{
		m_index++;
	}

	return m_str.substr( pos, m_index - pos );
}

string Parser::parse_element_attrKey( )
{
	int pos = m_index;

	if ( isalpha( m_str[ m_index ] ) || m_str[ m_index ] == '_' )
	{
		m_index++;
		while ( isalnum( m_str[ m_index ] ) || ( m_str[ m_index ] == '_' ) || ( m_str[ m_index ] == '-' ) || ( m_str[ m_index ] == ':' ) )
		{
			m_index++;
		}
	}

	return m_str.substr( pos, m_index - pos );
}

string Parser::parse_element_attrValue( )
{
	if ( m_str[ m_index ] != '\"' )
	{
		throw std::logic_error( "xml attribute value exception, it should be in double quotes!" );
	}

	m_index++;
	int pos = m_index;

	while ( m_str[ m_index ] != '\"' )
	{
		m_index++;
	}
	m_index++;

	return m_str.substr( pos, m_index - pos - 1 );
}

Xml Parser::parse_element( )
{
	m_index++;

	skip_whitespace( );

	const string & name = parse_element_Name( ); // 获取节点名称

	Xml elem; // 子节点
	elem.setName( name );

	// 进入节点属性、子节点、节点结束、仅节点名称(获取节点内容)、注释
	//! 以标志性符号判别，< 或 <!-- 、/、>、节点属性符号（ _ 字母 双引号 等未知情况）
	while ( m_str[ m_index ] != '\0' )
	{
		skip_whitespace( );

		//! 节点结束，如 <br/>
		if ( m_str[ m_index ] == '/' )
		{
			if ( m_str[ m_index ] == '>' )
			{
				m_index += 2;
			}
			else
			{
				throw std::logic_error( "xml child node is abnormal!" );
			}
		}
		//! 仅节点名称，如 <br>
		else if ( m_str[ m_index ] == '>' )
		{
			m_index++;
			string elemTxt = parse_element_Text( );
			if ( elemTxt != "" )
			{
				elem.setText( elemTxt );
			}
		}
		//! 子节点、双节点结束标志、注释
		else if ( m_str[ m_index ] == '<' )
		{
			// 注释，<!-- ssss -->
			if ( m_str.compare( m_index, 4, "<!--" ) == 0 )
			{
				if ( !parse_comment( ) )
				{
					throw std::logic_error( "xml comment is abnormal!" );
				}
			}
			// 双节点结束标志，</br>
			else if ( m_str[ m_index + 1 ] == '/' )
			{
				string endTag = "</" + name + ">";
				size_t pos = m_str.find( endTag, m_index );
				if ( pos == std::string::npos )
				{
					throw std::logic_error( "not found xml element end tag : " + name + " !" );
				}
				m_index = pos + endTag.size( );
				break;
			}
			// 子节点
			else
			{
				elem.append( parse_element( ) );
			}
		}
		//! 节点属性，英文字母开头、下划线开头、含有其他字符符号数字字母
		else
		{
			// 属性名 name = "bin" age = 18
			string attrKey = parse_element_attrKey( );
			skip_whitespace( );

			if ( m_str[ m_index ] != '=' )
			{
				throw std::logic_error( "xml attribute " + attrKey + " name exception" );
			}

			// 属性值
			m_index++;
			skip_whitespace( );
			string attrVal = parse_element_attrValue( );

			elem.setAttr( attrKey, attrVal );
		}
	}

	return elem;
}

Xml Parser::parse( )
{
	if ( m_str == "" )
	{
		throw std::logic_error( "the xml data is empty!" );
	}

	// 声明
	skip_whitespace( );
	if ( m_str.compare( m_index, 5, "<?xml" ) == 0 )
	{
		if ( !parse_declaration( ) )
		{
			throw std::logic_error( "xml declaration exception!" );
		}
	}

	// 注释
	skip_whitespace( );
	while ( m_str.compare( m_index, 4, "<!--" ) == 0 )
	{
		if ( !parse_comment( ) )
		{
			throw std::logic_error( "comment exception!" );
		}
	}

	// 节点
	skip_whitespace( );
	if ( m_str[ m_index ] == '<' && ( isalpha( m_str[ m_index + 1 ] ) || m_str[ m_index + 1 ] == '_' ) )
	{
		return parse_element( );
	}

	// 文本

	throw std::logic_error( "failed to parse xml element!" );
}

bool Parser::loadFile( const string & fileName )
{
	ifstream fin( fileName );
	if ( fin.fail( ) )
	{
		return false;
	}
	if ( loadFile( fin ) )
	{
		return true;
	}
	return false;
}

bool Parser::loadFile( const ifstream & fileName )
{
	stringstream ss;
	ss << fileName.rdbuf( );
	m_str = ss.str( );
	m_index = 0;
	return true;
}

bool Parser::loadStr( const string & str )
{
	m_str = str;
	m_index = 0;
	return true;
}
