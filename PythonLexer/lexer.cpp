#include "lexer.hpp"
#include <iostream>

Lexer::Lexer() {
	std::unordered_map<std::string, Lexem> empty;
	this->lexems = empty;
}

bool Lexer::insert(std::string name, std::string spec) {
	std::string regex = spec;
	std::vector<Anchor> anchors;

	std::regex ref("<[a-zA-Z0-9_]+>");
	std::smatch match;
	int pos = 0;
	while (true) {
		if (!std::regex_search(regex, match, ref)) {
			break;
		}

		std::string label = match.str();
		if (!this->lexems.contains(label)) {
			return false;
		}

		pos = match.position();

		std::string target = this->lexems[label].regex;
		regex.replace(pos, label.size(), target);

		int curls = 0;
		for (int i = 0; i < pos + 1; i++) {
			if (regex[i] != '(') {
				continue;
			}

			bool escaped = false;
			for (int j = 1; 0 < i - j; j++) {
				if (regex[i - j] != '\\') {
					break;
				}
				escaped = !escaped;
			}

			if (escaped) {
				continue;
			}

			curls += 1;
		}

		anchors.push_back(Anchor{ label, curls });

		pos += target.size();
	}

	std::regex x(regex);
	this->lexems[name] = Lexem { spec, regex, x, anchors };

	return true;
}

Lexem& Lexer::operator[](std::string name) {
	return this->lexems[name];
}

void add_integers(Lexer& l) {
    /* Integer literals
	nonzerodigit ::=  "1"..."9"
	digit        ::=  "0"..."9"
	decinteger   ::=  nonzerodigit (["_"] digit)* | "0"+ (["_"] "0")*

	bindigit     ::=  "0" | "1"
	bininteger   ::=  "0" ("b" | "B") (["_"] bindigit)+

	octdigit     ::=  "0"..."7"
	octinteger   ::=  "0" ("o" | "O") (["_"] octdigit)+

	hexdigit     ::=  digit | "a"..."f" | "A"..."F"
	hexinteger   ::=  "0" ("x" | "X") (["_"] hexdigit)+

    integer      ::=  decinteger | bininteger | octinteger | hexinteger
    */
    l.insert("<non_zero_digit>", "([1-9])");
    l.insert("<digit>", "([0-9])");
    l.insert("<dec_integer>", "(<non_zero_digit>(_?<digit>)*|0+(_?0)*)");

    l.insert("<bin_digit>", "([01])");
    l.insert("<bin_integer>", "(0[bB](_?<bin_digit>)+)");

    l.insert("<oct_digit>", "([0-7])");
    l.insert("<oct_integer>", "(0[oO](_?<oct_digit>)+)");

    l.insert("<hex_digit>", "([0-9a-zA-Z])");
    l.insert("<hex_integer>", "(0[xX](_?<hex_digit>)+)");

    l.insert("<integer>", "(<bin_integer>|<oct_integer>|<hex_integer>|<dec_integer>)");
}

void add_float_numbers(Lexer& l) {
    /* Floating-point literals
	digitpart     ::=  digit (["_"] digit)*

	fraction      ::=  "." digitpart
	exponent      ::=  ("e" | "E") ["+" | "-"] digitpart

	pointfloat    ::=  [digitpart] fraction | digitpart "."
	exponentfloat ::=  (digitpart | pointfloat) exponent

    floatnumber   ::=  pointfloat | exponentfloat
    imagnumber ::=  (floatnumber | digitpart) ("j" | "J")
    */
	l.insert("<digit_part>", "(<digit>(_?<digit>)*)");

	l.insert("<fraction>", "(\\.<digit_part>)");
	l.insert("<exponent>", "([eE](\\+|-)?<digit_part>)");

	l.insert("<point_float>", "(<digit_part>?<fraction>|<digit_part>\\.)");
	l.insert("<exponent_float>", "((<digit_part>|<point_float>)<exponent>)");

	l.insert("<float_number>", "(<exponent_float>|<point_float>)");
	l.insert("<imag_number>", "((<float_number>|<digit_part>)[jJ])");
}

