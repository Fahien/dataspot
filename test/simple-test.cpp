#include "DataSpot.h"

#include <iostream>
#include <cassert>
#include <cstdlib>

namespace dst = dataspot;


int main()
{
	dst::DataSpot data;

	try
	{
		data.Open("test.data");
		assert(data.GetConfigValue("window.title")  == "Name");
		assert(data.GetConfigValue("window.width")  == "480" );
		assert(data.GetConfigValue("window.height") == "270" );
		assert(data.GetConfigValue("log.level")     == "4"   );
	}
	catch (const dst::DataSpotException& e)
	{
		std::cerr << e.ToString() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
