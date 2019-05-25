#include "dataspot/Database.h"

#include <logspot/Log.h>

#include "dataspot/Exception.h"

namespace lst = logspot;

namespace dataspot
{
std::variant<Database, Error> Database::open( const std::string& path )
{
	lst::Log::info( "Opening a database at %s", path.c_str() );

	sqlite3* sqlite_db   = nullptr;
	auto     open_result = sqlite3_open_v2( path.c_str(), &sqlite_db, SQLITE_OPEN_READWRITE, nullptr );

	// If the database file does not exist, log and create it
	if ( open_result != SQLITE_OK )
	{
		return Error{ open_result, *sqlite_db };
	}

	return Database{ *sqlite_db };
}

Database::Database( sqlite3& sqlite_db ) : db{ &sqlite_db }
{
}

Database::Database( Database&& other ) : db{ other.db }
{
	other.db = nullptr;
}

Database::~Database()
{
	if ( !db )
	{
		return;
	}

	lst::Log::info( "Closing the database" );

	int result{ sqlite3_close( db ) };

	if ( result != SQLITE_OK )
	{
		auto message = sqlite3_errstr( result );
		lst::Log::error( "Could not close the database: ", message );
	}
}

/// Returns a prepared statement
std::variant<Statement, Error> Database::prepare( const std::string& query )
{
	sqlite3_stmt* statement{ nullptr };

	auto prepare_result = sqlite3_prepare_v2( db, query.c_str(), -1, &statement, nullptr );

	if ( prepare_result != SQLITE_OK )
	{
		return Error{ prepare_result, *db };
	}

	return statement;
}


std::variant<Database, Error> Database::create( const std::string& path )
{
	sqlite3* sqlite_db = nullptr;

	auto create_result = sqlite3_open_v2( path.c_str(), &sqlite_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr );

	// If could not create the database, complain
	if ( create_result != SQLITE_OK )
	{
		return Error{ create_result, *sqlite_db };
	}

	Database db{ *sqlite_db };
	// Populate with some default values
	db.create_config_table();
	return db;
}


void Database::create_config_table()
{
	create_table( "CREATE TABLE main.config(key TEXT PRIMARY KEY ASC, value TEXT);" );
}


/// Creates a table
void Database::create_table( const std::string& query )
{
	lst::Log::info( "Creating a config table" );
	sqlite3_stmt* createTableStmt;

	int prepare_result = sqlite3_prepare_v2( db, query.c_str(),
	                                         -1,  // Read the statement up to the first zero terminator
	                                         &createTableStmt, nullptr );

	// If could not prepare the statement, log the error code
	if ( prepare_result != SQLITE_OK )
	{
		throw Exception{ "Could not prepare a statement", prepare_result };
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
	lst::Log::info( "Populating the config table" );
	sqlite3_stmt* populateTableStmt;

	int prepare_result = sqlite3_prepare_v2( db, "INSERT INTO main.config(key, value) VALUES(?, ?);",
	                                         -1,  // Read the statement up to the first zero terminator
	                                         &populateTableStmt, nullptr );

	// If could not prepare the statement, log the error code
	if ( prepare_result != SQLITE_OK )
	{
		lst::Log::info( "Could not prepare a statement. [error %d]", prepare_result );
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
				lst::Log::info( "Could not bind a parameter [error %d]", bind_result );
				continue;
			}

			bind_result = sqlite3_bind_text( populateTableStmt,
			                                 2,  // Second parameter: value
			                                 values[i], -1, SQLITE_TRANSIENT );

			// If could not bind the value, log the error
			if ( bind_result != SQLITE_OK )
			{
				lst::Log::info( "Could not bind a parameter [error %d]", bind_result );
				continue;
			}

			// Step the insert statement
			int step_result = sqlite3_step( populateTableStmt );

			// If could not step the statement, log the error
			if ( step_result != SQLITE_DONE )
			{
				lst::Log::info( "Could not step the insert statement: %s", sqlite3_errmsg( db ) );
			}

			// Reset the statement
			int reset_result = sqlite3_reset( populateTableStmt );

			// Check reset result
			if ( reset_result != SQLITE_OK )
			{
				lst::Log::info( "Could not reset the statement [error %d]", reset_result );
			}
		}
	}

	// Delete the statement
	sqlite3_finalize( populateTableStmt );
}

}  // namespace dataspot
