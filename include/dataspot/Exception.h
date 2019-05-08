#ifndef DST_EXCEPTION_H_
#define DST_EXCEPTION_H_

#include <stdexcept>
#include <string>


namespace dataspot
{
class Exception : public std::runtime_error
{
  public:
	Exception( const char* msg, int err ) : std::runtime_error( msg ), error{ err }
	{
	}

	/// Returns the SQLite error code
	int get_error() const
	{
		return error;
	}

	/// Returns a string representation
	std::string to_string() const
	{
		return what();
	}

  private:
	int error;
};

}  // namespace dataspot


#endif  // DST_EXCEPTION_H_
