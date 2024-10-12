#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

struct Match {
	std::string label;
	long long pos;
	long long len;
};

std::string colorize(const std::string&, std::vector<Match>&);

struct Anchor {
	std::string name;
	long long pos;
};

class Lexem {
public:
	std::string spec;
	std::string regex;
	std::regex x;
	std::vector<Anchor> anchors;
	std::vector<Match> match(std::string);
};

class Lexer {
public:
	Lexer();
	bool insert(std::string, std::string);
	Lexem& operator[](std::string);
private:
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
