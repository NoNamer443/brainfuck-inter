#include "brainfuck.hpp"

int main() {
	bf::Brainfuck b;
	b.run("+[>+]");
	std::cout << "done" << std::endl;
	return 0;
}
