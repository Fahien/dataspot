#include <sqlite3.h>

#include "dataspot/Statement.h"

#include "dataspot/Dataspot.h"

#define ENTRIES_COUNT 4

using namespace std;
using namespace dataspot;


Dataspot::Dataspot( const string& path ) : db{}, statements{}
{
	db.open( path );
}


Dataspot::~Dataspot()
{
}


/// Returns a prepared statement
Statement& Dataspot::prepare( const string& query )
{
	Statement* stmt = nullptr;

	auto pair = statements.find( query );
	if ( pair == statements.end() )
	{
		// Prepare the statement once
		stmt = &( statements[query] = db.prepare( query ) );
	}
	else
	{
		stmt = &pair->second;
		stmt->reset();
	}

	return *stmt;
}


string Dataspot::get_config_value( const string& key )
{
	string query{ "SELECT value FROM main.config WHERE key = ?;" };

	Statement& getConfigStmt = prepare( query );

	getConfigStmt.bind( key );
	getConfigStmt.step();
	auto value = getConfigStmt.get_text( 0 );
	getConfigStmt.reset();

	return value;
}


/// Creates a table
void Dataspot::create_table( const string& query )
{
	db.create_table( query );
}
