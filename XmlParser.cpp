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

	// �ı���ֱ���½ڵ��ڵ������־ ��<�� ���������ڣ����ؿգ������쳣�������ı��������ڷ���ȥ�յ��ַ���������Ϊ�գ�
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

	const string & name = parse_element_Name( ); // ��ȡ�ڵ�����

	Xml elem; // �ӽڵ�
	elem.setName( name );

	// ����ڵ����ԡ��ӽڵ㡢�ڵ���������ڵ�����(��ȡ�ڵ�����)��ע��
	//! �Ա�־�Է����б�< �� <!-- ��/��>���ڵ����Է��ţ� _ ��ĸ ˫���� ��δ֪�����
	while ( m_str[ m_index ] != '\0' )
	{
		skip_whitespace( );

		//! �ڵ�������� <br/>
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
		//! ���ڵ����ƣ��� <br>
		else if ( m_str[ m_index ] == '>' )
		{
			m_index++;
			string elemTxt = parse_element_Text( );
			if ( elemTxt != "" )
			{
				elem.setText( elemTxt );
			}
		}
		//! �ӽڵ㡢˫�ڵ������־��ע��
		else if ( m_str[ m_index ] == '<' )
		{
			// ע�ͣ�<!-- ssss -->
			if ( m_str.compare( m_index, 4, "<!--" ) == 0 )
			{
				if ( !parse_comment( ) )
				{
					throw std::logic_error( "xml comment is abnormal!" );
				}
			}
			// ˫�ڵ������־��</br>
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
			// �ӽڵ�
			else
			{
				elem.append( parse_element( ) );
			}
		}
		//! �ڵ����ԣ�Ӣ����ĸ��ͷ���»��߿�ͷ�����������ַ�����������ĸ
		else
		{
			// ������ name = "bin" age = 18
			string attrKey = parse_element_attrKey( );
			skip_whitespace( );

			if ( m_str[ m_index ] != '=' )
			{
				throw std::logic_error( "xml attribute " + attrKey + " name exception" );
			}

			// ����ֵ
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

	// ����
	skip_whitespace( );
	if ( m_str.compare( m_index, 5, "<?xml" ) == 0 )
	{
		if ( !parse_declaration( ) )
		{
			throw std::logic_error( "xml declaration exception!" );
		}
	}

	// ע��
	skip_whitespace( );
	while ( m_str.compare( m_index, 4, "<!--" ) == 0 )
	{
		if ( !parse_comment( ) )
		{
			throw std::logic_error( "comment exception!" );
		}
	}

	// �ڵ�
	skip_whitespace( );
	if ( m_str[ m_index ] == '<' && ( isalpha( m_str[ m_index + 1 ] ) || m_str[ m_index + 1 ] == '_' ) )
	{
		return parse_element( );
	}

	// �ı�

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
