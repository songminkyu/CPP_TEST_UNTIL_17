#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>

namespace opt = boost::program_options;

int main(int argc, char* argv[])
{
	opt::options_description desc("all options");
	desc.add_options()
		("conv.apples,a", opt::value<int>()->default_value(10), "how many apples do you have")
		("conv.oranges,o", opt::value<int>()->default_value(20), "how many oranges do you have")
		("conv.name", opt::value<std::string>()->default_value("songminkyu"), "your name")
		("help", "produce help message");

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	try {
		std::ifstream ini_file("config.ini");
		opt::store(opt::parse_config_file<char>(ini_file, desc, true), vm);
		std::cout << "try" << "\n";
	}
	catch (const opt::reading_file& e)
	{
		std::cout << "Failed to open configuration file 'config.ini':" << e.what();
	}

	opt::notify(vm);

	if (vm.count("conv.name")) {
		std::cout << "Hi," << vm["conv.name"].as<std::string>() << "\n";
	}

	std::cout << "Fruits count: " << vm["conv.apples"].as<int>() + vm["conv.oranges"].as<int>() << "\n";

	return 0;
}