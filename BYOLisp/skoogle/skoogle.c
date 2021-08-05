#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpc/mpc.h"

#ifdef _WIN32
#define BUFF_SIZE
static char buffer[BUFF_SIZE];

char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, BUFF_SIZE, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

void add_history(char* unused) {}

/* Fake add_history function */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

#define LASSERT(args, cond, err) \
    if (!(cond)) { lval_del(args); return lval_err(err); }



struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

typedef lval*(*lbuiltin)(lenv*, lval*);

struct lval {
    int type;
    long num;
    char* err;
    char* sym;
    lbuiltin fun;
    int count;
    lval** cell;
};

struct lenv {
    int count;
    char** syms;
    lval** vals;
};


lval eval(mpc_ast_t*);
lval eval_op(lval, const char*, lval);
lval* lval_num(long);
lval* lval_err(char*);
lval* lval_sym(char*);
lval* lval_sexpr(void);
lval* lval_fun(lbuiltin func);
lval* lval_read_num(mpc_ast_t*);
lval* lval_read(mpc_ast_t*);
lval* lval_add(lval*, lval*);
lval* lval_copy(lval*);
lval* lval_eval_sexpr(lval*);
lval* lval_eval(lval*);
lval* lval_pop(lval*, int);
lval* lval_take(lval*, int);
lval* lval_join(lval*, lval*);
lenv* lenv_new(void);
void lenv_delete(lenv*);
lval* builtin(lval*, char*);
lval* builtin_op(lval*, char*);
lval* builtin_head(lval*);
lval* builtin_tail(lval*);
lval* builtin_list(lval*);
lval* builtin_eval(lval*);
lval* builtin_join(lval*);
/* Add a builtin function cons that takes a value and a Q-Expression and appends it to the front. */
lval* builtin_cons(lval*);
lval* builtin_len(lval*);
/* Add a builtin function init that returns all of a Q-Expression except the final element. */
lval* builtin_init(lval*);
lval* lval_qexpr(void);

void lval_del(lval* v);
void lval_expr_print(lval*, char, char);
void lval_print(lval*);
void lval_println(lval*);



int main(int argc, char** argv) {
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Symbol   = mpc_new("symbol");
    mpc_parser_t* Sexpr    = mpc_new("sexpr");
    mpc_parser_t* Qexpr    = mpc_new("qexpr");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");
/*
'+' | '-' | '*' | '/' | '%' | '^'           \
| \"add\" | \"sub\" | \"mul\" | \"div\"     \
| \"mod\" | \"exp\" | \"min\" | \"max\"     \
| \"list\" | \"head\" | \"tail\" | \"join\" \
| \"eval\" | \"cons\" | \"len\" | \"init\"; \
*/

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                      \
        number   : /-?([0-9]*[.])?[0-9]+/ ;                    \
        symbol   : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&^%]+/ ;        \
        sexpr    : '(' <expr>* ')' ;                           \
        qexpr    : '{' <expr>* '}' ;                           \
        expr     : <number> | <symbol> | <sexpr> | <qexpr> ;   \
        lispy    : /^/ <expr>* /$/ ;                           \
        ",
        Number, Symbol, Sexpr, Qexpr, Expr, Lispy);
    
    puts("Skoogle Version 0.0.1");
    puts("Press Ctrl+c or :q to Exit\n");

    while(1) {
        char* input = readline("skoogle> ");
        add_history(input);
        if (strcmp(input, ":q") == 0) {
            free(input);
            break;
        } 
        
        mpc_result_t r; 
        if(mpc_parse("<stdin>", input, Lispy, &r)) {
            lval* x = lval_eval(lval_read(r.output));
            lval_println(x); 
            lval_del(x);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        free(input);    
    
    }
    /* Undefine and Delete our Parser */
    mpc_cleanup(5, Number, Symbol, Sexpr, Qexpr, Expr, Lispy);
    return 0;
}

/* Pointer to a new empty Qexpr lval */
lval* lval_qexpr(void) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_QEXPR;
    v->count = 0;
    v->cell  = NULL;
    return v;
}

/* Pointer to numerical lval */
lval* lval_num(long x) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

/* Pointer to Error lval */
lval* lval_err(char* m) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(sizeof(m) + 1);
    strcpy(v->err, m);
    return v; 
}

/* Pointer to the symbol lval */
lval* lval_sym(char* s) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(sizeof(s) + 1);
    strcpy(v->sym, s);
    return v;
}

/* Pointer to new empty sexpr lval */
lval* lval_sexpr(void) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

lval* lval_fun(lbuiltin func) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_FUN;
    v->fun = func;
    return v;
}

void lval_del(lval* v) {
    
    switch (v->type) {
        /* Nothing needed for function pointers */
        case LVAL_FUN: break;
        /* Do not need to do anything for numberical type */
        case LVAL_NUM: break;
        /* For Err and Sym remember to free the string data */
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
        /* If Sexpr then delete all elements inside */
        case LVAL_QEXPR:
        case LVAL_SEXPR: 
            for(int i=0; i < v->count; i++) {
                lval_del(v->cell[i]);
            }
            /* Also free the memory allocated to contain the pointers */
            free(v->cell);
        break;
    }
    /* Free the memory allocated for the lval struct itself */
    free(v);
}

