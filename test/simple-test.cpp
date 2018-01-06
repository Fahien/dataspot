#include "DataSpot.h"

namespace dst = dataspot;


int main()
{
	dst::DataSpot data;

	data.Open("test.data");

	return 0;
}
