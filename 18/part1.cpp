#include <numeric>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

class Lexer {
public:
	enum class TokenType {
		NUMBER,
		OP_ADD,
		OP_MULTIPLY,
		PARENS_OPEN,
		PARENS_CLOSE,
		LINE_END
	};

	struct Token {
		TokenType	type;
		int			value;
	};

public:
	Lexer(string_view src) : source(src) {
	}

	Token next_token() {

		while (true) {
			if (current >= source.size()) {
				return {TokenType::LINE_END, 0};
			}

			switch (source[current]) {
				case '+' :	return make_token(TokenType::OP_ADD);
				case '*' :	return make_token(TokenType::OP_MULTIPLY);
				case '(' :	return make_token(TokenType::PARENS_OPEN);
				case ')' :	return make_token(TokenType::PARENS_CLOSE);
				case ' ':   current += 1; break;
				case '0': case '1':
				case '2': case '3':
				case '4': case '5':
				case '6': case '7':
				case '8': case '9':
							return make_token(TokenType::NUMBER, source[current] - '0');
				default:
					cerr << "Invalid character (" << source[current] << ") at position " << current << endl;
					return {TokenType::LINE_END, -1};
			}
		}
	}

private:
	Token make_token(TokenType type, int value = 0) {
		current += 1;
		return {type, value};
	}

private:
	string_view		source;
	int				current = 0;
};

class Parser {
public:
	Parser(string_view src) : lexer(src) {
	}

	int64_t process() {
		int64_t result = 0;
		Lexer::TokenType op = Lexer::TokenType::OP_ADD;

		for (auto token = lexer.next_token(); token.type != Lexer::TokenType::LINE_END; token = lexer.next_token()) {
			switch (token.type) {
				case Lexer::TokenType::NUMBER:
					if (op == Lexer::TokenType::OP_ADD) {
						result += token.value;
					} else {
						result *= token.value;
					}
					break;
				case Lexer::TokenType::OP_ADD:
				case Lexer::TokenType::OP_MULTIPLY:
					op = token.type;
					break;
				case Lexer::TokenType::PARENS_OPEN:
					if (op == Lexer::TokenType::OP_ADD) {
						result += process();
					} else {
						result *= process();
					}
					break;
				case Lexer::TokenType::PARENS_CLOSE:
				case Lexer::TokenType::LINE_END:
					return result;
			}
		}

		return result;
	}

private:
	Lexer	lexer;
};

ostream &operator <<(ostream &os, Lexer::Token token) {
	switch (token.type) {
		case Lexer::TokenType::NUMBER:
			cout << "NUMBER(" << token.value << ")";
			break;
		case Lexer::TokenType::OP_ADD:
			cout << "OP_ADD";
			break;
		case Lexer::TokenType::OP_MULTIPLY:
			cout << "OP_MULTIPLY";
			break;
		case Lexer::TokenType::PARENS_OPEN:
			cout << "PARENS_OPEN";
			break;
		case Lexer::TokenType::PARENS_CLOSE:
			cout << "PARENS_CLOSE";
			break;
		case Lexer::TokenType::LINE_END:
			cout << "LINE_END";
			break;
	}
	return os;
}

int main() {

	string line;

	int64_t sum = 0;

	while (getline(cin, line)) {
		Parser parser(line);
		sum += parser.process();
	}

	cout << sum << endl;

	return 0;
}
