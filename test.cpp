#include "XmlParser.h"

#include <iostream>


int main( )
{
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

	// 遍历所有节点
	//cout << opt << endl;

	// 指定节点遍历子节点
	//int x1size = x1.size( );
	//for ( int i = 0; i < x1size; i++ )
	//{
	//	cout << x1[ i ].serial( ) << endl;
	//}

	// x 节点下的子节点遍历
	//for ( auto it = x.begin(); it != x.end(); it++ )
	//{
	//	cout << it->serial( ) << endl;
	//}

	// 删除指定节点下的第 n-1 个子节点
	//x1.remove( 1 );

	// 删除指定节点下的对应节点
	//x1.erase( x1.begin() );

	//string opt = x.serial( );
	//cout << opt << endl;

	// 获取解析后的 xml
	Xml pom;
	/*if ( !pom.load( "pomTest.xml" ) )
	{
		cout << "not found the file" << endl;
	}*/

	ifstream fin( "pomTest.xml" );
	if ( !pom.load( fin ) )
	{
		cout << "not found the file" << endl;
	}

	//const string pomStr = pom.serial( );

	const string pomStr = pom["description" ].serial( );
	cout << "project : description = \n\t\t" << pomStr << endl;

	//pom.save("pomTest111.xml" );

	//Parser p;
	//p.loadFile( "pomTest.xml" );

}
