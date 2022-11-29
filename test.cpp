#include "XmlParser.h"

#include <iostream>
#include <string>

#include <log4cplus/initializer.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

auto fun = [ ]( ) {

	Xml x;
	x.setName( "XML" );
	x.setAttr( "ID", 1 );
	x.setAttr( "pos", "first" );
	x.setText( "this is a first xml element" );

	Xml x1;
	x1.setName( "XML1" );
	x1.setAttr( "ID", 1.1 );
	x1.setAttr( "pos", "1" );
	x1.setText( "this is a second xml element" );

	Xml x2;
	x2.setName( "XML2" );
	x2.setAttr( "ID", 1.2 );
	x2.setAttr( "pos", "2" );
	x2.setText( "this is a third xml element" );

	Xml x3;
	x3.setName( "XML3" );
	x3.setAttr( "ID", 1.3 );
	x3.setAttr( "pos", "3" );
	x3.setText( "this is a fourth xml element" );

	x1.append( x2 );
	x1.append( x3 );
	x.append( x1 );

	string opt = x.serial( );
	return opt;
};

#define LOGPROPERTY "logconfig.property"

int main( )
{
	//! 1����ʼ�� initialize
	log4cplus::initialize( );
	//! 2���������ʵ���� appender
	log4cplus::SharedAppenderPtr  _consoleAppender( new log4cplus::ConsoleAppender );
	log4cplus::SharedAppenderPtr _fileAppender( new log4cplus::FileAppender(
		LOG4CPLUS_TEXT( "test.log" ), std::ios_base::app & std::ios_base::ate ) );
	//! 3�������ʽʵ���� layout
	log4cplus::SimpleLayout * _consoleLayout = new log4cplus::SimpleLayout( );
	log4cplus::tstring pattern = LOG4CPLUS_TEXT( "[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p - %m%n[%p]" );
	log4cplus::PatternLayout * _patternLayout = new log4cplus::PatternLayout( pattern );
	//! 4��layout ������ʶ���󶨵� appender �����ʽ����
	//!? ʡ�� layout ������ʶ���󶨵� appender �����ʽ���󣬼򵥲�������󶨵� logger ��־�������
	//_consoleAppender->setLayout( std::unique_ptr<log4cplus::Layout>( _consoleLayout ) );
	//_fileAppender->setLayout( std::unique_ptr<log4cplus::Layout>( _patternLayout ) );
	_consoleAppender->setLayout( std::unique_ptr<log4cplus::Layout>( _patternLayout ) );
	//! 5����־���ʵ���� logger
	log4cplus::Logger logger = log4cplus::Logger::getInstance( LOG4CPLUS_TEXT( "test" ) );
	//! 6����־�ȼ���������ʰ�
	logger.setLogLevel( log4cplus::DEBUG_LOG_LEVEL );
	logger.addAppender( _consoleAppender );
	logger.addAppender( _fileAppender );

	LOG4CPLUS_DEBUG( logger, "test : " << fun( ).c_str( ) );

	//cout << opt << endl;

	// �������нڵ�
	//cout << opt << endl;

	// ָ���ڵ�����ӽڵ�
	//int x1size = x1.size( );
	//for ( int i = 0; i < x1size; i++ )
	//{
	//	cout << x1[ i ].serial( ) << endl;
	//}

	// x �ڵ��µ��ӽڵ����
	//for ( auto it = x.begin(); it != x.end(); it++ )
	//{
	//	cout << it->serial( ) << endl;
	//}

	// ɾ��ָ���ڵ��µĵ� n-1 ���ӽڵ�
	//x1.remove( 1 );

	// ɾ��ָ���ڵ��µĶ�Ӧ�ڵ�
	//x1.erase( x1.begin() );

	// ��ȡ������� xml
	//Xml pom;

	/*if ( !pom.load( "pomTest.xml" ) )
	{
		cout << "not found the file" << endl;
	}*/

	//ifstream fin( "pomTest.xml" );
	//if ( !pom.load( fin ) )
	//{
	//	cout << "not found the file" << endl;
	//}

	//const string pomStr = pom.serial( );

	//const string pomStr = pom["description" ].serial( );
	//cout << "project : description = \n\t\t" << pomStr << endl;

	//pom.save("pomTest111.xml" );

	//Parser p;
	//p.loadFile( "pomTest.xml" );

}
