#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

//
// token.h
//
enum token_type {T_OP, T_CP, T_MUL, T_DIV, T_ADD, T_SUB, T_IMM};
const char *token_type_str[8] = {"T_OP", "T_CP", "T_MUL", "T_DIV", "T_ADD", "T_SUB", "T_IMM"};

struct token {
	const char			*token;
	enum token_type type;
	int							len;
	bool						negate;
};

double token_value(struct token *t);

//
// ast.h
//
enum ast_op {A_MUL, A_DIV, A_ADD, A_SUB, A_IMM};
const char *ast_op_str[5] = {"A_MUL", "A_DIV", "A_ADD", "A_SUB", "A_IMM"};

struct ast_node;
struct ast_node {
	enum ast_op op;
	bool negate;
	union {
		struct ast_node *children[2];
		double val;
	} v;
};

struct ast_node *parse_value();
struct ast_node *parse_term();
struct ast_node *parse_expr();

// ast.c
static struct token *parse_tokens;

struct ast_node *node_imm(double v) {
	struct ast_node *n = malloc(sizeof(struct ast_node));
	n->op = A_IMM;
	n->v.val = v;
	n->negate = false;
	return n;
}

struct ast_node *node_bin(enum ast_op op, struct ast_node *a, struct ast_node *b) {
	struct ast_node *n = malloc(sizeof(struct ast_node));
	n->op = op;
	n->v.children[0] = a;
	n->v.children[1] = b;
	n->negate = false;
	return n;
}

/*
 * value::= number
 *        | '(' expression ')
 */
struct ast_node *parse_value() {
	struct ast_node *n = NULL;

	if (parse_tokens->type == T_IMM) {
		n = node_imm(token_value(parse_tokens));
		if (parse_tokens->negate) {
			n->negate = true;
		}
		parse_tokens++;
	} else if (parse_tokens->type == T_OP) {
		bool negate = parse_tokens->negate;
		parse_tokens++;
		n = parse_expr();
		if (negate) {
			n->negate = !n->negate;
		}
		parse_tokens++;
	}

	return n;
}

/*
 * term ::= value
 *        | term '*' value
 *        | term '\' value
 */
struct ast_node *parse_term() {
	struct ast_node *n = parse_value();
	struct ast_node *t;

	while (parse_tokens->type == T_MUL || parse_tokens->type == T_DIV) {
		if (parse_tokens->type == T_MUL) {
			parse_tokens++;
			t = parse_value();
			n = node_bin(A_MUL, n, t);
		} else if (parse_tokens->type == T_DIV) {
			parse_tokens++;
			t = parse_value();
			n = node_bin(A_DIV, n, t);
		} else {
			printf("ERROR: parse_term\n");
			exit(-1);
		}
	}

	return n;
}

/*
 * expr ::= term
 *        | expr '+' term
 *        | expr '-' term
 */
struct ast_node *parse_expr() {
	struct ast_node *t = parse_term();
	struct ast_node *e;

	while (parse_tokens->type == T_ADD || parse_tokens->type == T_SUB) {
		if (parse_tokens->type == T_ADD) {
			parse_tokens++;
			e = parse_term();
			t = node_bin(A_ADD, t, e);
		} else if (parse_tokens->type == T_SUB) {
			parse_tokens++;
			e = parse_term();
			t = node_bin(A_SUB, t, e);
		} else {
			printf("ERROR: parse_expr\n");
			exit(-1);
		}
	}
	return t;
}

struct ast_node *parse(struct token *tokens) {
	parse_tokens = tokens;
	return parse_expr();
}

void print_ast(struct ast_node *ast) {
	static int level = -1;
	level++;
	switch(ast->op) {
		case A_ADD:
		case A_SUB:
		case A_MUL:
		case A_DIV:
			printf("%*sBin (%s, \n", level, "", ast_op_str[ast->op]);
			print_ast(ast->v.children[0]);
			print_ast(ast->v.children[1]);
			printf("%*s) (%d)\n", level, "", ast->negate);
			break;
		case A_IMM:
			printf("%*sImm (%f) (%d)\n", level, "", ast->v.val, ast->negate);
			break;
	}
	level--;
}

//
// token.c
//
double token_value(struct token *t) {
	char *tmp = NULL;
	double ret = 0.0;

	if (t->type != T_IMM) {
		return ret;
	}

	tmp = strndup(t->token, t->len);
	ret = atof(tmp);
	free(tmp);

	return ret;
}

struct token tokens[1024];

#define SINGLE_CHAR(c, t)															\
	if (*expression == (c)) {														\
		tokens[tokenctr].token = expression;							\
		tokens[tokenctr].type = (t);											\
		tokens[tokenctr].len = tlen;											\
		goto next;																				\
	}

