#pragma once
#include "Value.h"
#include <vector>
#include <string>
class Array {
public:
    Array();
    ~Array();
    Array(const Array& other);
    Array(const Value& value);

    size_t size() const;
    bool empty() const;
    const std::vector<Value*>& values() const;

    template <typename T>
    bool has(unsigned int i) const;

    template <typename T>
    T& get(unsigned int i);

    template <typename T>
    const T& get(unsigned int i) const;

    template <typename T>
    const T& get(unsigned int i, const typename identity<T>::type& default_value) const;

      

    void add(const Array& other);
    void add(const Value& value);
    /*std::string json() const;
    std::string xml(unsigned format = JSONx, const std::string& header = std::string(), const std::string& attrib = std::string()) const;

    std::string write(unsigned format) const { return format == JSON ? json() : xml(format); }*/
    void reset();
    //bool parse(std::istream& input);
    bool parse(std::istream& input);
    bool parse(const std::string& input);
    typedef std::vector<Value*> container;
    
    
    Array& operator<<(const Array& other);
    Array& operator<<(const Value& value);
    Array& operator=(const Array& other);
    Array& operator=(const Value& value);
    
protected:
    
    container values_;
};