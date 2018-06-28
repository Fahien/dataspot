#ifndef DST_STATEMENT_H_
#define DST_STATEMENT_H_

#include <string>
#include <iterator>
#include <memory>
#include <cassert>
#include <sqlite3.h>



struct sqlite3_stmt;


namespace dataspot
{

class Statement;

class Row
{
  public:
	Row(Statement& stmt);

	void Next();

	template <typename T>
	T Get(const uint64_t column);

  private:
	Statement& mStmt;
};

template <>
std::string Row::Get<std::string>(const uint64_t column);


class Statement
{
public:
	class Iterator;

	Statement(sqlite3_stmt* stmt = nullptr);
	Statement(Statement&& statement);
	~Statement();

	Statement& operator=(Statement&& other);

	void Bind(const int          value, const int index = 1) const;
	void Bind(const std::string& value, const int index = 1) const;

	Iterator begin() const;
	Iterator end() const;

	int Step() const;

	int         GetInteger(const unsigned column) const;
	double      GetDouble (const unsigned column) const;
	std::string GetText   (const unsigned column) const;

	// Resets the statement for reuse
	void Reset() const;

protected:
	bool HasStatement() const { return mStmt != nullptr; }

	sqlite3_stmt* GetStmt() { return mStmt; }
	void SetStmt(sqlite3_stmt* stmt);

private:
	void CheckBind(const int result) const;

	sqlite3_stmt* mStmt;
	Row mRow;

	friend class DataSpot;
	friend class Row;
};


class Statement::Iterator
{
  public:
	static Iterator END;

	Iterator(const Statement* stmt = nullptr);

	Iterator operator++();
	bool operator!=(const Iterator& other) const;
	const Row& operator*() const { return mStmt->mRow; }

  private:
	const Statement* mStmt;
};



}


#endif // DST_STATEMENT_H_
