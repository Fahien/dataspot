#include <sqlite3.h>

#include "dataspot/Exception.h"

#include "dataspot/Statement.h"

using namespace std;
using namespace dataspot;


Statement::Statement(sqlite3_stmt* stmt)
:	mStmt{ stmt }
,	mRow{ *this }
{}


Statement::Statement(Statement&& statement)
: mRow{ move(statement.mRow) }
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
		throw Exception{ "Could not bind", result };
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


Statement::Iterator Statement::begin() const
{
	Step();
	return Statement::Iterator{ this };
}


Statement::Iterator Statement::end() const
{
	return Iterator::END;
}


int Statement::Step() const
{
	return sqlite3_step(mStmt);
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


Row::Row(Statement& stmt)
:	mStmt{ stmt }
{}


void Row::Next()
{
	int stepResult = mStmt.Step();
	
	// Check the result
	if (stepResult != SQLITE_ROW)
	{
		throw Exception{ "Could not step", stepResult };
	}
}


template <>
std::string Row::Get<std::string>(const uint64_t column)
{
	assert(column >= 0);
	const unsigned char* cValue{ sqlite3_column_text(mStmt.GetStmt(), static_cast<int>(column)) };
	return reinterpret_cast<const char*>(cValue);
}


Statement::Iterator Statement::Iterator::END{ nullptr };


Statement::Iterator::Iterator(const Statement* stmt)
: mStmt{ stmt }
{}


bool Statement::Iterator::operator!=(const Iterator& other) const
{
	return mStmt != other.mStmt;
}


Statement::Iterator Statement::Iterator::operator++()
{
	auto stepResult = mStmt->Step();
	
	// Check the result
	if (stepResult != SQLITE_ROW)
	{
		mStmt = nullptr;
	}

	return *this;
}
