#include <stdbool.h>

typedef struct _declist *declist;
typedef struct _decl *decl;
typedef struct _exprlist *exprlist;
typedef struct _expr *expr;
typedef struct _stmlist *stmlist;
typedef struct _stm *stm;
typedef struct _arglist *arglist;
typedef struct _arg *arg;
typedef struct _idlist *idlist;
typedef struct _type *type;

struct _declist {       // lista de declarações
    enum {
        singledecl,     // uma declaração
        moredecl        // múltiplas declarações
    } kind;

    union {
        struct {
            decl d;         
            declist decls;
        } decliststruct;
    } u;
};

struct _decl {          // declaração
    enum {
        vardecl,        // delcaração de variável
        vardeclinic,    // declaração de variável com inicialização
        funcdecl,       // declaração de função
        funcdeclargs,   // declaração de função com argumentos
        typedecl        // declaração de novo tipo
    } kind;

    union {
        struct {        // struct de variável
            idlist id;  // numa declaração, pode haver várias variaveis declaradas
            type t;     // tipo de variável
            expr e;     // valor da variável
        } var;

        struct {            // struct da função
            char *id;       // nome da função
            type t;         // tipo da função
            arglist args;   // argumentos da função  
            stmlist stmts;  // corpo da função
        } function;

        struct {            // struct de um novo tipo
            char *id;       // nome do novo tipo
            type t;         // tipo do novo tipo
        } newtype;
    } u;
};

struct _exprlist {          // várias expressões
    enum {
        singleexpr,          // uma expressão
        moreexpr            // várias
    } kind;

    union {
        struct {
            expr e;
            exprlist exprs;
        } exprliststruct;
    } u;
};

struct _expr {
    enum {
        expr_int,            // inteiro
        expr_float,          // float
        expr_bool,           // booleano
        expr_string,         // string
        expr_id,             // variavel
        expr_binop,          // expressão com operação binária
        expr_unop,           // expressão com operação unária
        expr_assign,         // afetação
        expr_fcall,          // chamada de função
        expr_fcallargs       // chamada de função com argumentos
    } kind;

    union {
        int i;
        float f;
        bool b;
        char *id;       // funciona também para expr string

        struct {
            char op[4];     // operador maior é "mod" (tamanho 3 + terminador)
            expr op1;
            expr op2; 
        } binop;            // também funciona para unop.
        
        struct {
            char *id;
            expr e;
        } assign;

        struct {
            char *id;
            exprlist args;
        } fcall;    
    } u;
};

struct _stmlist {       // lista de statements (para corpo de funções)
    enum {
        singlestm,      // um statement
        morestm         // vários statements
    } kind;

    union {
        struct {
            stm s;
            stmlist stms;
        } stmliststruct;
    } u;
};

struct _stm {           // statements
    enum {  
        stm_decl,       // declaração de variável
        stm_expr,       // expressão
        stm_if,         // if then
        stm_ifelse,     // if then else
        stm_while,      // while 
        stm_return,     // return
    } kind;

    union {
        decl d;
        expr e;                 // pode ser usado para o return

        struct {
            expr cond;
            stmlist thenstms;
            stmlist elsestms;
        } condition;

        struct {
            expr cond;
            stmlist stms;
        } cicle;
    } u;
};

struct _arglist {   // lista de argumentos
    enum {
        singlearg,
        moreargs
    } kind;

    union {
        struct {
            arg a;
            arglist args;
        } argliststruct;
    } u;
};

struct _arg {       // argumento de uma função
    enum {
        arg_id      // nome da variavel parametro
    } kind;

    union {
        struct {
            char *id;
            type t;
        } argument;
    } u;
};

struct _idlist {        // lista de variaveis
    enum {
        singleid,
        moreids
    } kind;

    union {
        struct {
            char *id;
            idlist ids;
        } idliststruct;
    } u;
};

struct _type {          // tipo de variaveis
    enum {
        type_int,
        type_float,
        type_string,
        type_bool,
        type_new,
        type_void
    } kind;

    union {
        char *id;       // para novos tipos criados
    } u;
};

/*  ============== FUNCTIONS ============== */

declist declistnode(decl d, declist decls);
decl vardeclnode(idlist ids, type t, expr e);
decl funcdeclnode(char *id, arglist args, type t, stmlist stms);
decl newtypedeclnode(char *id, type t);

stmlist stmlistnode(stm s, stmlist stms);
stm declstmnode(decl d);
stm exprstmnode(expr e);
stm ifstmnode(expr cond, stmlist thenstms, stmlist elsestms);
stm whilestmnode(expr cond, stmlist stms);
stm returnstmnode(expr e);

exprlist exprlistnode(expr e, exprlist exprs);
expr intexprnode(int i);
expr floatexprnode(float f);
expr boolexprnode(bool b);
expr stringexprnode(char *s);
expr idexprnode(char *id);
expr binopexprnode(char op[3], expr op1, expr op2);
expr unopexprnode(char op[2], expr e);
expr assignexprnode(char *id, expr e);
expr fcallexprnode(char *id, exprlist exprs);

arglist arglistnode(arg a, arglist args);
arg argnode(char *id, type t);

idlist idlistnode(char *id, idlist ids);

type typenode(char *id);