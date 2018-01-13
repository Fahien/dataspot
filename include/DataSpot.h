#ifndef DST_DATASPOT_H_
#define DST_DATASPOT_H_

#include "Statement.h"

#include <string>
#include <stdexcept>



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
	sqlite3_stmt* Prepare(const std::string& query);

	/// Creates a table
	void CreateTable(const std::string& query);

	std::string GetConfigValue(const std::string& key);

private:
	void create(const char* path);
	void createConfigTable();
	void populateConfigTable();

	sqlite3* mDb;
	Statement mGetConfigStmt;
};


}

#endif // DST_DATASPOT_H_
