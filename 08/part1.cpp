#include <algorithm>
#include <charconv>
#include <iostream>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
namespace ranges = std::ranges;

class BootVm {
public:
	enum class Opcode {
		NOP = 0,
		ACC = 1,
		JMP = 2
	};

	struct Instruction {
		Opcode		op;
		int64_t		arg;
	};

public:
	BootVm() = default;
	BootVm(const BootVm &) = delete;

	bool assemble_instruction(const std::string &src);
	void execute_instruction();

	size_t instruction_count() const {return program.size();}
	int64_t program_counter() const {return pc;}
	int64_t accumulator() const {return acc;}

private :
	int64_t		pc = 0;
	int64_t		acc = 0;
	bool		invalid = false;

	vector<Instruction>		program;
};

bool BootVm::assemble_instruction(const std::string &src) {

	auto split = src.find_first_of(' ');
	auto op_str = src.substr(0, split);
	auto arg_str = src.substr(split + 1);

	Instruction instr;

	if (op_str == "nop") {
		instr.op = Opcode::NOP;
	} else if (op_str == "acc") {
		instr.op = Opcode::ACC;
	} else if (op_str == "jmp") {
		instr.op = Opcode::JMP;
	} else {
		cerr << "Vm: Invalid instruction (" << op_str << ") at line " << program.size() + 1 << "\n";
		return false;
	}

	instr.arg = stoi(arg_str);

	program.emplace_back(instr);
	return true;
}

void BootVm::execute_instruction() {

	if (invalid) {
		return;
	}

	if (pc < 0 || pc >= program.size()) {
		cerr << "Vm: program counter out of bounds\n";
		invalid = true;
		return;
	}

	switch (program[pc].op) {
		case Opcode::NOP :
			cout << pc << ": execute NOP\n";
			break;
		case Opcode::ACC :
			cout << pc << ": execute ACC with " << program[pc].arg << "(acc=" << acc << ")\n";
			acc += program[pc].arg;
			break;
		case Opcode::JMP :
			cout << pc << ": execute JMP with " << program[pc].arg << "(acc=" << acc << ")\n";
			pc += program[pc].arg - 1;
			break;
	}

	pc += 1;
}


int main() {
	string line;

	// initialize virtual machine
	BootVm vm;
	while (getline(cin, line)) {
		vm.assemble_instruction(line);
	}

	// execute until an opcode is executed twice
	vector<bool> executed(vm.instruction_count(), false);

	while (!executed[vm.program_counter()]) {
		executed[vm.program_counter()] = true;
		vm.execute_instruction();
	}

	cout << vm.accumulator() << endl;

	return 0;
}
