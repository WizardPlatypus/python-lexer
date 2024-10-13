// PythonLexer.cpp : Defines the entry point for the application.
//
#include "PythonLexer.h"
#include <regex>
#include <string>
#include <cstring>
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

std::vector<Format> good_colors() {
	std::vector<Format> colors = {
		Format(30, 41, 3),
		Format(30, 42, 3),
		Format(30, 43, 3),
		Format(30, 44, 3),
		Format(30, 45, 3),
		Format(30, 46, 3),
		Format(30, 47, 3),

		Format(30, 101, 3),
		Format(30, 102, 3),
		Format(30, 103, 3),
		Format(30, 104, 3),
		Format(30, 105, 3),
		Format(30, 106, 3),
		Format(30, 107, 3),
	};
	return colors;
}

std::unordered_map<std::string, Format> assign(std::vector<std::string> labels, std::vector<Format> colors) {
	std::unordered_map<std::string, Format> m;
	for (int i = 0; i < labels.size(); ++i) {
		m.insert({ labels[i], colors[i % colors.size()] });
	}
	return m;
}

// https://docs.python.org/3/reference/lexical_analysis.html
int main()
{
#if 0
	test_lexer();
#else
	std::ostringstream buffer;
	std::string line;
	while (std::getline(std::cin, line)) {
		buffer << line << std::endl;
	}
	std::string text = buffer.str();

	auto l = python::init();
	if (!l.insert("<lexem>", "(<string_literal>|<comment>|<keywords>|<identifier>|<forbidden>|<imag_number>|<float_number>|<delimeters>|<operators>|<bin_integer>|<oct_integer>|<hex_integer>|<dec_integer>)")) {
		std::cerr << "Could not insert <lexem>" << std::endl;
		return -1;
	}

	auto lexem = l["<lexem>"];

	std::vector<std::string> labels;
	for (const auto& a : lexem.anchors) {
		labels.push_back(a.name);
	}
	auto assigned = assign(labels, good_colors());

	auto matches = lexem.match(text);
	std::cout << "Found " << matches.size() << std::endl;
	std::cout << colorize(text, matches, assigned);
#endif
}

