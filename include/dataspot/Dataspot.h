#ifndef DST_DATASPOT_H_
#define DST_DATASPOT_H_

#include <map>
#include <stdexcept>
#include <string>

#include "dataspot/Database.h"
#include "dataspot/Statement.h"


struct sqlite3;
struct sqlite3_stmt;

namespace dataspot
{
class Dataspot
{
  public:
	/// Opens a SQLite database, or create if it does not exist
	/// @param[in] path Path to the database
	Dataspot( const std::string& path );
	~Dataspot();

	/// @param[in] query Query to prepare
	/// @return A prepared statement
	Statement& prepare( const std::string& query );

	/// Creates a table
	/// @param[in] query Create query
	void create_table( const std::string& query );

	/// @param[in] key Key to search for
	/// @return A value from the config table
	std::string get_config_value( const std::string& key );

  private:
	Database db;

	std::map<const std::string, Statement> statements;
};


}  // namespace dataspot

#endif  // DST_DATASPOT_H_
