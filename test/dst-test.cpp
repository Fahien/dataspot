#include <logspot/Logger.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <dataspot/Dataspot.h>
#include <dataspot/Exception.h>

using namespace std;
namespace lst = logspot;
namespace dst = dataspot;


int main()
{
	try
	{
		dst::Dataspot data{ "test.data" };
		assert( data.get_config_value( "window.title" ) == "Name" );
		assert( data.get_config_value( "window.width" ) == "480" );
		assert( data.get_config_value( "window.height" ) == "270" );
		assert( data.get_config_value( "log.level" ) == "4" );

		auto& stmt = data.prepare( "SELECT * from main.config;" );
		for ( auto row : stmt )
		{
			auto key   = row.get<string>( 0 );
			auto value = row.get<string>( 1 );
			cout << "(" << key << ", " << value << ")" << endl;
		}

		for ( auto row : stmt )
		{
			auto key   = row.get<string>( 0 );
			auto value = row.get<string>( 1 );
			cout << "(" << key << ", " << value << ")" << endl;
		}
	}
	catch ( const dst::Exception& e )
	{
		lst::Logger::log.Error( e.to_string() );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
