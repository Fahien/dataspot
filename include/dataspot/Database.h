#ifndef DST_DATABASE_H_
#define DST_DATABASE_H_

#include <sqlite3.h>
#include <string>

namespace dataspot
{
class Database
{
  public:
	Database() = default;
	Database( const std::string& path );
	~Database();

	/// Opens a SQLite database, or create if it does not exist
	void open( const std::string& path );

	/// @return A prepared statement
	sqlite3_stmt* prepare( const std::string& query );

	/// Creates a table
	void create_table( const std::string& query );

  private:
	void create( const std::string& path );
	void create_config_table();
	void populate_config_table();

	sqlite3* db = nullptr;
};

}  // namespace dataspot

#endif  // DST_DATABASE_H_