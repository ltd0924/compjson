#include <string>

#include "jsonc.h"
bool match(const char* pattern, std::istream& input) {
    input >> std::ws;// skip whitespace
    const char* cur(pattern);
    char ch(0);
    while (input && !input.eof() && *cur != 0) {
        input.get(ch);
        if (ch != *cur) {
            input.putback(ch);
            if (parse_comment(input))
                continue;
            while (cur > pattern) {
                cur--;
                input.putback(*cur);
            }
            return false;
        }
        else {
            cur++;
        }
    }
    return *cur == 0;
}
bool parse_comment(std::istream& input) {
    if (parser_is_permissive())
        if (!input.eof() && input.peek() == '/')
        {
            char ch0(0);
            input.get(ch0);

            if (!input.eof())
            {
                char ch1(0);
                input.get(ch1);

                if (ch0 == '/' && ch1 == '/')
                {
                    // trim chars till \r or \n
                    for (char ch(0); !input.eof() && (input.peek() != '\r' && input.peek() != '\n'); )
                        input.get(ch);

                    // consume spaces, tabs, \r or \n, in case no eof is found
                    if (!input.eof())
                        input >> std::ws;
                    return true;
                }

                input.unget();
                input.clear();
            }

            input.unget();
            input.clear();
        }

    return false;
}


bool parse_identifier(std::istream& input, std::string& value) {
    input >> std::ws;

    char ch = '\0', delimiter = ':';
    bool first = true;

    while (!input.eof() && input.good()) {
        input.get(ch);

        if (ch == delimiter) {
            input.unget();
            break;
        }

        if (first) {
            if ((ch != '_' && ch != '$') &&
                (ch < 'a' || ch > 'z') &&
                (ch < 'A' || ch > 'Z')) {
                return false;
            }
            first = false;
        }
        if (ch == '_' || ch == '$' ||
            (ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9')) {
            value.push_back(ch);
        }
        else if (ch == '\t' || ch == ' ') {
            input >> std::ws;
        }
    }
    if (input && ch == delimiter) {
        return true;
    }
    else {
        return false;
    }
}

std::ostream& operator<<(std::ostream& stream, const Value& v) {

    if (v.is<Number>()) {
        return stream << v.get<Number>();
    }
    else if (v.is<String>()) {
        return stream_string(stream, v.get<std::string>());
    }
    else if (v.is<Boolean>()) {
        if (v.get<Boolean>()) {
            return stream << "true";
        }
        else {
            return stream << "false";
        }
    }
    else if (v.is<Null>()) {
        return stream << "null";
    }
    else if (v.is<Object>()) {
        return stream << v.get<Object>();
    }
    else if (v.is<Array>()) {
        return stream << v.get<Array>();
    }
    // Shouldn't reach here.
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Array& v) {
    stream << "[";
    Array::container::const_iterator
        it = v.values().begin(),
        end = v.values().end();
    while (it != end) {
        stream << *(*it);
        ++it;
        if (it != end) {
            stream << ", ";
        }
    }
    return stream << "]";
}

std::ostream& operator<<(std::ostream& stream, const Object& v) {
    stream << "{";
    Object::container::const_iterator
        it = v.kv_map().begin(),
        end = v.kv_map().end();
    while (it != end) {
        stream_string(stream, it->first);
        stream << ": " << *(it->second);
        ++it;
        if (it != end) {
            stream << ", ";
        }
    }
    return stream << "}";
}
