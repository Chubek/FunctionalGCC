#include <iostream>
#include <cctype>

class Lexer {
private:
    const char* input;
    size_t position;
    std::unordered_set<std::string> keywords;

public:
    Lexer(const char* input) : input(input), position(0) {
	keywords.insert("structure");
	
    }

    char peek() const {
        return input[position];
    }

    void advance() {
        if (input[position] != '\0') {
            ++position;
        }
    }

    std::string previousLexeme() const {
        return std::string(input, position);
    }

    void skipWhitespace() {
        while (std::isspace(peek())) {
            advance();
        }
    }

    std::string consumeAlphabeticIdentifier() {
        size_t start = position;
        while (std::isalpha(peek()) || peek() == '_' || peek() == '\'') {
            advance();
        }
        return std::string(input + start, position - start);
    }

    std::string consumeSymbolicIdentifier() {
        size_t start = position;
        while (ispunct(peek()) && peek() != '\'' && peek() != '"') {
            advance();
        }
        return std::string(input + start, position - start);
    }


    std::string consumeOperator() {
        size_t start = position;
        while (ispunct(peek()) && peek() != '\'' && peek() != '"' && !std::isalpha(peek())) {
            advance();
        }
        return std::string(input + start, position - start);
    }

    std::string consumeStringLiteral() {
        std::string value;
        advance(); 
        while (peek() != '"' && peek() != '\0') {
            value.push_back(peek());
            advance();
        }
        advance(); 
        return value;
    }

    std::string consumeDigitSequence() {
        size_t start = position;
        while (std::isdigit(peek())) {
            advance();
        }
        return std::string(input + start, position - start);
    }

    std::string consumeFloatingPointLiteral() {
        std::string value = consumeDigitSequence();
        if (peek() == '.') {
            value.push_back('.');
            advance();
            value += consumeDigitSequence();
        }
        if (peek() == 'e' || peek() == 'E') {
            value.push_back(peek());
            advance();
            if (peek() == '+' || peek() == '-') {
                value.push_back(peek());
                advance();
            }
            value += consumeDigitSequence();
        }
        return value;
    }


    char consumeCharacterLiteral() {
        char value = '\0';
        advance(); 
        if (peek() != '\'' && peek() != '\0') {
            if (peek() == '\\') {
                advance(); 
                switch (peek()) {
                    case 'n':
                        value = '\n';
                        break;
                    case 't':
                        value = '\t';
                        break;
                    case '\'':
                        value = '\'';
                        break;
                    case '"':
                        value = '\"';
                        break;
                    case '\\':
                        value = '\\';
                        break;
                    case 'r':
                        value = '\r';
                        break;
                    case 'a':
                        value = '\a';
                        break;
                    case 'b':
                        value = '\b';
                        break;
                    case 'f':
                        value = '\f';
                        break;
                    case 'v':
                        value = '\v';
                        break;
                    case 'x':
                        advance(); 
                        value = lexHexEscape();
                        break;
                    case 'U':
                        advance(); 
                        value = lexUnicodeEscape();
                        break;
                    default:
                        
                        break;
                }
            } else {
                value = peek();
            }
            advance();
        }
        advance(); 
        return value;
    }

    char lexHexEscape() {
        char hexValue = 0;
        while (std::isxdigit(peek())) {
            hexValue = hexValue * 16 + hexDigitValue(peek());
            advance();
        }
        return hexValue;
    }

    char lexUnicodeEscape() {
        char unicodeValue = 0;
        for (int i = 0; i < 8; ++i) {
            if (std::isxdigit(peek())) {
                unicodeValue = unicodeValue * 16 + hexDigitValue(peek());
                advance();
            } else {
                break;
            }
        }
        return unicodeValue;
    }

    int hexDigitValue(char digit) {
        if (std::isdigit(digit)) {
            return digit - '0';
        } else if (std::isxdigit(digit)) {
            return std::tolower(digit) - 'a' + 10;
        }
        return 0;
    }

   std::string consumeKeyword() {
        size_t start = position;
        while (std::isalpha(peek()) || peek() == '_') {
            advance();
        }
        std::string keyword = std::string(input + start, position - start);
        if (keywords.count(keyword)) {
            return keyword;
        }
        return ""; 
    }
};

int main() {
    const char* input = "structure identifier = structure;";
    Lexer lexer(input);

    lexer.skipWhitespace();
    std::cout << "Identifier: " << lexer.consumeIdentifier() << std::endl;

    return 0;
}

