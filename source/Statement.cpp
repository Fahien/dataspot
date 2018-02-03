#include "Statement.h"

#include "DataSpotException.h"
#include "sqlite3.h"

#include <iostream>

namespace dst = dataspot;


dst::Statement::Statement()
:	mStmt{ nullptr }
{}


dst::Statement::Statement(sqlite3_stmt* stmt)
:	mStmt{ stmt }
{}


dst::Statement::Statement(dst::Statement&& statement)
{
	*this = std::move(statement);
}


dst::Statement::~Statement()
{
	// Delete the statement
	if (mStmt)
	{
		sqlite3_finalize(mStmt);
		printf("Finilizing statement\n");
	}
}


dst::Statement& dst::Statement::operator=(dst::Statement&& other)
{
	SetStmt(other.mStmt);
	other.mStmt = nullptr;
	return *this;
}


void dst::Statement::SetStmt(sqlite3_stmt* stmt)
{
	if (mStmt)
	{
		// Delete previous statement
		sqlite3_finalize(mStmt);
	}

	mStmt = stmt;
}


// Checks a bind result
void dst::Statement::CheckBind(const int result) const
{
	// Check the result
	if (result != SQLITE_OK)
	{
		throw DataSpotException{ "Could not bind", result };
	}
}


// Binds an integer
void dst::Statement::Bind(const int value, const int index) const
{
	CheckBind(sqlite3_bind_int(mStmt, index, value));
}


// Binds a string
void dst::Statement::Bind(const std::string& value, const int index) const
{
	CheckBind(sqlite3_bind_text(mStmt, index, value.c_str(), -1, SQLITE_TRANSIENT));
}


void dst::Statement::Step() const
{
	// Step
	int stepResult{ sqlite3_step(mStmt) };

	// Check the result
	if (stepResult != SQLITE_ROW)
	{
		throw DataSpotException{ "Could not step", stepResult };
	}
}


int dst::Statement::GetInteger(const unsigned column) const
{
	return sqlite3_column_int(mStmt, column);
}


std::string dst::Statement::GetText(const unsigned column) const
{
	const unsigned char* cValue{ sqlite3_column_text(mStmt, column) };
	return reinterpret_cast<const char*>(cValue);
}


// Resets the statement for reuse
void dst::Statement::Reset() const
{
	sqlite3_reset(mStmt);
}
