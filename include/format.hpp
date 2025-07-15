#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <concepts>

class FormatError : public std::runtime_error {
public:
    explicit FormatError(const std::string& msg)
        : std::runtime_error(msg) {}
};

template <typename T>
concept Formattable = requires(std::ostream& os, T const& v) {
    { os << v } -> std::same_as<std::ostream&>;
};

template <Formattable... Args>
std::string format(const std::string& pattern, Args&&... args) {

    std::vector<std::string> arg_strings;
    arg_strings.reserve(sizeof...(Args));
    ((
        [&] {
            std::ostringstream oss;
            oss << args;
            arg_strings.push_back(oss.str());
        }()
    ), ...);

    std::string result;
    result.reserve(pattern.size());

    for (size_t i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        if (c == '{') {
            size_t j = i + 1;
            if (j >= pattern.size() || !std::isdigit(pattern[j])) {
                throw FormatError("Invalid or empty placeholder at position " + std::to_string(i));
            }
            size_t start = j;
            while (j < pattern.size() && std::isdigit(pattern[j])) ++j;
            if (j >= pattern.size() || pattern[j] != '}') {
                throw FormatError("Unmatched '{' at position " + std::to_string(i));
            }
            std::string numstr = pattern.substr(start, j - start);
            size_t idx = 0;
            try {
                idx = std::stoul(numstr);
            } catch (...) {
                throw FormatError("Bad number in placeholder: " + numstr);
            }
            if (idx >= arg_strings.size()) {
                throw FormatError("Argument index " + std::to_string(idx) + " out of range");
            }
            result += arg_strings[idx];
            i = j;
        }
        else if (c == '}') {
            throw FormatError("Unexpected '}' at position " + std::to_string(i));
        }
        else {
            result += c;
        }
    }

    return result;
}
