//
//  Value.h
//  test
//
//  Created by ÂÞæÃµ¤ on 2023/3/14.
//
#pragma once
#include "jsonc.h"
#include "Array.h"
#include "Object.h"
//#ifndef Value_h
//#define Value_h
class Value {
public:

    Value();
    Value(const Value& other);
    template<typename T>
    Value(const T& t) : type_(INVALID_) { add(t); }
    template<size_t N>
    Value(const char(&t)[N]) : type_(INVALID_) { add(std::string(t)); }

    ~Value() { reset(); }
    void reset();

    template<typename T>
    void add(const T&) {
        reset();
        type_ = INVALID_;
    }
    void add(const bool& b) {
        reset();
        type_ = BOOL_;
        bool_value_ = b;
    }

    void add(const Null&) {
        reset();
        type_ = NULL_;
    }
    void add(const String& s) {
        reset();
        type_ = STRING_;
        *(string_value_ = new String()) = s;
    }
    void add(const char* s) {
        reset();
        type_ = STRING_;
        *(string_value_ = new String()) = s;
    }
    void add(const Array& a) {
        reset();
        type_ = ARRAY_;
        *(array_value_ = new Array()) = a;
    }
    void add(const Object& o) {
        reset();
        type_ = OBJECT_;
        *(object_value_ = new Object()) = o;
    }
    void add(const Value& other) {
        if (this != &other)
            switch (other.type_) {
            case NULL_:
                add(Null());
                break;
            case BOOL_:
                add(other.bool_value_);
                break;
            case NUMBER_:
                add(other.number_value_);
                break;
            case STRING_:
                add(*other.string_value_);
                break;
            case ARRAY_:
                add(*other.array_value_);
                break;
            case OBJECT_:
                add(*other.object_value_);
                break;
            case INVALID_:
                type_ = INVALID_;
                break;
            default:
                JSONXX_ASSERT(!"not implemented");
            }
    }
    // define ?
#define $number(TYPE) \
  void add( const TYPE &n ) { \
    reset(); \
    type_ = NUMBER_; \
    number_value_ = static_cast<long double>(n); \
  }
    $number(char)
        $number(int)
        $number(long)
        $number(long long)
        $number(unsigned char)
        $number(unsigned int)
        $number(unsigned long)
        $number(unsigned long long)
        $number(float)
        $number(double)
        $number(long double)
#undef $number
#if JSONXX_COMPILER_HAS_CXX11 > 0
        void add(const std::nullptr_t&) {
        reset();
        type_ = NULL_;
    }
#endif
    template<typename T>
    Value& operator <<(const T& t) {
        add(t);
        return *this;
    }
    template<typename T>
    Value& operator =(const T& t) {
        reset();
        add(t);
        return *this;
    }
    
    

    bool parse(std::istream& input);
    bool parse(const std::string& input);

    template<typename T>
    bool is() const;

    template<typename T>
    T& get();

    template<typename T>
    const T& get() const;

    bool empty() const;


    enum {
        NUMBER_,
        STRING_,
        BOOL_,
        NULL_,
        ARRAY_,
        OBJECT_,
        INVALID_
    } type_;
    union {
        Number number_value_;
        String* string_value_;
        Boolean bool_value_;
        Array* array_value_;
        Object* object_value_;
    };

protected:
    static bool parse(std::istream& input, Value& value);
};



//#endif /* Value_h */
