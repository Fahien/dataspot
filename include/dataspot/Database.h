#ifndef DST_DATABASE_H_
#define DST_DATABASE_H_

#include <string>
#include <variant>

#include <sqlite3.h>

#include "dataspot/Statement.h"

namespace dataspot
{
class Error
{
  public:
	Error( int code, sqlite3& db ) : message{ sqlite3_errstr( code ) + std::string{ ": " } + sqlite3_errmsg( &db ) }
	{
	}

	const std::string& get_message() const
	{
		return message;
	}

  private:
	std::string message;
};

class Database
{
  public:
	/// Creates a new database
	/// @param path Database file path
	/// @return A newly created database
	static std::variant<Database, Error> create( const std::string& path );

	/// Opens an existing database
	/// It fails if the database does not exist
	/// @param path Database file path
	static std::variant<Database, Error> open( const std::string& path );

	Database( Database&& other );

	~Database();

	/// @return A prepared statement
	std::variant<Statement, Error> prepare( const std::string& query );

	/// Creates a table
	void create_table( const std::string& query );

  private:
	Database( sqlite3& db );

	void create_config_table();
	void populate_config_table();

	sqlite3* db = nullptr;
};

}  // namespace dataspot

#endif  // DST_DATABASE_H_