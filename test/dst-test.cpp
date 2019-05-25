#include <logspot/Log.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <dataspot/Database.h>
#include <dataspot/Exception.h>


int main()
{
	using namespace dataspot;
	using namespace logspot;

	Database* data{ nullptr };

	// Open or create (if does not exist) database
	auto result = Database::open( "test.data" );
	if ( auto error = std::get_if<Error>( &result ) )
	{
		auto result = Database::create( "test.data" );
		if ( auto error = std::get_if<Error>( &result ) )
		{
			Log::error( "Cannot open database %s", error->get_message().c_str() );
			return EXIT_FAILURE;
		}

		data = std::get_if<Database>( &result );
	}
	else
	{
		data = std::get_if<Database>( &result );
	}

	// Get database

	{
		auto prepare_result = data->prepare( "SELECT value FROM main.config WHERE key = ?;" );
		if ( auto error = std::get_if<Error>( &result ) )
		{
			Log::error( "Cannote prepare statement %s", error->get_message().c_str() );
		}

		// Get statement
		auto& statement = *std::get_if<Statement>( &prepare_result );

		statement.bind( "window.title" );
		statement.step();
		assert( statement.get_text() == "Name" );
		statement.reset();

		statement.bind( "window.width" );
		statement.step();
		assert( statement.get_text() == "480" );
		statement.reset();

		statement.bind( "window.height" );
		statement.step();
		assert( statement.get_text() == "270" );
		statement.reset();

		statement.bind( "log.level" );
		statement.step();
		assert( statement.get_text() == "4" );
		statement.reset();
	}

	{
		auto prepare_result = data->prepare( "SELECT * from main.config;" );
		if ( auto error = std::get_if<Error>( &result ) )
		{
			Log::error( "Cannote prepare statement %s", error->get_message().c_str() );
		}

		auto& stmt = *std::get_if<Statement>( &prepare_result );

		for ( auto row : stmt )
		{
			auto key   = row.get<std::string>( 0 );
			auto value = row.get<std::string>( 1 );
			std::cout << "(" << key << ", " << value << ")" << std::endl;
		}

		for ( auto row : stmt )
		{
			auto key   = row.get<std::string>( 0 );
			auto value = row.get<std::string>( 1 );
			std::cout << "(" << key << ", " << value << ")" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
