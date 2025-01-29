#ifndef SHUNTING_YARD_H_
#define SHUNTING_YARD_H_

#include <iostream>
#include "assert.h"
#include "../../includes/table/typedefs.h"
#include "../../includes/token/token.h"
#include "../../includes/token/child_tokens.h"

#include "string.h"
#include "../../includes/stack/MyStack.h"

#include "../../includes/queue/MyQueue.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/bplustree/btree_array_funcs.h"
#include <algorithm>


class ShuntingYard {
    public:
    ShuntingYard ();
    ShuntingYard (Queue <Token*> infix);
    ShuntingYard (vectorstr list);
    ShuntingYard (Queue <Token*> infix,vectorstr list);
    Queue <Token*> postfix();
    Queue <Token*> postfix(Queue <Token*> Sample);
    void infix(Queue <Token*> Sample);
    void tokenize ();
    // friend ostream& operator << (ostream& outs, const ShuntingYard& printMe);

    bool is_logic (string option);
    bool is_relation (string option);
    bool is_republican (string option);
    bool is_democrat (string option);
    // bool is_logic (string option);

    private:
    Queue <Token *> _inner;
    vectorstr to_fix;

};

#endif // SHUNTING_YARD_H_