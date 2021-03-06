#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

void warn(char *fmt, ...);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

typedef struct Var Var;
struct Var {
  Var *next;   // Next variable or NULL
  char *name;  // Variable name
  int offset;  // Variable offset from RBP
};

// Kind of node for abstract syntax tree
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LE,      // <=
  //ND_GT,      // > is invert to ND_LT
  //ND_GE,      // >= is invert to ND_LE
  ND_ASSIGN,  // a = 0
  ND_VAR,    // Local value
  ND_NUM,     // Integer
  ND_RETURN,  // Return
  ND_IF,      // If
  ND_WHILE,   // While
  ND_FOR,     // For
  ND_BLOCK,   // Block
  ND_FUNCALL, // Call function
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;  // Type of node
  Node *next;     // Next
  Node *lhs;      // Left-hand side
  Node *rhs;      // Right-hand side
  int val;        // Use only the kind is ND_NUM
  Var *var;       // Use only the kind is ND_VAR
  Node *cond;     // If/While/For condition
  Node *then;     // If/While/For statement
  Node *els;      // Else statement
  Node *init;     // For's init statement
  Node *inc;      // For's increment statement
  Node *body;     // Block statements
  char *funcname; // Name of function
  Node *args;     // Arguments of function
};

typedef struct Function Function;
struct Function {
  Node *node;
  Var *locals;
  int stack_size;
};

Node *primary();
Node *unary();
Node *mul();
Node *add();
Node *relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
Function *program();

void dump_node(Node *node, int depth);

// Kind of token
typedef enum {
  TK_RESERVED, // Symbols
  TK_INDENT,   // Identifier
  TK_NUM,      // Integer
  TK_EOF,      // Token of the end of file
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind; // Type of token
  Token *next;    // Next token
  int val;        // Integer when kind is TK_NUM
  char *str;      // Token string
  int len;        // Token length
};

// Current token
Token *currentToken;

// Input program
char *user_input;

// Fragment of code
// Node *code[100];

void codegen(Function *prog);
void tokenize();
