#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using TokenKind = enum {
  TK_PUNCT, // Punctuators
  TK_NUM,   // Numeric literal
  TK_EOF,   // End of file
};

class Token {
public:
  TokenKind kind;
  Token *next;
  long long val; // numver val
  char *loc;
  int len;
  std::string s;
  Token();

  Token(TokenKind kind, std::string s) : kind(kind), s(s) {
    if (kind == TK_NUM) {
      val = stoi(s);
    }
  }
};

// for debug
template <class T>
std::ostream &operator<<(std::ostream &os, const Token &tok) {
  switch (tok.kind) {
  case TK_NUM:
    os << "TK_NUM: ";
    break;
  case TK_PUNCT:
    os << "TK_PUNCT: ";
    break;
  case TK_EOF:
    os << "TK_EOF: ";
    break;
  default:
    os << "unknown token: ";
    break;
  }
  return os << '\n';
}

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[";
  bool first = false;
  for (auto d : v) {
    if (first)
      os << ", ";
    first = true;
    os << d;
  }
  return os << "]";
}

std::unique_ptr<std::vector<Token>> tokenize(char *p) {
  std::unique_ptr<std::vector<Token>> toks =
      std::make_unique<std::vector<Token>>();
  while (*p) {
    if (isspace(*p))
      ++p;
    if (*p == '+' || *p == '-') {
      if (*p == '+')
        toks->push_back(Token(TK_PUNCT, "+"));
      if (*p == '-')
        toks->push_back(Token(TK_PUNCT, "-"));
      ++p;
      continue;
    } else { // number
      int val = 0;
      while (isdigit(*p)) {
        val *= 10;
        val += *p - '0';
        ++p;
      }
      toks->push_back(Token(TK_NUM, std::to_string(val)));
      continue;
    }
    break;
  }
  toks->push_back(Token(TK_EOF, ""));
  return toks;
}
