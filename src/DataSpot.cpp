#include "Statement.h"
#include "DataSpotException.h"
#include "sqlite3.h"

#include "DataSpot.h"

#define ENTRIES_COUNT 4

using namespace std;
using namespace dataspot;


DataSpot::DataSpot()
:	mDb        {}
,	mStatements{}
{}


DataSpot::~DataSpot()
{}


/// Opens a SQLite database, or create if it does not exist
void DataSpot::Open(const string& path)
{
	mDb.Open(path);
}


/// Returns a prepared statement
Statement& DataSpot::Prepare(const string& query)
{
	Statement* pStmt{};
	
	auto pair = mStatements.find(query);
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


string DataSpot::GetConfigValue(const string& key)
{
	
	string query{ "SELECT value FROM main.config WHERE key = ?;" };
	
	Statement& getConfigStmt = Prepare(query);

	getConfigStmt.Bind(key);
	getConfigStmt.Step();
	string value{ getConfigStmt.GetText(0) };
	getConfigStmt.Reset();

	return value;
}


/// Creates a table
void DataSpot::CreateTable(const string& query)
{
	mDb.CreateTable(query);
}
