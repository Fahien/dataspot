#ifndef DST_DATASPOT_H_
#define DST_DATASPOT_H_

#include <string>
#include <stdexcept>
#include <map>

#include "dataspot/Statement.h"
#include "dataspot/Database.h"


struct sqlite3;
struct sqlite3_stmt;

namespace dataspot
{


class DataSpot
{
public:
	/// Opens a SQLite database, or create if it does not exist
	/// @param[in] path Path to the database
	DataSpot(const std::string& path);
	~DataSpot();

	/// @param[in] query Query to prepare
	/// @return A prepared statement
	Statement& Prepare(const std::string& query);

	/// Creates a table
	/// @param[in] query Create query
	void CreateTable(const std::string& query);

	/// @param[in] key Key to search for
	/// @return A value from the config table
	std::string GetConfigValue(const std::string& key);

private:
	Database mDb;

	std::map<const std::string, Statement> mStatements;
};


}

#endif // DST_DATASPOT_H_
