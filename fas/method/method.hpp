#ifndef FAS_MEHTOD_METHOD_HPP
#define FAS_MEHTOD_METHOD_HPP

#include <fas/typemanip/empty_type.hpp>

namespace fas{
  
struct method_status
{
  typedef enum{
    wait = 0,
    ready = 1
  } type;
};

/*
template<
  typename ResponseType = empty_type,
  typename ErrorType = empty_type,
  bool     Async = false
>
struct callback_interface
{
  
};

  
template<
  typename ReqestType = empty_type,
  typename ResponseType = empty_type,
  typename ErrorType = empty_type,
  bool     Async = false
>
struct method_interface
{
  typedef callback_interface<ResponseType, ErrorType, Async> callback_interface;

  typedef ReqestType   request_type;
  typedef ResponseType response_type;
  typedef ErrorType    error_type;
  enum { asynchronous = Async };

  
  struct type
  {
    virtual ~type() {};
    virtual method_status::type request()
  };
};*/

template<
  typename ReqestType = empty_type,
  typename ResponseType = empty_type,
  typename ErrorType = empty_type,
  bool     Async = false
>
struct method;

template<
  typename ResponseType = empty_type,
  typename ErrorType = empty_type
>
struct callback_method;

template<
  typename ResponseType,
  typename ErrorType
>
struct callback_pointer;


template<
  typename ResponseType,
  typename ErrorType
>
struct callback_method
{
  typedef ResponseType response_type;
  typedef ErrorType    error_type;
  typedef callback_pointer<ResponseType, ErrorType> pointer_type;

  ~callback_method()
  {
    std::cout << "~callback_method()" << std::endl;
    for ( typename pointer_set::iterator itr = _references.begin(); itr != _references.end(); ++itr)
      (*itr)->release(this);
  }
  
  virtual method_status::type response( const response_type& ) = 0;
  virtual method_status::type error( const error_type& ) = 0;
  
  void add_ref( pointer_type* ptr ) 
  {
    std::cout << "callback_method add_ref" << std::endl;
    _references.insert(ptr); 
  }
  
  void release( pointer_type* ptr ) 
  {
    std::cout << "callback_method release" << std::endl;
    _references.erase(ptr); 
  }
private:
  typedef std::set<pointer_type*> pointer_set;
  pointer_set _references;
};

template<
  typename ResponseType,
  typename ErrorType
>
struct callback_pointer
{
  typedef ResponseType response_type;
  typedef ErrorType    error_type;
  typedef callback_method<ResponseType, ErrorType> callback_type;

  virtual ~callback_pointer() 
  {
    if (_ptr) _ptr->release(this); 
  }
  
  callback_pointer(): _ptr(0) {}
  
  callback_pointer(int): _ptr(0) {}
  
  callback_pointer(const callback_pointer<ResponseType, ErrorType>& cp ): _ptr(cp._ptr) 
  {
    std::cout << "callback_pointer copy" << std::endl;
    if ( _ptr )
      _ptr->add_ref(this);
  }

  void operator = (const callback_pointer<ResponseType, ErrorType>& cp )
  {
    _ptr = cp._ptr;
    std::cout << "callback_pointer operator =" << std::endl;
    if ( _ptr )
      _ptr->add_ref(this);
  }

  callback_pointer(callback_type* ptr): _ptr(ptr) 
  {
    if ( _ptr )
      _ptr->add_ref(this);
  }
  
  operator bool () const { return _ptr!=0; }
  
  callback_type* operator -> () { return _ptr; }
  
  const callback_type* operator -> () const { return _ptr; }
  
  void release( callback_type* ) 
  {
    std::cout << "callback_pointer release" << std::endl;
    _ptr = 0;
  }
  
private:
  callback_type* _ptr;
  
};


template<
  typename ReqestType,
  typename ResponseType,
  typename ErrorType,
  bool     Async
>
struct method
{
  typedef ReqestType   request_type;
  typedef ResponseType response_type;
  typedef ErrorType    error_type;
  enum { asynchronous = Async };
  typedef callback_pointer<ResponseType, ErrorType> callback_pointer_type;
  
  virtual ~method() {};
  virtual method_status::type request( const ReqestType&, ResponseType&, ErrorType&, callback_pointer<ResponseType, ErrorType> callback ) = 0;
   
  
  /*
  typedef typename method_interface<request_type, response_type, error_type, asynchronous >::type interface;
  typedef typename callback_interface<request_type, response_type, error_type, asynchronous >::type callback_interface;
  */
};
  
  
}

#endif

