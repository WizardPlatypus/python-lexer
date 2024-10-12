#include <string>
#include <cstdint>

namespace ansi {
	const static std::string ESC = "\033";
	const static std::string CSI = "\033[";

	enum class mode : uint8_t {
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
		enum class iso : uint8_t {
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

		enum class bold : uint8_t {
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
		enum class iso : uint8_t {
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

		enum class bold : uint8_t {
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

std::string format(std::string text, int fg = (int)ansi::fg::iso::DEFAULT, int bg = (int)ansi::bg::iso::DEFAULT, int mode = (int)ansi::mode::BOLD);
void display();
