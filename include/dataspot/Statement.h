#ifndef DST_STATEMENT_H_
#define DST_STATEMENT_H_

#include <sqlite3.h>
#include <cassert>
#include <iterator>
#include <memory>
#include <string>


struct sqlite3_stmt;


namespace dataspot
{
class Statement;

class Row
{
  public:
	Row( Statement& stmt );

	void next();

	template <typename T>
	T get( uint64_t column );

  private:
	Statement& stmt;
};

template <>
std::string Row::get<std::string>( uint64_t column );


class Statement
{
  public:
	class Iterator;

	Statement( sqlite3_stmt* stmt = nullptr );
	Statement( Statement&& statement );
	~Statement();

	Statement& operator=( Statement&& other );

	void bind( int value, int index = 1 ) const;
	void bind( const std::string& value, int index = 1 ) const;

	Iterator begin() const;
	Iterator end() const;

	int step() const;

	int         get_integer( unsigned column ) const;
	double      get_double( unsigned column ) const;
	std::string get_text( unsigned column ) const;

	// Resets the statement for reuse
	void reset() const;

  protected:
	sqlite3_stmt* get_stmt()
	{
		return stmt;
	}
	void set_stmt( sqlite3_stmt* stmt );

  private:
	void CheckBind( const int result ) const;

	sqlite3_stmt* stmt = nullptr;

	Row row;

	friend class DataSpot;
	friend class Row;
};


class Statement::Iterator
{
  public:
	static Iterator END;

	Iterator( const Statement* stmt = nullptr );

	Iterator   operator++();
	bool       operator!=( const Iterator& other ) const;
	const Row& operator*() const
	{
		return stmt->row;
	}

  private:
	const Statement* stmt = nullptr;
};


}  // namespace dataspot


#endif  // DST_STATEMENT_H_
