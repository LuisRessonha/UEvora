#include <stdlib.h>
#include <string.h>
#include "apt.h"

declist declistnode(decl d, declist decls)
{
    declist node = malloc(sizeof(*node));

    if (decls == NULL)
        node->kind = singledecl;
    else
    {
        node->kind = moredecl;
        node->u.decliststruct.decls = decls;
    }

    node->u.decliststruct.d = d;

    return node;
}

decl vardeclnode(idlist ids, type t, expr e)
{
    decl node = malloc(sizeof(*node));
    if (e == NULL)
        node->kind = vardecl;
    else
    {
        node->kind = vardeclinic;
        node->u.var.e = e;
    }

    node->u.var.id = ids;
    node->u.var.t = t;

    return node;
}

decl funcdeclnode(char *id, arglist args, type t, stmlist stms)
{
    decl node = malloc(sizeof(*node));

    if (args == NULL)
        node->kind = funcdecl;
    else
    {
        node->kind = funcdeclargs;
        node->u.function.args = args;
    }

    strcpy(node->u.function.id, id);
    node->u.function.stmts = stms;
    node->u.function.t = t;

    return node;
}

decl newtypedeclnode(char *id, type t)
{
    decl node = malloc(sizeof(*node));

    node->kind = typedecl;
    strcpy(node->u.newtype.id, id);
    node->u.newtype.t = t;

    return node;
}

stmlist stmlistnode(stm s, stmlist stms)
{
    stmlist node = malloc(sizeof(*node));

    if (stms == NULL)
        node->kind = singlestm;
    else
    {
        node->kind = morestm;
        node->u.stmliststruct.stms = stms;
    }

    node->u.stmliststruct.s = s;

    return node;    
}

stm declstmnode(decl d)
{
    stm node = malloc(sizeof(*node));

    node->kind = stm_decl;
    node->u.d = d;

    return node;
}

stm exprstmnode(expr e)
{
    stm node = malloc(sizeof(*node));

    node->kind = stm_expr;
    node->u.e = e;

    return node;
}

stm ifstmnode(expr cond, stmlist thenstms, stmlist elsestms)
{
    stm node = malloc(sizeof(*node));

    if (elsestms == NULL)
        node->kind = stm_if;
    else
    {
        node->kind = stm_ifelse;
        node->u.condition.elsestms = elsestms;
    }

    node->u.condition.cond = cond;
    node->u.condition.thenstms = thenstms;

    return node;
}   

stm whilestmnode(expr cond, stmlist stms)
{
    stm node = malloc(sizeof(*node));

    node->kind = stm_while;
    node->u.cicle.cond = cond;
    node->u.cicle.stms = stms;

    return node;
}

stm returnstmnode(expr e)
{
    stm node = malloc(sizeof(*node));

    node->kind = stm_return;
    node->u.e = e;      // expressao no node stm funciona também como retorno

    return node;
}

exprlist exprlistnode(expr e, exprlist exprs)
{
    exprlist node = malloc(sizeof(*node));

    if (exprs == NULL)
        node->kind = singleexpr;
    else
    {
        node->kind = moreexpr;
        node->u.exprliststruct.exprs = exprs;
    }

    node->u.exprliststruct.e = e;

    return node;
}

expr intexprnode(int i)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_int;
    node->u.i = i;

    return node;
}

expr floatexprnode(float f)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_float;
    node->u.f = f;

    return node;
}

expr boolexprnode(bool b)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_bool;
    node->u.b = b;

    return node;
}

expr stringexprnode(char *s)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_string;
    strcpy(node->u.id, s);

    return node;
}

expr idexprnode(char *id)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_id;
    strcpy(node->u.id, id);

    return node;
}

expr binopexprnode(char op[3], expr op1, expr op2)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_binop;
    strcpy(node->u.binop.op, op);
    node->u.binop.op1 = op1;
    node->u.binop.op2 = op2;

    return node;
}

expr unopexprnode(char op[2], expr e)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_unop;

    strcpy(node->u.binop.op, op);
    node->u.binop.op1 = e;

    return node; 
}

expr assignexprnode(char *id, expr e)
{
    expr node = malloc(sizeof(*node));

    node->kind = expr_assign;

    node->u.assign.e = e;
    strcpy(node->u.assign.id, id);

    return node;
}

expr fcallexprnode(char *id, exprlist exprs)
{
    expr node = malloc(sizeof(*node));

    if (exprs == NULL)
        node->kind = expr_fcall;
    else
    {
        node->kind = expr_fcallargs;
        node->u.fcall.args = exprs;
    }

    strcpy(node->u.fcall.id, id);

    return node;
}

arglist arglistnode(arg a, arglist args)
{
    arglist node = malloc(sizeof(*node));

    if(args == NULL)
        node->kind = singlearg;
    else
    {
        node->kind = moreargs;
        node->u.argliststruct.args = args;
    }

    node->u.argliststruct.a = a;
     
    return node;
}

arg argnode(char *id, type t)
{
    arg node = malloc(sizeof(*node));

    node->kind = arg_id;
    node->u.argument.t = t;
    strcpy(node->u.argument.id, id);

    return node;
}

idlist idlistnode(char *id, idlist ids)
{
    idlist node = malloc(sizeof(*node));

    if (ids == NULL)
        node->kind = singleid;
    else
    {
        node->kind = moreids;
        node->u.idliststruct.ids = ids;
    }
    
    strcpy(node->u.idliststruct.id, id);
    
    return node;
}

type typenode(char *id)
{
    type node = malloc(sizeof(*node));

    node->kind = type_new;

    if (strcmp(id, "int") == 0)
        node->kind = type_int;
    if (strcmp(id, "float") == 0)
        node->kind = type_float;
    if (strcmp(id, "string") == 0)
        node->kind = type_string;
    if (strcmp(id, "bool") == 0)
        node->kind = type_bool;
    if (strcmp(id, "void") == 0)
        node->kind = type_void;
    
    strcpy(node->u.id, id);

    return node;
}