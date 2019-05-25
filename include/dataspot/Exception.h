#ifndef DST_EXCEPTION_H_
#define DST_EXCEPTION_H_

#include <stdexcept>
#include <string>

#include <sqlite3.h>

namespace dataspot
{
class Exception : public std::runtime_error
{
  public:
	Exception( const char* msg, int err ) : std::runtime_error( msg ), message{ sqlite3_errstr( err ) }
	{
	}

	/// @return A string representation of the error
	std::string to_string() const
	{
		return what() + std::string{ ": " } + message;
	}

  private:
	const std::string message;
};

}  // namespace dataspot


#endif  // DST_EXCEPTION_H_
