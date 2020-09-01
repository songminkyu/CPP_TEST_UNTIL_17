#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char *argv[])
{
	opt::options_description desc("all options");
	desc.add_options()
		("user.id", opt::value<std::string>()->default_value(""), "user id")
		("user.public_key", opt::value<std::string>()->default_value(""), "user public key")
		("user.private_key", opt::value<std::string>()->default_value(""), "user private key")
		("wallet.wallet_address", opt::value<std::string>()->default_value(""), "wallet address")
		("relay_server.ip", opt::value<std::string>()->default_value(""), "relay server ip address")
		("coreclient_server.port", opt::value<std::string>()->default_value("50001"), "core client port")
		("ipfs.ip", opt::value<std::string>()->default_value(""), "ipfs ip address")
		("ipfs.port", opt::value<std::string>()->default_value("5001"), "ipfs port")
		("config_option.resource_cpu", opt::value<unsigned int>()->default_value(0), "cpu resource")
		("config_option.resource_ram", opt::value<unsigned int>()->default_value(0), "ram resource")
		("config_option.resource_gpu", opt::value<unsigned int>()->default_value(0), "gpu resource")
		("config_option.task_activation_switching", opt::value<std::string>()->default_value("ON"), "is task activation ???")
		("config_option.notices_activation_switching", opt::value<std::string>()->default_value("ON"), "is task notices activation???")
		("config_option.requestor_download_dir", opt::value<std::string>()->default_value(""), "requestor downsload dir")
		("config_option.provider_download_dir", opt::value<std::string>()->default_value(""), "provider downsload dir")
		("policy.policy_read", opt::value<std::string>()->default_value(""), "policy check")
		("help", "produce help message");

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	try {

		std::wstring wconfig_ini_path = boost::filesystem::current_path().c_str();
		std::string config_ini_path;
		config_ini_path.assign(wconfig_ini_path.begin(), wconfig_ini_path.end());

		std::ifstream ini_file(config_ini_path + "\\" + "config.ini");
		opt::store(opt::parse_config_file<char>(ini_file, desc, true), vm);
	}
	catch (const opt::reading_file& e)
	{
		std::cout << "Failed to open configuration file 'config.ini':" << e.what();
	}

	opt::notify(vm);

	if (vm.count("user.id")) {
		std::cout << "Hi," << vm["user.id"].as<std::string>() << "\n";
	}
	
	std::cout << vm["user.public_key"].as<std::string>() << std::endl;
	std::cout << vm["user.private_key"].as<std::string>() << "\n";
	std::cout << vm["wallet.wallet_address"].as<std::string>() << "\n";
	std::cout << vm["relay_server.ip"].as<std::string>() << "\n";
	std::cout << vm["coreclient_server.port"].as<std::string>() << "\n";
	std::cout << vm["ipfs.ip"].as<std::string>() << "\n";
	std::cout << vm["ipfs.port"].as<std::string>() << "\n";
	std::cout << vm["config_option.resource_cpu"].as<unsigned int>() << "\n";
	std::cout << vm["config_option.resource_ram"].as<unsigned int>() << "\n";
	std::cout << vm["config_option.resource_gpu"].as<unsigned int>() << "\n";
	std::cout << vm["config_option.task_activation_switching"].as<std::string>() << "\n";
	std::cout << vm["config_option.notices_activation_switching"].as<std::string>() << "\n";
	std::cout << vm["config_option.requestor_download_dir"].as<std::string>() << "\n";
	std::cout << vm["config_option.provider_download_dir"].as<std::string>() << "\n";
	std::cout << vm["policy.policy_read"].as<std::string>() << "\n";
	
}