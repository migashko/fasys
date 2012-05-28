#include <list>
#include <boost/program_options.hpp>

#include <fas/method/method.hpp>
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



struct my_method:
  ::fas::method<int, int, int, true>
{};

struct my_method2:
  ::fas::method<long, long, long, true>
{};

class my_class
  : public my_method
  , public my_method2
{
  fas::callback_pointer<int, int> _callback1;
  fas::callback_pointer<long, long> _callback2;
public:
  virtual fas::method_status::type request(const int&, int&, int&, fas::callback_pointer<int, int> callback)
  {
    std::cout << "int" << std::endl;
    _callback1 = callback;
    return fas::method_status::ready;
  }

  virtual fas::method_status::type request(const long&, long&, long&, fas::callback_pointer<long, long> callback)
  {
    std::cout << "long" << std::endl;
    _callback2 = callback;
    return fas::method_status::ready;
  }

  void fire()
  {
    std::cout << "fire" << std::endl;
    if ( _callback1 )
    {
      std::cout << "fire1" << std::endl;
      _callback1->response(0);
    }
  }
};

class my_callback:
  public fas::callback_method<int, int>
{
public:
  virtual fas::method_status::type response(const int&)
  {
     std::cout << "callback int" << std::endl;
    return fas::method_status::ready;
  }

  virtual fas::method_status::type error(const int&)
  {
    return fas::method_status::ready;
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

  my_class m;
  {
  my_callback mc;
  int response =0;
  int error = 0; 

  long response2 =0;
  long error2 = 0; 

  m.request( 0, response, error, &mc );
  m.request( 0, response2, error2, 0 );
  }
  m.fire();

	return 0;
}
