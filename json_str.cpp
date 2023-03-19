#include "json_str.h"
#include "jsonc.h"
bool parse_string(std::istream& input, std::string& value) {
    char ch = '\0', delimiter = '"';
    if (!match("\"", input)) {
        if (parser_is_strict()) {
            return false;
        }
        delimiter = '\'';
        if (input.peek() != delimiter) {
            return false;
        }
        input.get(ch);
    }
    while (!input.eof() && input.good()) {
        input.get(ch);
        if (ch == delimiter) {
            break;
        }
        if (ch == '\\') {
            input.get(ch);
            switch (ch) {
            case '\\':
            case '/':
                value.push_back(ch);
                break;
            case 'b'://backspace
                value.push_back('\b');
                break;
            case 'f'://formfeed
                value.push_back('\f');
                break;
            case 'n'://newline
                value.push_back('\n');
                break;
            case 'r'://carriage return
                value.push_back('\r');
                break;
            case 't'://horizontal
                value.push_back('\t');
                break;
            case 'u': {//4 hex
                int num;
                std::stringstream ss;
                for (int i = 0; (!input.eof() && input.good()) && i < 4; ++i) {
                    input.get(ch);
                    ss << std::hex << ch;
                }
                if (input.good() && (ss >> num))
                    value.push_back(static_cast<char>(num));
            }
                    break;
            default:
                if (ch != delimiter) {
                    value.push_back('\\');
                    value.push_back(ch);
                }
                else value.push_back(ch);
                break;
            }
        }
        else {
            value.push_back(ch);
        }
    }
    if (input && ch == delimiter) {
        return true;
    }
    else {
        return false;
    }
}

static std::ostream& stream_string(std::ostream& stream,
    const std::string& string) {
    stream << '"';
    for (std::string::const_iterator i = string.begin(),
        e = string.end(); i != e; ++i) {
        switch (*i) {
        case '"':
            stream << "\\\"";
            break;
        case '\\':
            stream << "\\\\";
            break;
        case '/':
            stream << "\\/";
            break;
        case '\b':
            stream << "\\b";
            break;
        case '\f':
            stream << "\\f";
            break;
        case '\n':
            stream << "\\n";
            break;
        case '\r':
            stream << "\\r";
            break;
        case '\t':
            stream << "\\t";
            break;
        default:
            if (*i < 32) {
                stream << "\\u" << std::hex << std::setw(4) <<
                    std::setfill('0') << static_cast<int>(*i) << std::dec <<
                    std::setw(0);
            }
            else {
                stream << *i;
            }
        }
    }
    stream << '"';
    return stream;
}

  // namespace jsonxx



