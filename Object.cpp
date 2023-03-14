//
//  Object.cpp
//  test
//
//  Created by 罗婷丹 on 2023/3/14.
//

#include "Object.h"
#include "jsonc.h"
Object::Object() : value_map_() {}

Object::Object(const Object &other) {
  insert(other);
}
Object::Object(const std::string &key, const Value &value) {
  insert(key,value);
}

//copy
Object &Object::operator=(const Object &other) {
  if (this != &other) {
    reset();
    insert(other);
  }
  return *this;
}


Object::~Object() {
    reset();
}


void Object::insert(const Object &other ) {
  if (this != &other) {// not same address
    container::const_iterator
        it = other.value_map_.begin(),
        end = other.value_map_.end();
    for (/**/ ; it != end ; ++it) {
      container::iterator found = value_map_.find(it->first);
      if( found != value_map_.end() ) {//found the same key
        delete found->second;
      }
      value_map_[ it->first ] = new Value( *it->second );
    }
  } else {
    // recursion is supported here
    insert( Object(*this) );
  }
}
void Object::insert( const std::string &key, const Value &value ) {
  container::iterator found = value_map_.find(key);
  if( found != value_map_.end() ) {
    delete found->second;
  }
  value_map_[ key ] = new Value( value );
}

Object &Object::operator<<(const Value &value) {
  if (odd.empty()) {
    str_value = value.get<String>();
  } else {
    import( Object(odd, value) );
    str_value.clear();
  }
  return *this;
}
Object &Object::operator<<(const Object &value) {
  import( std::string(str_value),value);
  str_value.clear();
  return *this;
}

size_t Object::size() const {
  return value_map_.size();
}

bool Object::empty() const {
  return value_map_.size() == 0;
}

std::string Object::write( unsigned format ) const {
  return format == JSON ? json() : xml(format);
}
void Object::reset() {
  container::iterator it;
  for (it = value_map_.begin(); it != value_map_.end(); ++it) {
    delete it->second;
  }
  value_map_.clear();
}
    
//bool Object::parse(std::istream &input) {
//  return parse(input,*this);
//}
//    
//bool Object::parse(const std::string &input) {
//  std::istringstream is( input );
//  return parse(is,*this);
//}
//

bool Object::parse(std::istream& input) {
    obj.reset();

    if (!match("{", input)) {
        return false;
    }
    if (match("}", input)) {
        return true;
    }

    do {
        std::string key;
        if (unquoted_keys_are_enabled()) {
            if (!parse_identifier(input, key)) {
                if (parser_is_permissive()) {
                    if (input.peek() == '}')
                        break;
                }
                return false;
            }
        }
        else {
            if (!parse_string(input, key)) {
                if (parser_is_permissive()) {
                    if (input.peek() == '}')
                        break;
                }
                return false;
            }
        }
        if (!match(":", input)) {
            return false;
        }
        Value* v = new Value();
        if (!parse_value(input, *v)) {
            delete v;
            break;
        }
  //repeat key
        if (value_map_.find(key) == value_map_.end()) {
          value_map_[key] = v;
        } else {
          if (parser_is_permissive()) {
            delete value_map_[key];//cover
            value_map_[key] = v;
          } else {
            delete v;
            return false;
          }
        }
    } while (match(",", input));


    if (!match("}", input)) {
        return false;
    }

    return true;
}
