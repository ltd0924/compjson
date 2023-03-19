#include "Value.h"
template<>
inline bool Value::is<Value>() const {
    return true;
}

template<>
inline bool Value::is<Null>() const {
    return type_ == NULL_;
}

template<>
inline bool Value::is<Boolean>() const {
    return type_ == BOOL_;
}

template<>
inline bool Value::is<String>() const {
    return type_ == STRING_;
}

template<>
inline bool Value::is<Number>() const {
    return type_ == NUMBER_;
}

template<>
inline bool Value::is<Array>() const {
    return type_ == ARRAY_;
}

template<>
inline bool Value::is<Object>() const {
    return type_ == OBJECT_;
}

template<>
inline Value& Value::get<Value>() {
    return *this;
}

template<>
inline const Value& Value::get<Value>() const {
    return *this;
}

template<>
inline bool& Value::get<Boolean>() {
    JSONXX_ASSERT(is<Boolean>());
    return bool_value_;
}



bool parse_null(std::istream& input) {
    if (match("null", input)) {
        return true;
    }
    if (parser_is_strict()) {
        return false;
    }
    return (input.peek() == ',');
}

bool parse_bool(std::istream& input, Boolean& value) {
    if (match("true", input)) {
        value = true;
        return true;
    }
    if (match("false", input)) {
        value = false;
        return true;
    }
    return false;
}


bool parse_array(std::istream& input, Array& array) {
    return array.parse(input);
}

bool parse_object(std::istream& input, Object& object) {
    return object.parse(input);
}


bool parse_number(std::istream& input, Number& value) {
    input >> std::ws;
    std::streampos rollback = input.tellg();
    IOStateMasker masker(input);
    input >> value;
    if (input.fail()) {
        input.clear();
        input.seekg(rollback);
        return false;
    }
    return true;
}

Value::Value() : type_(INVALID_) {}

void Value::reset() {
    if (type_ == STRING_) {
        delete string_value_;
        string_value_ = 0;
    }
    else if (type_ == OBJECT_) {
        delete object_value_;
        object_value_ = 0;
    }
    else if (type_ == ARRAY_) {
        delete array_value_;
        array_value_ = 0;
    }
}
bool Value::parse(std::istream& input) {
    return parse(input, *this);
}

bool Value::parse(std::istream& input) {
    return parse(input, *this);
}

bool Value::parse(std::istream& input, Value& value) {
    value.reset();

    std::string string_value;
    if (parse_string(input, string_value)) {
        value.string_value_ = new std::string();
        value.string_value_->swap(string_value);
        value.type_ = STRING_;
        return true;
    }
    if (parse_number(input, value.number_value_)) {
        value.type_ = NUMBER_;
        return true;
    }

    if (parse_bool(input, value.bool_value_)) {
        value.type_ = BOOL_;
        return true;
    }
    if (parse_null(input)) {
        value.type_ = NULL_;
        return true;
    }
    if (input.peek() == '[') {
        value.array_value_ = new Array();
        if (parse_array(input, *value.array_value_)) {
            value.type_ = ARRAY_;
            return true;
        }
        delete value.array_value_;
        value.array_value_ = 0;
    }
    value.object_value_ = new Object();
    if (parse_object(input, *value.object_value_)) {
        value.type_ = OBJECT_;
        return true;
    }
    delete value.object_value_;
    value.object_value_ = 0;
    return false;
}