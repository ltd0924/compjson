#include "Array.h"

Array::Array() : values_() {}


Array::Array(const Array& v) {
    add(v);
}

Array::Array(const Value& v) {
    values_.push_back(new Value(v));
}

Array::~Array() {
    reset();
}


size_t Array::size()const {
    return values_.size();
}

bool Array::empty() const {
    return values_.empty();
}

const std::vector<Value*>& Array::values() const {
    return values_;
}
template <typename T>
bool Array::has(unsigned int i) const {
    if (i >= size()) {
        return false;
    }
    else {
        Value* v = values_.at(i);
        return v->is<T>();
    }
}

template <typename T>
T& Array::get(unsigned int i) {
    JSONXX_ASSERT(i < size());
    Value* v = values_.at(i);
    return v->get<T>();
}

template <typename T>
const T& Array::get(unsigned int i) const {
    JSONXX_ASSERT(i < size());
    const Value* v = values_.at(i);
    return v->get<T>();
}

template <typename T>
const T& Array::get(unsigned int i, const typename identity<T>::type& default_value) const {
    if (has<T>(i)) {
        const Value* v = values_.at(i);
        return v->get<T>();
    }
    else {
        return default_value;
    }
}





void Array::add(const Array& other) {
    if (this != &other) {
        container::const_iterator
            it = other.values_.begin(),
            end = other.values_.end();
        for (/**/; it != end; ++it) {
            values_.push_back(new Value(**it));
        }
    }
    else {
        // recursion is supported here
        add(Array(*this));
    }
}
void Array::add(const Value& value) {
    values_.push_back(new Value(value));
}
void Array::reset() {
    for (container::iterator i = values_.begin(); i != values_.end(); ++i) {
        delete* i;
    }
    values_.clear();
}

bool Array::parse(std::istream& input) {

    if (!match("[", input)) {
        return false;
    }
    if (match("]", input)) {
        return true;
    }

    do {
        Value* v = new Value();
        if (!v->parse(input)) {
            delete v;
            break;
        }
        values_.push_back(v);
    } while (match(",", input));

    if (!match("]", input)) {
        return false;
    }
    return true;
}

bool  Array::parse(const std::string& input) {
    std::istringstream is(input);
    return parse(is);
}

Array& Array::operator<<(const Array& other) {
    add(other);
    return *this;
}
Array& Array::operator<<(const Value& value) {
    add(value);
    return *this;
}
Array& Array::operator=(const Array& other) {
    if (this != &other) {
        reset();
        add(other);
    }
    return *this;   
}
Array& Array::operator=(const Value& value) {
    reset();
    add(value);
    return *this;
}