lval* lval_read_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}

lval* lval_read(mpc_ast_t* t) {
    /* If Symbol or Number return conversion to that type */
    if (strstr(t->tag, "number")) { return lval_read_num(t); }
    if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

    /* If root (>) or sexpr then create empty list */
    lval* x = NULL;
    if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
    if (strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
    if (strstr(t->tag, "qexpr")) { x = lval_qexpr(); }
    /* Fill this list with any valid expressions contained */
    for (int i = 0; i < t->children_num; i++) {
        if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
        if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
        if (strcmp(t->children[i]->tag, "regex") == 0)  { continue; }
        x = lval_add(x, lval_read(t->children[i]));
    }

    return x;
}

lval* lval_add(lval* v, lval* x) {
    v->count++;
    v->cell = realloc(v->cell, sizeof(lval*) * v->count);
    v->cell[v->count-1] = x;
    return v;
}

lval* lval_copy(lval* v) {
    lval* x = malloc(sizeof(lval));
    x->type = v->type;
    switch (x->type) {
        /* Copy Functions and Numbers Directly */
        case LVAL_FUN: x->fun = v->fun; break;
        case LVAL_NUM: x->num = v->num; break;
        /* Copy Strings using malloc and strcpy */
        case LVAL_ERR:
            x->err = malloc(strlen(v->err) + 1);
            strcpy(x->err, v->err); break;

        case LVAL_SYM:
            x->sym = malloc(strlen(v->err) + 1);
            strcpy(x->err, v->err); break;

        /* Copy Lists by copying each sub-expression */
        case LVAL_SEXPR:
        case LVAL_QEXPR:
            x->count = v->count;
            x->cell = malloc(sizeof(lval*) * x->count);
            for (int i = 0; i < x->count; i++) {
                x->cell[i] = lval_copy(v->cell[i]);
            }
        break;
    }
    return x;
}

void lval_expr_print(lval* v, char open, char close) {
    putchar(open);
    for (int i = 0; i < v->count; i++) {
        /* Print Value contained within */
        lval_print(v->cell[i]);

        /* Do not print trailing space if last element */
        if (i != (v->count-1)) {
            putchar(' ');
        }
    }
    putchar(close);
}

void lval_print(lval* v) {
    switch (v->type) {
        case LVAL_FUN: printf("<function>");break;
        /* In the case the type is a number print it */
        /* Then 'break' out of the switch */
        case LVAL_NUM: printf("%li", v->num); break;
        /* In the case the typ is an error */
        case LVAL_ERR: printf("Error: %s", v->err); break;
        case LVAL_SYM: printf("%s", v->sym); break;
        case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
        case LVAL_QEXPR: lval_expr_print(v, '{', '}'); break;
        }
}


void lval_println(lval* v) { lval_print(v); putchar('\n'); }

lval* lval_eval_sexpr(lval* v) {

    /* Evaluate Children */
    for (int i = 0; i < v->count; i++) {
        v->cell[i] = lval_eval(v->cell[i]);
    }

    /* Error Checking */
    for (int i = 0; i < v->count; i++) {
        if (v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
    }

    /* Empty Expression */
    if (v->count == 0) { return v; }

    /* Single Expression */
    if (v->count == 1) { return lval_take(v,0); }

    /* Ensure First Element is Symbol */
    lval* f = lval_pop(v, 0);
    if (f->type != LVAL_SYM) {
        lval_del(f); lval_del(v);
        return lval_err("S-expression Does not start with symbol!");
    }
    
    /* Call builtin with operator */
    lval* result = builtin(v, f->sym);
    lval_del(f);
    return result;
}

lval* lval_eval(lval* v) {
    /* Evaluate Sexpressions */
    if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(v); }
    /* All other lval types remain the same */
    return v;
}

lval* lval_pop(lval* v, int i) {
    /* Find the item at "i" */
    lval* x = v->cell[i];

    /* Shift memory after the item at "i" over the top */
    memmove(&v->cell[i], &v->cell[i+1],
            sizeof(lval*) * (v->count-i-1));

    /* Decrease the count of items in the list */
    v->count--;

    /* Reallocate the memory used */
    v->cell = realloc(v->cell, sizeof(lval*) * v->count);
    return x;
}


lval* lval_take(lval* v, int i) {
    lval* x = lval_pop(v, i);
    lval_del(v);
    return x;
}

lval* builtin_op(lval* a, char* op) {
    /* Ensure all arguments are numbers */
    for (int i=0; i < a->count; i++) {
        if (a->cell[i]->type != LVAL_NUM) {
            lval_del(a);
            return lval_err("Cannot operate on non-number!");
        }
    }
    /* Pop the first element */
    lval* x = lval_pop(a, 0);
        
    /* If no arguments and sub then perform unary negation */
    if ((strcmp(op, "-") == 0) && a->count == 0) {
        x->num = -x->num;
    }
    
    /* While there are still elements remaining */
    while (a->count > 0) {
        /* Pop the next element */
        lval* y = lval_pop(a, 0);
        
        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) {
            if (y->num == 0) {
                lval_del(x); lval_del(y);
                x = lval_err("Division By Zero!"); break;
            }
            x->num /= y->num;
        }
        if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0) { 
            x->num %= y->num; }     
        if(strcmp(op, "^") == 0 || strcmp(op, "exp") == 0) { 
            x->num = pow(x->num, y->num) ;}
        if(strcmp(op, "min") == 0) {if(x->num < y->num) { continue; }
          else { x->num = y->num; }}
        if(strcmp(op, "max") == 0) {if(x->num > y->num) { continue;} 
          else { x->num = y->num; }}
        lval_del(y);
    }
    lval_del(a); 
    return x;
    
}

