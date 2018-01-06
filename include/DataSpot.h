#ifndef DST_DATASPOT_H_
#define DST_DATASPOT_H_

#include <string>

struct sqlite3;
struct sqlite3_stmt;

namespace dataspot
{


class DataSpot
{
public:
	DataSpot();
	~DataSpot();

	void Open(const std::string& path);
	std::string GetConfigValue(const std::string& key);

private:
	void create(const char* path);
	void createConfigTable();
	void populateConfigTable();

	sqlite3* mDb;
	sqlite3_stmt* mGetConfigStmt;
};


}

#endif // DST_DATASPOT_H_
