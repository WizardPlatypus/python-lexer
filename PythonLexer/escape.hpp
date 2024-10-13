#pragma once
#include <string>

namespace ansi {
	void display();

	const static std::string ESC = "\033";
	const static std::string CSI = "\033[";

	enum class mode : char {
		RESET = 0,
		BOLD = 1,
		DIM = 2,
		ITALIC = 3,
		UNDERLINE = 4,
		BLINKING = 5,
		REVERSE = 7,
		HIDE = 8,
		STRIKETHROUGH = 9,
	};

	namespace fg {
		enum class iso : char {
			BLACK = 30,
			RED = 31,
			GREEN = 32,
			YELLOW = 33,
			BLUE = 34,
			MAGENTA = 35,
			CYAN = 36,
			WHITE = 37,
			DEFAULT = 39,
		};

		enum class bold : char {
			BLACK = 90,
			RED = 91,
			GREEN = 92,
			YELLOW = 93,
			BLUE = 94,
			MAGENTA = 95,
			CYAN = 96,
			WHITE = 97,
		};
	}

	namespace bg {
		enum class iso : char {
			BLACK = 40,
			RED = 41,
			GREEN = 42,
			YELLOW = 43,
			BLUE = 44,
			MAGENTA = 45,
			CYAN = 46,
			WHITE = 47,
			DEFAULT = 49,
		};

		enum class bold : char {
			BLACK = 100,
			RED = 101,
			GREEN = 102,
			YELLOW = 103,
			BLUE = 104,
			MAGENTA = 105,
			CYAN = 106,
			WHITE = 107,
		};
	}
}

struct Format {
	Format(int fg = (int)ansi::fg::iso::DEFAULT, int bg = (int)ansi::bg::iso::DEFAULT, int mode = (int)ansi::mode::BOLD);
	std::string format(const std::string&) const;

	int fg;
	int bg;
	int mode;
};

