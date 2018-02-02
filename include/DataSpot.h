#ifndef DST_DATASPOT_H_
#define DST_DATASPOT_H_

#include "Statement.h"
#include "Database.h"

#include <string>
#include <stdexcept>
#include <map>



struct sqlite3;
struct sqlite3_stmt;

namespace dataspot
{


class DataSpot
{
public:
	DataSpot();
	~DataSpot();

	/// Opens a SQLite database, or create if it does not exist
	void Open(const std::string& path);

	/// Returns a prepared statement
	Statement& Prepare(const std::string& query);

	/// Creates a table
	void CreateTable(const std::string& query);

	std::string GetConfigValue(const std::string& key);

private:
	Database  mDb;

	std::map<std::string, Statement> mStatements;
};


}

#endif // DST_DATASPOT_H_
