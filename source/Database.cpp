#include "Database.h"
#include "DataSpotException.h"


namespace dst = dataspot;


dst::Database::Database()
:	mDb{ nullptr }
{}


dst::Database::Database(const std::string& path)
:	mDb{ nullptr }
{
	printf("Opening a database at %s\n", path.c_str());

	int openResult{ sqlite3_open_v2(path.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr) };

	// If the database file does not exist, log and create it
	if (openResult != SQLITE_OK)
	{
		printf("No database available, creating a default one at %s", path.c_str());
		create(path);
	}
}


dst::Database::~Database()
{
	if (!mDb)
	{
		return;
	}

	printf("Closing the database\n");

	int closeResult{ sqlite3_close(mDb) };

	if (closeResult != SQLITE_OK)
	{
		printf("Could not close the database [error %d]\n", closeResult);
	}
}


/// Opens a SQLite database, or create if it does not exist
void dst::Database::Open(const std::string& path)
{
	if (mDb)
	{
		printf("Database already opened\n");
		return;
	}

	printf("Opening a database at %s\n", path.c_str());

	int openResult{ sqlite3_open_v2(path.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr) };

	// If the database file does not exist, log and create it
	if (openResult != SQLITE_OK)
	{
		printf("No database available, creating a default one at %s", path.c_str());
		create(path.c_str());
	}
}


/// Returns a prepared statement
sqlite3_stmt* dst::Database::Prepare(const std::string& query)
{
	sqlite3_stmt* statement{ nullptr };

	int prepareResult{ sqlite3_prepare_v2(mDb, query.c_str(), -1, &statement, nullptr) };

	if (prepareResult != SQLITE_OK)
	{
		throw DataSpotException{ sqlite3_errmsg(mDb), prepareResult };
	}

	return statement;
}


void dst::Database::create(const std::string& path)
{
	int createResult{ sqlite3_open_v2(path.c_str(), &mDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) };

	// If could not create the database, complain
	if (createResult != SQLITE_OK)
	{
		printf("Could not create a default database. Why? WHY? [error %d]", createResult);
	}
	else // Populate with some default values
	{
		createConfigTable();
	}
}


void dst::Database::createConfigTable()
{
	CreateTable("CREATE TABLE main.config(key TEXT PRIMARY KEY ASC, value TEXT);");
}


/// Creates a table
void dst::Database::CreateTable(const std::string& query)
{
	printf("Creating a config table");
	sqlite3_stmt* createTableStmt;

	int prepareResult = sqlite3_prepare_v2(
		mDb,
		query.c_str(),
		-1, // Read the statement up to the first zero terminator
		&createTableStmt,
		nullptr);

	// If could not prepare the statement, log the error code
	if (prepareResult != SQLITE_OK)
	{
		throw DataSpotException{ "Could not prepare a statement", prepareResult };
	}
	else // Step the prepared statement
	{
		int stepResult = sqlite3_step(createTableStmt);

		// If could not create the table, log the error code
		if (stepResult != SQLITE_DONE)
		{
			throw DataSpotException{ "Could not step a statement", stepResult };
		}

		// Populate config table with some entries
		populateConfigTable();
	}

	// Delete the statement
	sqlite3_finalize(createTableStmt);
}



void dst::Database::populateConfigTable()
{
	printf("Populating the config table");
	sqlite3_stmt* populateTableStmt;

	int prepareResult = sqlite3_prepare_v2(
		mDb,
		"INSERT INTO main.config(key, value) VALUES(?, ?);",
		-1, // Read the statement up to the first zero terminator
		&populateTableStmt,
		nullptr);

	// If could not prepare the statement, log the error code
	if (prepareResult != SQLITE_OK)
	{
		printf("Could not prepare a statement. [error %d]", prepareResult);
	}
	else // Populate the table
	{
		#define ENTRIES_COUNT 4
		int entries_count = ENTRIES_COUNT;

		const char* keys[ENTRIES_COUNT] =
		{
			"window.title",
			"window.width",
			"window.height",
			"log.level"
		};

		const char* values[ENTRIES_COUNT] =
		{
			"Name",
			"480",
			"270",
			"4"
		};

		for (int i = 0; i < entries_count; ++i)
		{
			int bind_result = sqlite3_bind_text(
				populateTableStmt,
				1, // First parameter: key
				keys[i],
				-1,
				SQLITE_TRANSIENT);

			// If could not bind the key, log the error
			if (bind_result != SQLITE_OK)
			{
				printf("Could not bind a parameter [error %d]", bind_result);
				continue;
			}

			bind_result = sqlite3_bind_text(
				populateTableStmt,
				2, // Second parameter: value
				values[i],
				-1,
				SQLITE_TRANSIENT);

			// If could not bind the value, log the error
			if (bind_result != SQLITE_OK)
			{
				printf("Could not bind a parameter [error %d]", bind_result);
				continue;
			}

			// Step the insert statement
			int step_result = sqlite3_step(populateTableStmt);

			// If could not step the statement, log the error
			if (step_result != SQLITE_DONE)
			{
				printf("Could not step the insert statement: %s", sqlite3_errmsg(mDb));
			}

			// Reset the statement
			int reset_result = sqlite3_reset(populateTableStmt);

			// Check reset result
			if (reset_result != SQLITE_OK)
			{
				printf("Could not reset the statement [error %d]", reset_result);
			}
		}
	}

	// Delete the statement
	sqlite3_finalize(populateTableStmt);
}
