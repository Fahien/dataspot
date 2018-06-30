#include <cassert>
#include <cstdlib>
#include <iostream>
#include <logspot/Logger.h>

#include "dataspot/DataSpot.h"
#include "dataspot/Exception.h"

using namespace std;
namespace lst = logspot;
namespace dst = dataspot;


int main()
{

	try
	{
		dst::DataSpot data{ "test.data" };
		assert(data.GetConfigValue("window.title")  == "Name");
		assert(data.GetConfigValue("window.width")  == "480" );
		assert(data.GetConfigValue("window.height") == "270" );
		assert(data.GetConfigValue("log.level")     == "4"   );

		auto& stmt = data.Prepare("SELECT * from main.config;");
		for(auto row : stmt)
		{
			auto key = row.Get<string>(0);
			auto value = row.Get<string>(1);
			cout << "(" << key << ", " << value << ")" << endl;
		}

		for(auto row : stmt)
		{
			auto key = row.Get<string>(0);
			auto value = row.Get<string>(1);
			cout << "(" << key << ", " << value << ")" << endl;
		}
	}
	catch (const dst::Exception& e)
	{
		lst::Logger::log.Error(e.ToString());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
