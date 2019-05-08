#include <sqlite3.h>

#include "dataspot/Exception.h"

#include "dataspot/Statement.h"

using namespace std;
using namespace dataspot;


Statement::Statement( sqlite3_stmt* s ) : stmt{ s }, row{ *this }
{
}


Statement::Statement( Statement&& statement ) : row{ move( statement.row ) }
{
	*this = move( statement );
}


Statement::~Statement()
{
	// Delete the statement
	if ( stmt )
	{
		sqlite3_finalize( stmt );
	}
}


Statement& Statement::operator=( Statement&& other )
{
	set_stmt( other.stmt );
	other.stmt = nullptr;
	return *this;
}


void Statement::set_stmt( sqlite3_stmt* stmt )
{
	if ( stmt )
	{
		// Delete previous statement
		sqlite3_finalize( stmt );
	}

	stmt = stmt;
}


// Checks a bind result
void Statement::CheckBind( const int result ) const
{
	// Check the result
	if ( result != SQLITE_OK )
	{
		throw Exception{ "Could not bind", result };
	}
}


// Binds an integer
void Statement::bind( const int value, const int index ) const
{
	CheckBind( sqlite3_bind_int( stmt, index, value ) );
}


// Binds a string
void Statement::bind( const string& value, const int index ) const
{
	CheckBind( sqlite3_bind_text( stmt, index, value.c_str(), -1, SQLITE_TRANSIENT ) );
}


Statement::Iterator Statement::begin() const
{
	step();
	return Statement::Iterator{ this };
}


Statement::Iterator Statement::end() const
{
	return Iterator::END;
}


int Statement::step() const
{
	return sqlite3_step( stmt );
}


int Statement::get_integer( const unsigned column ) const
{
	return sqlite3_column_int( stmt, column );
}


double Statement::get_double( const unsigned column ) const
{
	return sqlite3_column_double( stmt, column );
}


string Statement::get_text( const unsigned column ) const
{
	const unsigned char* cValue{ sqlite3_column_text( stmt, column ) };
	return reinterpret_cast<const char*>( cValue );
}


// Resets the statement for reuse
void Statement::reset() const
{
	sqlite3_reset( stmt );
}


Row::Row( Statement& stmt ) : stmt{ stmt }
{
}


void Row::next()
{
	int stepResult = stmt.step();

	// Check the result
	if ( stepResult != SQLITE_ROW )
	{
		throw Exception{ "Could not step", stepResult };
	}
}


template <>
std::string Row::get<std::string>( const uint64_t column )
{
	assert( column >= 0 );
	const unsigned char* cValue{ sqlite3_column_text( stmt.get_stmt(), static_cast<int>( column ) ) };
	return reinterpret_cast<const char*>( cValue );
}


Statement::Iterator Statement::Iterator::END{ nullptr };


Statement::Iterator::Iterator( const Statement* stmt ) : stmt{ stmt }
{
}


bool Statement::Iterator::operator!=( const Iterator& other ) const
{
	return stmt != other.stmt;
}


Statement::Iterator Statement::Iterator::operator++()
{
	auto stepResult = stmt->step();

	// Check the result
	if ( stepResult != SQLITE_ROW )
	{
		stmt = nullptr;
	}

	return *this;
}
