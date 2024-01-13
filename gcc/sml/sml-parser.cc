#include <iostream>
#include <string>
#include <vector>

class Token
{
public:
  enum Type
  {
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL,
    
  };

  Token (Type type, const std::string &lexeme) : type (type), lexeme (lexeme)
  {
  }

  Type
  getType () const
  {
    return type;
  }
  const std::string &
  getLexeme () const
  {
    return lexeme;
  }

private:
  Type type;
  std::string lexeme;
};

class Lexer
{
public:
  Lexer (const std::string &input) : input (input), position (0) {}

  Token
  getNextToken ()
  {
    
    
    
  }

private:
  std::string input;
  size_t position;
};

class Parser
{
public:
  Parser (const std::string &input) : lexer (input) {}

  void
  parse ()
  {
    
    
    parseApplication ();
  }

private:
  Lexer lexer;

  

  void
  parseApplication ()
  {
    parseExpression ();
    while (lexer.getNextToken ().getType () == Token::IDENTIFIER)
      {
        parseExpression ();
      }
  }

  void
  parseExpression ()
  {
    
  }

  
};

int
main ()
{
  std::string input = "your_input_program_here";
  Parser parser (input);
  parser.parse ();

  return 0;
}
