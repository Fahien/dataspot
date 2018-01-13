#ifndef DST_STATEMENT_H_
#define DST_STATEMENT_H_

#include <string>

struct sqlite3_stmt;


namespace dataspot
{


class Statement
{
public:
	Statement();
	Statement(sqlite3_stmt* stmt);
	Statement(Statement&& statement);
	~Statement();

	Statement& operator=(Statement&& other);

	// Binds an int
	void Bind(const int value) const;

	// Binds a string
	void Bind(const std::string& value) const;

	void Step() const;

	std::string GetText(const unsigned column) const;

	// Resets the statement for reuse
	void Reset() const;

protected:
	bool HasStatement() const { return mStmt != nullptr; }

	sqlite3_stmt* GetStmt() { return mStmt; }
	void SetStmt(sqlite3_stmt* stmt);

private:
	void CheckBind(const int result) const;

	sqlite3_stmt* mStmt;

	friend class DataSpot;
};


}


#endif // DST_STATEMENT_H_