lval* builtin_head(lval* a) {
    LASSERT(a, a->count == 1,
    "Function 'head' passed too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'head' passed incorrect types!");
    LASSERT(a, a->cell[0]->count != 0,
    "Function 'head' passed {}");
    
    /* Otherwise take the first argument */
    lval* v = lval_take(a, 0);

    /* Delete all elements that are not head and return */
    while (v->count > 1) {lval_del(lval_pop(v,1)); }
    return v;
}

lval* builtin_tail(lval* a) {
    LASSERT(a, a->count == 1,
    "Function 'tail' passed too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'tail' passed incorrect types!");
    LASSERT(a, a->cell[0]->count != 0,
    "Function 'tail' passed {}!");
    
    /* Take first argument */
    lval* v = lval_take(a, 0);

    lval_del(lval_pop(v,0));
    return v;
}

lval* builtin_init(lval* a) {
    LASSERT(a, a->count == 1,
    "Function 'init' passed too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'init' passed incorrect types!");
    LASSERT(a, a->cell[0]->count != 0,
    "Function 'init' passed {}");
    
    /* Take the first argument passed */
    lval* v = lval_pop(a, 0);
    lval_del(lval_pop(v,v->count - 1)); 

    return v;
}

lval* builtin_cons(lval* a) {
    lval* x = lval_pop(a, a->count-1);
    LASSERT(x, x->type == LVAL_QEXPR,
    "Functions 'cons' ended with wrong type.") 
    lval* y = lval_qexpr();
    while(a->count){
        lval_add(y, lval_pop(a, 0));
    }
    y = lval_join(y, x);
    lval_del(a);
    return y;
}

lval* builtin_list(lval* a) {
    a->type = LVAL_QEXPR;
    return a;
}

lval* builtin_eval(lval* a) {
    LASSERT(a, a->count == 1,
    "Function 'eval' passed too many arguments!")
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'eval' passed incorrect type!")

    lval* x = lval_take(a, 0);
    x->type = LVAL_SEXPR;
    return lval_eval(x);
}

lval* builtin_join(lval* a) {
    for (int i = 0; i < a->count; i++) {
        LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
        "Function 'join' passed incorrect type.");
    }
    lval* x = lval_pop(a, 0);
    
    while (a->count) {
        x = lval_join(x, lval_pop(a, 0));
    }
    lval_del(a);
    return x;
}

lval* builtin_len(lval* a) {
    LASSERT(a, a->count == 1, 
    "Function 'len' passed to many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'len' passed incorrect type.");
    lval* v = lval_num(a->cell[0]->count);
    lval_del(a);
    return v;
}

lval* lval_join(lval* x, lval* y) {
    /* For each cell in 'y' add it to 'x' */
    while (y->count) {
        x = lval_add(x, lval_pop(y, 0));
    }

    /* Delete the empty 'y' and return 'x' */
    lval_del(y);
    return x;
}

lenv* lenv_new(void) {
    lenv* e = malloc(sizeof(lenv));
    e->count = 0;
    e->syms = NULL;
    e->vals = NULL;
    return e;
}

void lenv_del(lenv* e) {
    for (int i = 0; i < e->count; i++) {
        free(e->syms[i]);
        lval_del(e->vals[i]);
    }
    free(e->syms);
    free(e->vals);
    free(e);
}

lval* builtin(lval* a, char* func) {
    if (strcmp("list", func) == 0) { return builtin_list(a); }
    if (strcmp("head", func) == 0) { return builtin_head(a); }
    if (strcmp("tail", func) == 0) { return builtin_tail(a); }
    if (strcmp("join", func) == 0) { return builtin_join(a); }
    if (strcmp("eval", func) == 0) { return builtin_eval(a); }
    if (strcmp("cons", func) == 0) { return builtin_cons(a); }
    if (strcmp("len", func) == 0)  { return builtin_len(a);  }
    if (strcmp("init", func) == 0) { return builtin_init(a); }
    if (strstr("+-/*^%", func))    {return builtin_op(a, func); }
    if (strcmp("min", func) == 0 ||
        strcmp("max", func) == 0)  { return builtin_op(a, func); }
    lval_del(a);
    return lval_err("Unknown Function!");
}