void tokenize(const char *expression) {
	int tlen, tokenctr = 0;
	bool negate = false;

	memset(tokens, 0, sizeof(struct token) * 1024);

	while (*expression != '\0') {
		while (isspace(*expression)) {
			expression++;
		}

		tlen = 1;

		if (isdigit(*expression)) {
			tokens[tokenctr].token = expression;
			tokens[tokenctr].type = T_IMM;
			while (isdigit(*(expression + 1))) {
				expression++;
				tlen++;
			}

			if (*(expression + 1) == '.') {
				expression++;
				tlen++;
				while (isdigit(*(expression + 1))) {
					expression++;
					tlen++;
				}
			}
			tokens[tokenctr].len = tlen;
			goto next;
		}

		SINGLE_CHAR('(', T_OP);
		SINGLE_CHAR(')', T_CP);
		SINGLE_CHAR('*', T_MUL);
		SINGLE_CHAR('/', T_DIV);
		SINGLE_CHAR('+', T_ADD);

		if (*expression == '-') {
			if (tokenctr == 0 ||
					tokens[tokenctr - 1].type == T_ADD ||
					tokens[tokenctr - 1].type == T_SUB ||
					tokens[tokenctr - 1].type == T_MUL ||
					tokens[tokenctr - 1].type == T_DIV ||
					tokens[tokenctr - 1].type == T_OP) {
				negate = true;
				expression++;
				continue;
			}
			tokens[tokenctr].token = expression;
			tokens[tokenctr].type = T_SUB;
			tokens[tokenctr].len = tlen;
			goto next;
		}

next:
		if (negate) {
			tokens[tokenctr].negate = true;
			negate = false;
		}
		tokenctr++;
		expression++;
	}
}

//
// main.c
//

struct ast_node *process(struct ast_node *ast) {
	if (ast->op == A_IMM) {
		if (ast->negate) {
			ast->v.val *= -1.0;
		}
		return ast;
	}

	struct ast_node *l = ast->v.children[0];
	struct ast_node *r = ast->v.children[1];
	struct ast_node *n = NULL;

	if (l->op != A_IMM) {
		l = process(l);
	}

	if (r->op != A_IMM) {
		r = process(r);
	}

	if (l->op == A_IMM && r->op == A_IMM) {
		double lval = l->negate ? -1.0 * l->v.val : l->v.val;
		double rval = r->negate ? -1.0 * r->v.val : r->v.val;

		switch (ast->op) {
			case A_ADD:
				n = node_imm(lval + rval);
				break;
			case A_SUB:
				n = node_imm(lval - rval);
				break;
			case A_MUL:
				n = node_imm(lval * rval);
				break;
			case A_DIV:
				n = node_imm(lval / rval);
				break;
			default:
				exit(0);
		}
		if (ast->negate) {
			n->v.val *= -1.0;
		}
		return n;
	}

	return ast;
}

double solve (struct ast_node *ast) {
	struct ast_node *s = process(ast);
	return s->v.val;
}

double calculate(const char *expression) {
	tokenize(expression);
	struct ast_node *tree = parse(tokens);

	for (int i = 0; tokens[i].token != NULL; i++) {
		printf("T: %.*s (%s) (%d)\n", tokens[i].len, tokens[i].token, token_type_str[tokens[i].type], tokens[i].negate);
	}

	print_ast(tree);
	return solve(tree);
}

int main() {
	char *expr = "9.95/7.63*8.20*9.12+9.71*7.03*8.50/ -9.60+6.52*9.22+ -5.72- -8.78* -8.67*7.68* -3.85* -3.17/ -3.75/9.21+7.41/7.63/8.30/7.33-((7.26-9.93*7.96-5.32*8.79* -0.04+6.58- -3.90+8.16/7.37+8.75-6.26)/(7.81)+6.40+7.22/6.07- -5.70/ -6.67+8.78/7.22/7.57)/9.23+7.39-9.43*8.78+5.69+9.55*8.87/9.90+7.96-7.12*9.97/7.36+8.76+8.65/(7.03+ -2.14+ -3.09- -2.21* -1.95/9.89-6.66)/7.41*5.37* -8.05+7.19-9.40+(7.17)+(7.69/9.29- -7.27/8.23-8.70-6.15/ -7.88*8.73/6.39/6.14/ -0.60- -1.81* -0.87+7.42/7.66+ -3.63+5.60+6.96* -9.78*5.25-9.58+6.71+5.73*(8.06))+((9.07+5.12+9.51+ -0.06/5.60/ -3.69- -2.41+(8.81)/( -1.19/7.89+8.06/6.33)/(6.41* -9.95/ -2.00-9.18+ -8.60- -7.77+8.10)-7.08+9.55+ -2.03+ -1.13-8.57/6.46/7.49* -4.36/6.53+7.09+8.77/9.43+9.78/8.61))";
	double out = calculate(expr);
	printf("Solution: %.18g\n", out);
	return 0;
}

