#include "sml-lexer.h"

Token::Token(const string &lexeme, TokenKind kind)
{
    tokenValue = lexeme;
    tokenKind = kind;
}

TokenKind Token::getKind(void)
{
    return tokenKind;
}

Lexeme Token::getValue(void)
{
    return tokenValue;
}

Lexer::Lexer(ifstream &inputFile)
{
    if (!inputFile.is_open())
    {
        // TODO: Add IO error handling
    }

    currentText = string((istreambuf_iterator<char>(inputFile)),
                         istreambuf_iterator<char>());

    currentCursor = currentText;
    currentPosition = 0;
}

Lexer::Lexer(string &inputString)
{
    currentText = inputString;
    currentCursor = currentText;
    currentPosition = 0;
}

Lexer::~Lexer(void) {}

void Lexer::skipLexeme(Lexeme &lexeme)
{
    skipWhiteSpace();
    size_t lexemeNext = currentCursor.find(lexeme);
    size_t lexemeLength = lexeme.length();
    if (lexemeNext == 0)
    {
        currentPosition += lexemeLength;
        currentCursor = currentCursor.substr(lexemeLength);
    }
}

void Lexer::skipWhiteSpace(void)
{
    size_t i = 0;
    while (i < currentCursor.length() && isblank(currentCursor[i]))
    {
        i++;
    }
    currentPosition += i;
    currentCursor = currentCursor.substr(i);
}

Lexeme Lexer::peekNextLexeme(void)
{
    skipWhiteSpace();
    size_t i = 0;
    while (i < currentCursor.length() && !isblank(currentCursor[i]))
    {
        i++;
    }
    return currentCursor.substr(0, i);
}

Lexeme Lexer::peekPreviousLexeme(void)
{
    if (!history.empty())
    {
        return history.back();
    }
    return ""; // Add appropriate handling if history is empty
}

char Lexer::peekNextChar(void)
{
    if (currentPosition + 1 < currentCursor.length())
    {
        return currentCursor[currentPosition + 1];
    }
    return '\0'; // Return null character if end of cursor is reached
}

char Lexer::peekPreviousChar(void)
{
    if (currentPosition > 0)
    {
        return currentText[currentPosition - 1];
    }
    return '\0'; // Return null character if beginning of text is reached
}

Token
Lexer::consumeAndReturnIdentifier (void)
{
     skipWhiteSpace();
     Lexeme nextLexeme = peekNextLexeme();

     for (char currentChar : nextLexeme) {
	if (!isalnum(currentChar) || currentChar != '_') {
	   // TODO: Handle lexical error
	}
     }
     
     skipLexeme(nextLexeme);
     return Token(nextLexeme, TOKEN_IDENTIFIER);
}

Token
Lexer::consumeAndReturnKeyword (Lexeme &keyword)
{
    skipWhiteSpace();
    Lexeme nextLexeme = peekNextLexeme();

    // TODO: Implement keyword matching
}

Token
Lexer::consumeAndReturnOperator (void)
{
     skipWhiteSpace();
     char nextChar = peekNextChar();

     switch (nextChar) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	if (currentCursor[1] == ' ') {
	   // TODO: Handle lexical error
	}
	string lexeme(1, nextChar);
	skipLexeme(lexeme);
	return Token(lexeme, TOKEN_OPERATOR);

	case '&':
	case '|':
	if (currentCursor[1] == ' ') {
	   string lexeme(1, nextChar);
	   skipLexeme(lexeme);
	   return Token(lexeme, TOKEN_OPERATOR);
	} else if (currentCursor[1] == nextChar) {
	   string lexeme(2, nextChar);
	   skipLexeme(lexeme);
	   return Token(lexeme, TOKEN_OPERATOR_LITERAL);
	}

     }
}

Token
Lexer::consumeAndReturnIntegerLiteral (void)
{
    skipWhiteSpace();
    Lexeme nextLexeme = peekNextLexeme();

    for (char currentChar : nextLexeme) {
	if (!isdigit(currentChar) || !isxdigit(currentChar)) {
	    // TODO: Handle lexical error
	}
    }

    skipLexeme(nextLexeme);
    return Token(nextLexeme, TOKEN_INTEGER_LITERAL);
}

Token
Lexer::consumeAndReturnFloatLiteral (void)
{
    skipWhiteSpace();
    Lexeme nextLexeme = peekNextLexeme();

    for (char currentChar : nextLexeme) {
	if (!isdigit(currentChar) 
			|| !(currentChar == 'e' 
				|| currentChar == '.' 
				|| currentChar == 'E')) {
		// TODO: Handle lexical error
	}
    }

    skipLexeme(nextLexeme);
    return Token(nextLexeme, TOKEN_FLOAT_LITERAL);
}

Token
Lexer::consumeAndReturnStringLiteral (void)
{
    skipWhiteSpace();
    Lexeme currentLexeme = peekNextLexeme();

    if (currentLexeme[0] != '"') {
	// TODO: Handle lexical error
    }

    skipLexeme(currentLexeme);
    return Token(parseString(currentLexeme), TOKEN_STRING_LITERAL);    
}

Token
Lexer::consumeAndReturnCharLiteral (void)
{
   skipWhiteSpace();
   Lexeme currentLexeme = peekNextLexeme();

   if (currentLexeme[0] != '\'' || currentLexeme[2] != '\'') {
	// TODO: Handle lexical error
   }

   string lexeme(1, currentLexeme[1]);
   skipLexeme(currentLexeme);
   return Token(lexeme, TOKEN_CHAR_LITERAL);

}
