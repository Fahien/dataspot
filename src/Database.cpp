#include "dataspot/Database.h"

#include <logspot/Logger.h>

#include "dataspot/Exception.h"


using namespace std;
using namespace dataspot;
namespace lst = logspot;


Database::Database( const string& path )
{
	lst::Logger::log.Info( "Opening a database at %s", path.c_str() );

	int open_result{ sqlite3_open_v2( path.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr ) };

	// If the database file does not exist, log and create it
	if ( open_result != SQLITE_OK )
	{
		lst::Logger::log.Info( "No database available, creating a default one at %s", path.c_str() );
		create( path );
	}
}


Database::~Database()
{
	if ( !db )
	{
		return;
	}

	lst::Logger::log.Info( "Closing the database" );

	int result{ sqlite3_close( db ) };

	if ( result != SQLITE_OK )
	{
		auto message = sqlite3_errstr( result );
		lst::Logger::log.Error( "Could not close the database: ", message );
	}
}


/// Opens a SQLite database, or create if it does not exist
void Database::open( const string& path )
{
	if ( db )
	{
		lst::Logger::log.Info( "Database already opened" );
		return;
	}

	lst::Logger::log.Info( "Opening a database at %s", path.c_str() );

	int open_result{ sqlite3_open_v2( path.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr ) };

	// If the database file does not exist, log and create it
	if ( open_result != SQLITE_OK )
	{
		auto message = sqlite3_errstr( open_result );
		lst::Logger::log.Info( "Cannot open database: %s", message );
		lst::Logger::log.Info( "Creating temporary database %s", path.c_str() );
		create( path.c_str() );
	}
}


/// Returns a prepared statement
sqlite3_stmt* Database::prepare( const string& query )
{
	sqlite3_stmt* statement{ nullptr };

	int prepareResult{ sqlite3_prepare_v2( db, query.c_str(), -1, &statement, nullptr ) };

	if ( prepareResult != SQLITE_OK )
	{
		throw Exception{ sqlite3_errmsg( db ), prepareResult };
	}

	return statement;
}


void Database::create( const string& path )
{
	auto create_result = sqlite3_open_v2( path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr );

	// If could not create the database, complain
	if ( create_result != SQLITE_OK )
	{
		throw Exception{ sqlite3_errmsg( db ), create_result };
	}
	else  // Populate with some default values
	{
		create_config_table();
	}
}


void Database::create_config_table()
{
	create_table( "CREATE TABLE main.config(key TEXT PRIMARY KEY ASC, value TEXT);" );
}


/// Creates a table
void Database::create_table( const string& query )
{
	lst::Logger::log.Info( "Creating a config table" );
	sqlite3_stmt* createTableStmt;

	int prepareResult = sqlite3_prepare_v2( db, query.c_str(),
	                                        -1,  // Read the statement up to the first zero terminator
	                                        &createTableStmt, nullptr );

	// If could not prepare the statement, log the error code
	if ( prepareResult != SQLITE_OK )
	{
		throw Exception{ "Could not prepare a statement", prepareResult };
	}
	else  // Step the prepared statement
	{
		int stepResult = sqlite3_step( createTableStmt );

		// If could not create the table, log the error code
		if ( stepResult != SQLITE_DONE )
		{
			throw Exception{ "Could not step a statement", stepResult };
		}

		// Populate config table with some entries
		populate_config_table();
	}

	// Delete the statement
	sqlite3_finalize( createTableStmt );
}


void Database::populate_config_table()
{
	lst::Logger::log.Info( "Populating the config table" );
	sqlite3_stmt* populateTableStmt;

	int prepareResult = sqlite3_prepare_v2( db, "INSERT INTO main.config(key, value) VALUES(?, ?);",
	                                        -1,  // Read the statement up to the first zero terminator
	                                        &populateTableStmt, nullptr );

	// If could not prepare the statement, log the error code
	if ( prepareResult != SQLITE_OK )
	{
		lst::Logger::log.Info( "Could not prepare a statement. [error %d]", prepareResult );
	}
	else  // Populate the table
	{
#define ENTRIES_COUNT 4
		int entries_count = ENTRIES_COUNT;

		const char* keys[ENTRIES_COUNT] = { "window.title", "window.width", "window.height", "log.level" };

		const char* values[ENTRIES_COUNT] = { "Name", "480", "270", "4" };

		for ( int i = 0; i < entries_count; ++i )
		{
			int bind_result = sqlite3_bind_text( populateTableStmt,
			                                     1,  // First parameter: key
			                                     keys[i], -1, SQLITE_TRANSIENT );

			// If could not bind the key, log the error
			if ( bind_result != SQLITE_OK )
			{
				lst::Logger::log.Info( "Could not bind a parameter [error %d]", bind_result );
				continue;
			}

			bind_result = sqlite3_bind_text( populateTableStmt,
			                                 2,  // Second parameter: value
			                                 values[i], -1, SQLITE_TRANSIENT );

			// If could not bind the value, log the error
			if ( bind_result != SQLITE_OK )
			{
				lst::Logger::log.Info( "Could not bind a parameter [error %d]", bind_result );
				continue;
			}

			// Step the insert statement
			int step_result = sqlite3_step( populateTableStmt );

			// If could not step the statement, log the error
			if ( step_result != SQLITE_DONE )
			{
				lst::Logger::log.Info( "Could not step the insert statement: %s", sqlite3_errmsg( db ) );
			}

			// Reset the statement
			int reset_result = sqlite3_reset( populateTableStmt );

			// Check reset result
			if ( reset_result != SQLITE_OK )
			{
				lst::Logger::log.Info( "Could not reset the statement [error %d]", reset_result );
			}
		}
	}

	// Delete the statement
	sqlite3_finalize( populateTableStmt );
}
