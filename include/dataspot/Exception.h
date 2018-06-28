#ifndef DST_EXCEPTION_H_
#define DST_EXCEPTION_H_

#include <string>
#include <stdexcept>


namespace dataspot
{


class Exception : public std::runtime_error
{
public:
	Exception(const char* msg, int error)
	:	std::runtime_error(msg)
	,	mError{ error }
	{}

	/// Returns the SQLite error code
	int GetError() const { return mError; }

	/// Returns a string representation
	std::string ToString() const
	{
		return what();
	}

private:
	int mError;
};

}


#endif // DST_EXCEPTION_H_
