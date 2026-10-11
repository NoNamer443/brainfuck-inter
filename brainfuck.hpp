#ifndef BRAINFUCK__BRAINFUCK_HPP_
#define BRAINFUCK__BRAINFUCK_HPP_ 1

#include <string> // std::string
#include <iostream> // std::cin, std::cout
#include <vector> // std::vector
#include <cstddef> // size_t
#include <unordered_map> // std::unordered_map
#include <stdexcept> // std::runtime_error, std::out_of_range
#include <utility> // std::pair


namespace bf {

	class BrainfuckSyntaxError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};


	class Brainfuck {
	private:
		std::vector<unsigned char> list;
		std::string buffer;
		size_t point;
		constexpr static size_t SIZE = 30000;
		
		static void check(const std::string &s) {
			size_t p = 0, row = 1, column = 0;
			std::vector<std::pair<size_t, size_t>> list;
			for (char c : s) {
				switch (c) {
				case '\n':
					++row;
					column = 0;
					break;
				default:
					++column;
				}

				switch (c) {
				case '[':
					++p;
					list.push_back({row, column});
					break;
				case ']':
					if (!p) {
						throw BrainfuckSyntaxError("Unmatched ']' in row " + std::to_string(row) + ", column " + std::to_string(column) + " (unit: bytes)");
					} else {
						--p;
						list.pop_back();
					}
					break;
				}
			}

			if (p) {
				throw BrainfuckSyntaxError("Unmatched '[' in row " + std::to_string(list.back().first) + ", column " + std::to_string(list.back().second) + " (unit: bytes)");
			}
		} // static void check

		std::string filter(const std::string& s) {
			std::string value;
			for (const char &c : s) {
				switch (c) {
				case ('<'):
				case ('>'):
				case ('+'):
				case ('-'):
				case (','):
				case ('.'):
				case ('['):
				case (']'):
					value += c;
					break;
				}
			}
			return value;
		} // std::string filter

		static std::unordered_map<size_t, size_t> match(const std::string& s) {
			std::unordered_map<size_t, size_t> dict;
			std::vector<size_t> list;
			size_t p = 0, temp;

			for (const char &c : s) {

				switch (c) {
				case '[':
					list.push_back(p);
					break;
				case ']':
					temp = list.back();
					list.pop_back();
					dict[p] = temp;
					dict[temp] = p;
					break;
				}

				++p;
			}

			return dict;
		} // static std::unordered_map<size_t, size_t> match
	public:
		Brainfuck() : list(SIZE, 0), point(0) {}

		inline void move_left() {
			point = (point == 0) ? SIZE - 1 : --point;	
		}

		inline void move_right() {
			point = (point == SIZE - 1) ? 0 : ++point;
		}

		inline void add_one() {
			++list[point];
		}

		inline void sub_one() {
			--list[point];
		}
		
		void get_input() {
			if (buffer.empty()) {
				std::getline(std::cin, buffer);
				if (std::cin.eof()) {
					buffer = '\0';
					std::cin.clear();
				} else {
					buffer += '\n';
				}
			}
			list[point] = buffer[0];
			buffer.erase(0, 1);
		}
		
		inline void clear_buffer() {
			buffer.clear();
		}

		inline void output() const {
			// std::cout << static_cast<unsigned int>(list[point]) << std::endl;
			std::cout << list[point];
		}

		void run(const std::string& s) {
			check(s);
			std::string code = filter(s);
			auto dict = match(code);

			size_t p = 0;
			for (char c = code[p]; p < code.size(); ++p) {
				c = code[p];

				switch (c) {
				case '<':
					move_left();
					break;
				case '>':
					move_right();
					break;
				case '+':
					add_one();
					break;
				case '-':
					sub_one();
					break;
				case ',':
					get_input();
					break;
				case '.':
					output();
					break;
				case '[':
					if (!list[point])
						p = dict[p];
					break;
				case ']':
					if (list[point])
						p = dict[p];
					break;
				}
			}

			clear_buffer();
			std::cout << std::flush;
		} // void run

		void reset() {
			std::fill(list.begin(), list.end(), 0);
			point = 0;
		}

		inline unsigned char peek() const {
			return list[point];
		}

		unsigned char peek(size_t index) const {
			if (index >= SIZE) {
				throw std::out_of_range(std::to_string(index) + " >= SIZE");
			}
			return list[index];
		}
	}; // class Brainfuck
} // namespace bf

#endif // BRAINFUCK__BRAINFUCK_HPP_
