// PythonLexer.cpp : Defines the entry point for the application.
//
#include "PythonLexer.h"
#include <regex>
#include <string>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "lexer.hpp"
#include "escape.hpp"

#define NAMEOF(name) #name

using str = std::string;

bool test_lexer() {
	Lexer l;
    if (!l.insert("<a>", "([1-9])")) {
        std::cerr << "Failed to insert <a>" << std::endl;
		return false;
    }
    if (!l.insert("<b>", "([0-9])")) {
        std::cerr << "Failed to insert <b>" << std::endl;
		return false;
    }
    if (!l.insert("<decimal>", "<a><b>*")) {
        std::cerr << "Failed to insert <decimal>" << std::endl;
		return false;
    }

    if (l["<a>"].regex != "([1-9])") {
        std::cerr << "Wrong <a>: " << l["<a>"].regex << std::endl;
		return false;
    }
    if (l["<b>"].regex != "([0-9])") {
        std::cerr << "Wrong <b>: " << l["<b>"].regex << std::endl;
		return false;
    }
    if (l["<decimal>"].regex != "([1-9])([0-9])*") {
        std::cerr << "Wrong <decimal>: " << l["<decimal>"].regex << std::endl;
		return false;
    }

    Lexem decimal = l["<decimal>"];
    if (decimal.anchors[0].pos != 1) {
        std::cerr << "Anchor <a> at wrong position: " << decimal.anchors[0].pos << std::endl;
        return false;
    }
    if (decimal.anchors[1].pos != 2) {
        std::cerr << "Anchor <b> at wrong position: " << decimal.anchors[1].pos << std::endl;
        return false;
    }

    std::string test = "1 10 23     34 asd   87889 x984      10101";
    std::regex regex(decimal.regex);
    auto xbegin = std::sregex_iterator(test.begin(), test.end(), regex);
    auto xend = std::sregex_iterator();

    std::vector<std::string> matches = { "1", "10", "23", "34", "87889", "984", "10101" };
    if (std::distance(xbegin, xend) > matches.size()) {
        std::cout << "Expected less matches" << std::endl;
        return false;
    }
    int ptr = 0;

    for (auto i = xbegin; i != xend; ++i) {
        std::smatch match = *i;
        if (match.str() != matches[ptr]) {
            std::cerr << "Expected '" << matches[ptr] << "', got '" << match.str() << "'" << std::endl;
            return false;
        }
        if (match[decimal.anchors[0].pos].str() != matches[ptr].substr(0, 1)) {
            std::cerr << "Wrong <a>: " << match[decimal.anchors[0].pos].str() << std::endl;
            return false;
        }
        /*
        if (match[decimal.anchors[1].pos].str() != matches[ptr].substr(1)) {
            std::cerr << "Wrong <b>: " << match[decimal.anchors[0].pos].str() << std::endl;
            return false;
        }
        // */
        ptr += 1;
    }

    std::cout << "PASSING" << std::endl;
	return true;
}

struct Match {
    std::string label;
    long long pos;
    long long len;
};

int main()
{
#if 0
    test_lexer();
#else
    // https://docs.python.org/3/reference/lexical_analysis.html

    str test = R"(7     2147483647                        0o177    0b100110111
3     79228162514264337593543950336     0o377    
0xdeadbeef
	  100_000_000_000                   0b_1110_0101
3.14    10.    .001    1e100    3.14e-10    0e0    3.14_15_93
3.14j   10.j    10j     .001j   1e100j   3.14e-10j   3.14_15_93j)";

    Lexer l;
    add_integers(l);
    add_float_numbers(l);
    add_strings(l);
    add_reserved(l);
    add_identifiers(l);

    if (!l.insert("<lexem>", "(<string_literal>|<bytes_literal>|<comment>|<keywords>|<identifier>|<operators>|<forbidden>|<imag_number>|<float_number>|<delimeters>|<bin_integer>|<oct_integer>|<hex_integer>|<dec_integer>)")) {
        std::cerr << "Could not insert <lexem>" << std::endl;
        return -1;
    }
    auto lexem = l["<lexem>"];

    auto xbegin = std::sregex_iterator(test.begin(), test.end(), lexem.x);
    auto xend = std::sregex_iterator();

    std::vector<Match> matches;
    std::cout << "Found " << std::distance(xbegin, xend) << std::endl;

    std::unordered_map<std::string, std::vector<std::string>> count;

    for (auto i = xbegin; i != xend; ++i) {
        std::smatch match = *i;
        // std::cout << match.str() << std::endl;
        for (auto a : lexem.anchors) {
            if (!match[a.pos].matched) {
                continue;
            }
            if (count.contains(a.name)) {
                count[a.name].push_back(match[a.pos]);
            }
            else {
                std::vector<std::string> x = { match[a.pos] };
                count[a.name] = x;
            }
            matches.push_back(Match{ a.name, match.position(), match.length() });
        }
    }

    for (const auto& [label, values] : count) {
        std::cout << label << ":";
        for (const auto& value : values) {
            std::cout << ' ' << value;
        }
        std::cout << std::endl;
    }

    std::unordered_map<std::string, int> bg;
    int bg_min = 41;
    int bg_max = 46;
    int bg_offset = 0;
    int cursor = 0;
    std::ostringstream buffer;
    for (const auto& m : matches) {
        if (!bg.contains(m.label)) {
            bg[m.label] = bg_min + bg_offset;
            bg_offset += 1;
            if (bg_min + bg_offset > bg_max) {
                bg_offset = 0;
            }
        }
		buffer << test.substr(cursor, m.pos - cursor);
		buffer << format(test.substr(m.pos, m.len), 39, bg[m.label]);
        cursor = m.pos + m.len;
    }
    if (cursor < test.size()) {
        buffer << test.substr(cursor);
    }
    std::cout << buffer.str();

#endif
}

