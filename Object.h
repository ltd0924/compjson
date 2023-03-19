//
//  Object.h
//  test
//
//  Created by 罗婷丹 on 2023/3/14.
//
#pragma once
#ifndef Object_h
#define Object_h
#include "Value.h"
#include "jsonc.h"
#include "json_str.h"
#include <string>
#include <map>
class Object {
 public:
  Object();
  ~Object();
   
  Object(const Object& other);
  Object(const std::string& key, const Value& value);

  template<size_t N>
  Object(const char(&key)[N], const Value& value);


  typedef std::map<std::string, Value*> container;

  template <typename T>
  bool count(const std::string& key) const;

  // Always call count<>() first. If the key doesn't exist, consider
  template <typename T>
  T& get(const std::string& key);
    
  template <typename T>
  const T& get(const std::string& key) const;

  template <typename T>
  const T& get(const std::string& key, const typename identity<T>::type& default_value) const;

  size_t size() const;
  bool empty() const;

  const std::map<std::string, Value*>& kv_map() const;

  //std::string json() const;
  //  
  //std::string xml( unsigned format = JSONx, const std::string &header = std::string(), const std::string &attrib = std::string() ) const;
  //  
  //std::string write( unsigned format ) const;

  void reset();
  bool parse(std::istream &input);
  bool parse(const std::string &input);
    
  
  void add( const Object &other );
  void add( const std::string &key, const Value &value );

   
 
    
  template<typename T>
  Object &operator<<(const T &value);

  Object& operator=(const Object& value);

 protected:
  static bool parse(std::istream& input, Object& object);
  container value_map_;
  std::string str_value;
};



#endif /* Object_h */
