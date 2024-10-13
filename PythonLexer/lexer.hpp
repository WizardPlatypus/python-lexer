#pragma once
#include "escape.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

struct Match {
	std::string label;
	long long pos;
	long long len;
};

std::string colorize(const std::string&, const std::vector<Match>&, const std::unordered_map<std::string, Format>&);

struct Anchor {
	std::string name;
	long long pos;
};

class Lexem {
public:
	std::vector<Match> match(std::string);
	std::string display(const std::string&, const std::unordered_map<std::string, Format>&) const;

	std::string spec;
	std::string regex;
	std::regex x;
	std::vector<Anchor> anchors;
};

class Lexer {
public:
	Lexer();
	bool insert(std::string, std::string);
	Lexem& operator[](std::string);
	std::unordered_map<std::string, Lexem> lexems;
};

namespace python {
	Lexer init();
	void add_integers(Lexer&);
	void add_float_numbers(Lexer&);
	void add_strings(Lexer&);
	void add_reserved(Lexer&);
	void add_identifiers(Lexer&);
}