void add_strings(Lexer& l) {
    /* String literals
	stringprefix    ::=  "r" | "u" | "R" | "U" | "f" | "F"
						 | "fr" | "Fr" | "fR" | "FR" | "rf" | "rF" | "Rf" | "RF"
	stringescapeseq ::=  "\" <any source character>

	shortstringchar ::=  <any source character except "\" or newline or the quote>
	shortstringitem ::=  shortstringchar | stringescapeseq
	shortstring     ::=  "'" shortstringitem* "'" | '"' shortstringitem* '"'

	longstringchar  ::=  <any source character except "\">
	longstringitem  ::=  longstringchar | stringescapeseq
	longstring      ::=  "'''" longstringitem* "'''" | '"""' longstringitem* '"""'

    stringliteral   ::=  [stringprefix](shortstring | longstring)
    */
	l.insert("<string_prefix>", "(r|u|R|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF)");
	l.insert("<string_escape_seq>", "(\\\\.)");

	l.insert("<short_string_char>", R"(([^\\\n\r"]))");
	l.insert("<short_string_item>", "(<short_string_char>|<string_escape_seq>)");
	l.insert("<short_string>", "('<short_string_item>*'|\"<short_string_item>*\")");

	l.insert("<long_string_char>", R"(([^\\]))");
	l.insert("<long_string_item>", "(<long_string_char>|<string_escape_seq>)");
	l.insert("<long_string>", "('''<long_string_item>*'''|\"\"\"<long_string_item>*\"\"\")");

	l.insert("<string_literal>", "(<string_prefix>?(<short_string>|<long_string>))");

    /* Byte strings
	bytesprefix    ::=  "b" | "B" | "br" | "Br" | "bR" | "BR" | "rb" | "rB" | "Rb" | "RB"
	bytesescapeseq ::=  "\" <any ASCII character>

	shortbyteschar ::=  <any ASCII character except "\" or newline or the quote>
	shortbytesitem ::=  shortbyteschar | bytesescapeseq
	shortbytes     ::=  "'" shortbytesitem* "'" | '"' shortbytesitem* '"'

	longbyteschar  ::=  <any ASCII character except "\">
	longbytesitem  ::=  longbyteschar | bytesescapeseq
	longbytes      ::=  "'''" longbytesitem* "'''" | '"""' longbytesitem* '"""'

    bytesliteral   ::=  bytesprefix(shortbytes | longbytes)
    */
	l.insert("<bytes_prefix>", "(b|B|br|Br|bR|BR|rb|rB|Rb|RB)");
	l.insert("<bytes_escape_seq>", "(\\\\.)");

	l.insert("<short_bytes_char>", R"(([^\\\n\r"]))");
	l.insert("<short_bytes_item>", "(<short_bytes_char>|<bytes_escape_seq>)");
	l.insert("<short_bytes>", "('<short_bytes_item>*'|\"<short_bytes_item>*\")");

	l.insert("<long_bytes_char>", R"(([^\\]))");
	l.insert("<long_bytes_item>", "(<long_bytes_char>|<bytes_escape_seq>)");
	l.insert("<long_bytes>", "('''<long_bytes_item>*'''|\"\"\"<long_bytes_item>*\"\"\")");

	l.insert("<bytes_literal>", "(<bytes_prefix>?(<short_bytes>|<long_bytes>))");
}

void add_reserved(Lexer& l) {
	l.insert("<keywords>", "(False|await|else|import|pass|None|break|except|in|raise|True|class|finally|is|return|and|continue|for|lambda|try|as|def|from|nonlocal|while|assert|del|global|not|with|async|elif|if|or|yield)");
	l.insert("<operators>", R"((\+|-|\*|\*\*|/|//|%|@|<<|>>|&|\||\^|~|:=|<|>|<=|>=|==|!=))");
	l.insert("<delimeters>", R"((\(|\)|\[|\]|\{|\}|,|:|!|\.|;|@|=|->|\+=|-=|\*=|/=|//=|%=|@=|&=|\|=|\^=|>>=|<<=|\*\*=))");
	l.insert("<forbidden>", R"((\$|\?))");
	l.insert("<comment>", "(#.*)");
}

void add_identifiers(Lexer& l) {
	l.insert("<id_start>", "[a-zA-Z_]");
	l.insert("<id_continue>", "[0-9a-zA-z_]");
	l.insert("<identifier>", "<id_start><id_continue>*");
}

