#include <fasys/fasys.hpp>
namespace
{
	fasys::fasys g_fasys;
}

struct add_library_status
{
	typedef enum
	{
		zzz
	} type;
};

extern "C"
add_library_status::type add_library(const char* library_path)
{

}

extern "C"
struct create_instance_status
{
	typedef enum
	{
		zzz
	} type;
};

extern "C"
struct create_instance_data_type
{
	typedef enum
	{
		zzz
	} type;
};

extern "C"
struct create_instance_result
{
	void* data;
	int data_size;
	create_instance_data_type::type type;
};


extern "C"
create_instance_status::type create_instance(const char* class_name, create_instance_result* result)
{
	
}