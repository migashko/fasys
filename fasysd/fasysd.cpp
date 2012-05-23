#include <list>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct fasys_params
{
	bool daemonize;
	bool autoup;
	bool coredump;
	std::string config_file;
	std::string module_path;
	std::string module_list;
};

class module_manager
{
	std::list<std::string> path_list;
	std::list<std::string> module_list;
public:
	void add_path(const std::string& path)
	{
		path_list.push_back(path);
	}

	void add_module(const std::string& module)
	{
		module_list.push_back(module);
	}
};

int main(int argc, char* argv[])
{
	fasys_params fp;
  po::options_description desc("Daemon configurations. Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("daemonize,d", po::value<bool>(&fp.daemonize)->default_value(false)->zero_tokens(), "")
    ("coredump,c", po::value<bool>(&fp.coredump)->default_value(false)->zero_tokens(), "")
    ("autoup,a", po::value<bool>(&fp.autoup)->default_value(false)->zero_tokens(), "")
    ("config-path,C", po::value<std::string>(&fp.config_file), "path to the configuration file")
		("module-path,L", po::value<std::string>(&fp.module_path), "path to the module directory")
		("module-list,l", po::value<std::string>(&fp.module_list), "module file list")
  ;

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
  po::store( parsed, vm );
  po::notify(vm);

	if (vm.count("help") )
  {
    std::cout << desc << std::endl;
		return 0;
  }


	return 0;
}
