#ifndef SML_LEXER_H
#define SML_LEXER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

typedef string Lexeme;

enum TokenKind
{
  TOKEN_INTEGER_LITERAL,
  TOKEN_FLOAT_LITERAL,
  TOKEN_STRING_LITERAL,
  TOKEN_CHAR_LITERAL,
  TOKEN_IDENTIFIER,
  TOKEN_OPERATOR,
  TOKEN_KEYWORD,
};

class Token
{
public:
  Token (const string &lexeme, TokenKind kind);

  TokenKind getKind (void);
  Lexeme getValue (void);

private:
  TokenKind tokenKind;
  Lexeme tokenValue;
};

class Lexer
{
public:
  Lexer (ifstream &inputFile);
  Lexer (string &inputString);
  ~Lexer (void);

  void skipLexeme (Lexeme &lexeme);
  void skipWhiteSpace (void);
  Lexeme peekNextLexeme (void);
  Lexeme peekPreviousLexeme (void);
  char peekNextChar (void);
  char peekPreviousChar (void);
  Token consumeAndReturnIdentifier (void);
  Token consumeAndReturnKeyword (Lexeme &keyword);
  Token consumeAndReturnOperator (void);
  Token consumeAndReturnIntegerLiteral (void);
  Token consumeAndReturnFloatLiteral (void);
  Token consumeAndReturnStringLiteral (void);
  Token consumeAndReturnCharLiteral (void);

private:
  string currentText;
  string currentCursor;
  int currentPosition;
  vec<Lexeme> history;
};

#endif
