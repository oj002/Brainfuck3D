#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>

/*
Syntax Guide:
	Brainfuck:
	>	increment the data pointer (to point to the next cell to the right).
	<	decrement the data pointer (to point to the next cell to the left).
	+	increment (increase by one) the byte at the data pointer.
	-	decrement (decrease by one) the byte at the data pointer.
	.	output the byte at the data pointer.
	,	accept one byte of input, storing its value in the byte at the data pointer.
	[	if the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
	]	if the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.

	New:
	^	increment the data pointer (backward).
	v	decrement the data pointer (forward).
	o	increment the data pointer (up).
	x	decrement the data pointer (down).
	@	return the data pointer to the origin.
	;	comments code until new line or another ';'
	#	clear the current cell to 0
	##	clear everything // ToDo:
*/

std::vector<std::vector<std::vector<char>>> memory(1, std::vector<std::vector<char>>(1, std::vector<char>(100, 0))); // TODO: dqueue
std::array<std::int64_t, 3> dataIndex{ 0, 0, 0 };
char *code;

int main(int argc, char** argv)
{
	atexit([]() {system("pause"); });
	std::string path;

	std::ios::sync_with_stdio(false);
	if (argc > 1)
	{
		path = argv[1];
	}
	else
	{
		std::getline(std::cin, path);
	}
	std::ifstream fin(path);
	if (!fin.good())
	{
		std::cout << "file was corrupted\n";
		return -1;
	}
	std::string codestr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	code = codestr.data();
	fin.close();

	std::size_t codeSize{ codestr.length() };

	while (*code)
	{
		switch (*code)
		{
		case '>':
			++dataIndex[2];
			if (memory[dataIndex[0]][dataIndex[1]].size() <= dataIndex[2])
			{
				memory[dataIndex[0]][dataIndex[1]].insert(memory[dataIndex[0]][dataIndex[1]].end(), 25, 0);
			}
			break;
		case '<':
			--dataIndex[2];
			if (0 >= dataIndex[2])
			{
				memory[dataIndex[0]][dataIndex[1]].insert(memory[dataIndex[0]][dataIndex[1]].begin(), 25, 0);
				dataIndex[2] += 25;
			}
			break;
		case '+': ++memory[dataIndex[0]][dataIndex[1]][dataIndex[2]]; break;
		case '-': --memory[dataIndex[0]][dataIndex[1]][dataIndex[2]]; break;
		case '.': std::putc(memory[dataIndex[0]][dataIndex[1]][dataIndex[2]], stdout); break;
		case ',': memory[dataIndex[0]][dataIndex[1]][dataIndex[2]] = std::getc(stdout); break;
		case '[':
		{
			if (memory[dataIndex[0]][dataIndex[1]][dataIndex[2]] == 0)
			{
				size_t open{ 1 };
				while (*code)
				{
					if (*code == ']' && open == 0) { break; }
					++code;
					if (*code == '[')
					{
						++open;
					}
					else if (*code == ']')
					{
						--open;
					}
				}
			}
		} break;
		case ']':
		{
			if (memory[dataIndex[0]][dataIndex[1]][dataIndex[2]] != 0)
			{
				size_t open{ 1 };
				while (*code)
				{
					if (*code == '[' && open == 0) { break; }
					--code;
					if (*code == '[')
					{
						--open;
					}
					else if (*code == ']')
					{
						++open;
					}
				}
			}
		} break;

		case '^':
			++dataIndex[1];
			if (memory[dataIndex[0]][dataIndex[1]].size() <= dataIndex[1])
			{
				memory[dataIndex[0]].insert(memory[dataIndex[0]].end(), 1, std::vector<char>(1, 0));
			}
			break;
		case 'v':
			--dataIndex[1];
			if (0 >= dataIndex[1])
			{
				memory[dataIndex[0]].insert(memory[dataIndex[0]].begin(), 1, std::vector<char>(1, 0));
				dataIndex[1] += 1;
			}
			break;
		case 'o': //TODO: fix 'o' and 'x'
			++dataIndex[0];
			if (memory.size() <= dataIndex[0])
			{
				memory.insert(memory.end(), 1, std::vector<std::vector<char>>(1, std::vector<char>(1, 0)));
			}
			break;
		case 'x':
			--dataIndex[0];
			if (0 >= dataIndex[0])
			{
				memory.insert(memory.begin(), 1, std::vector<std::vector<char>>(1, std::vector<char>(1, 0)));
				dataIndex[0] += 1;
			}
			break;
		case '@': dataIndex = { 0,0,0 }; break;
		case ';':
			while (*code && *code != '\n' && *code != ';') { ++code; }
			break;
		case '#': memory[dataIndex[0]][dataIndex[1]][dataIndex[2]] = 0; break;
		default: 
			break;
		}
		++code;
	}
	
	return 0;
}