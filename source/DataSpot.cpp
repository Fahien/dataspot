#include <iostream>

#include "Statement.h"
#include "DataSpotException.h"
#include "sqlite3.h"

#include "DataSpot.h"

#define ENTRIES_COUNT 4

namespace dst = dataspot;


dst::DataSpot::DataSpot()
:	mDb        {}
,	mStatements{}
{}


dst::DataSpot::~DataSpot()
{}


/// Opens a SQLite database, or create if it does not exist
void dst::DataSpot::Open(const std::string& path)
{
	mDb.Open(path);
}


/// Returns a prepared statement
dst::Statement& dst::DataSpot::Prepare(const std::string& query)
{
	Statement* pStmt{};
	
	auto& pair = mStatements.find(query);
	if (pair == mStatements.end())
	{
		// Prepare the statement once
		pStmt = &(mStatements[query] = mDb.Prepare(query));
	}
	else
	{
		pStmt = &pair->second;
		pStmt->Reset();
	}

	return *pStmt;
}


std::string dst::DataSpot::GetConfigValue(const std::string& key)
{
	
	std::string query{ "SELECT value FROM main.config WHERE key = ?;" };
	
	Statement& getConfigStmt{ Prepare(query) };

	getConfigStmt.Bind(key);
	getConfigStmt.Step();
	std::string value{ getConfigStmt.GetText(0) };
	getConfigStmt.Reset();

	return value;
}


/// Creates a table
void dst::DataSpot::CreateTable(const std::string& query)
{
	mDb.CreateTable(query);
}
