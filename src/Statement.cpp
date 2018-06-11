#include "Statement.h"

#include "DataSpotException.h"
#include "sqlite3.h"

#include <iostream>

using namespace std;
using namespace dataspot;


Statement::Statement()
:	mStmt{ nullptr }
{}


Statement::Statement(sqlite3_stmt* stmt)
:	mStmt{ stmt }
{}


Statement::Statement(Statement&& statement)
{
	*this = move(statement);
}


Statement::~Statement()
{
	// Delete the statement
	if (mStmt)
	{
		sqlite3_finalize(mStmt);
	}
}


Statement& Statement::operator=(Statement&& other)
{
	SetStmt(other.mStmt);
	other.mStmt = nullptr;
	return *this;
}


void Statement::SetStmt(sqlite3_stmt* stmt)
{
	if (mStmt)
	{
		// Delete previous statement
		sqlite3_finalize(mStmt);
	}

	mStmt = stmt;
}


// Checks a bind result
void Statement::CheckBind(const int result) const
{
	// Check the result
	if (result != SQLITE_OK)
	{
		throw DataSpotException{ "Could not bind", result };
	}
}


// Binds an integer
void Statement::Bind(const int value, const int index) const
{
	CheckBind(sqlite3_bind_int(mStmt, index, value));
}


// Binds a string
void Statement::Bind(const string& value, const int index) const
{
	CheckBind(sqlite3_bind_text(mStmt, index, value.c_str(), -1, SQLITE_TRANSIENT));
}


void Statement::Step() const
{
	// Step
	int stepResult{ sqlite3_step(mStmt) };

	// Check the result
	if (stepResult != SQLITE_ROW)
	{
		throw DataSpotException{ "Could not step", stepResult };
	}
}


int Statement::GetInteger(const unsigned column) const
{
	return sqlite3_column_int(mStmt, column);
}


double Statement::GetDouble(const unsigned column) const
{
	return sqlite3_column_double(mStmt, column);
}


string Statement::GetText(const unsigned column) const
{
	const unsigned char* cValue{ sqlite3_column_text(mStmt, column) };
	return reinterpret_cast<const char*>(cValue);
}


// Resets the statement for reuse
void Statement::Reset() const
{
	sqlite3_reset(mStmt);
}
