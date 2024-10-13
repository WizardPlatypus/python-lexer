#include "PythonLexer.h"
#include "lexer.hpp"
#include "escape.hpp"
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <sstream>

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
	std::cout << std::endl;

	std::cout << colorize(text, matches, assigned);
	std::cout << std::endl;

	for (const auto& [label, xem] : l.lexems) {
		std::cout << xem.display(label, assigned) << std::endl;
	}
	std::cout << std::endl;
}

