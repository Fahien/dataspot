#ifndef DST_DATABASE_H_
#define DST_DATABASE_H_

#include <string>
#include <sqlite3.h>


namespace dataspot
{


class Database
{
public:
	Database();
	Database(const std::string& path);
	~Database();

	/// Opens a SQLite database, or create if it does not exist
	void Open(const std::string& path);

	/// Returns a prepared statement
	sqlite3_stmt* Prepare(const std::string& query);

	/// Creates a table
	void CreateTable(const std::string& query);

private:
	void create(const std::string& path);
	void createConfigTable();
	void populateConfigTable();

	sqlite3* mDb;
};


}


#endif // DST_DATABASE_H_