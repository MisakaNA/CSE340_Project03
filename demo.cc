#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <map>
#include "compiler.h"
#include "lexer.h"
using namespace std;
LexicalAnalyzer lexer;
Token t;

map<string, int> inputVar;
struct InstructionNode* misaka;
//int switchOp1 = -1;
void parse_program();
void parse_var_section();
void parse_id_list();
InstructionNode * parse_body();
InstructionNode * parse_stmt_list();
InstructionNode * parse_stmt();
InstructionNode * parse_assign_stmt();
InstructionNode * parse_expr();
int parse_primary();
TokenType parse_op();
InstructionNode * parse_output_stmt();
InstructionNode * parse_input_stmt();
InstructionNode * parse_while_stmt();
InstructionNode * parse_if_stmt();
InstructionNode * parse_condition();
TokenType parse_relop();
InstructionNode * parse_switch_stmt(struct InstructionNode* covid_19);
InstructionNode * parse_for_stmt();
InstructionNode * parse_case_list(int op, struct InstructionNode* covid_19);
InstructionNode * parse_case(int op);
InstructionNode * parse_default_case();
void parse_inputs();
void parse_num_list();

Token peek(){
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

void syntax_error(){
    cout << "SYNTAX ERROR !!!\n";
    exit(1);
}

Token expect(TokenType expected_type){
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

struct InstructionNode * parse_generate_intermediate_representation()
{

    parse_program();
    return misaka;
}

void parse_program(){
    //var_section
    t = peek();
    if(t.token_type == ID) {
        parse_var_section();
    } else{
        syntax_error();
    }
    //body
    t = peek();
    if(t.token_type == LBRACE){
        misaka = parse_body();
    } else{
        syntax_error();
    }
    //inputs
    t = peek();
    if(t.token_type == NUM){
        parse_inputs();
    } else{
        syntax_error();
    }
}

void parse_var_section(){
    //id_list
    parse_id_list();
    expect(SEMICOLON);
}

void parse_id_list(){
    t = expect(ID);
    //store variables to the map at the first line
    inputVar[t.lexeme] = next_available;
    //initial the var to 0 in mem and match with the index
    mem[next_available++] = 0;

    t = lexer.GetToken();
    if(t.token_type == COMMA){
        parse_id_list();
    } else{
        lexer.UngetToken(t);
    }
}

struct InstructionNode* parse_body(){
    struct InstructionNode* instl = nullptr;
    expect(LBRACE);
    //stmt_list
    instl = parse_stmt_list();
    expect(RBRACE);
    return instl;
}

struct InstructionNode* parse_stmt_list(){
    struct InstructionNode* inst = nullptr;
    struct InstructionNode* instl = nullptr;
    TokenType debug1 = t.token_type;
    string debug2 = t.lexeme;
    inst = parse_stmt();

    t = peek();
    if(t.token_type == ID || t.token_type == WHILE || t.token_type == IF || t.token_type == SWITCH
       || t.token_type == FOR || t.token_type == OUTPUT || t.token_type == INPUT){
        instl = parse_stmt_list();

        //assign instl to the tail of inst
        struct InstructionNode* getLast = inst;
        while(getLast->next != nullptr){
            getLast = getLast->next;
        }
        getLast->next = instl;
    }
    return inst;
}

struct InstructionNode* parse_stmt(){
    struct InstructionNode* inst = nullptr;
    struct InstructionNode* covid_19 = new InstructionNode;
    covid_19->type = NOOP;
    covid_19->next = nullptr;
    struct InstructionNode* getLast;
    t = peek();
    switch(t.token_type){
        case ID:
            inst = parse_assign_stmt();
            break;
        case WHILE:
            inst = parse_while_stmt();
            break;
        case IF:
            inst = parse_if_stmt();
            break;
        case SWITCH:
            inst = parse_switch_stmt(covid_19);
            //set a end node for the switch statement
            //every case will jump to this at the end
            //it follows the DEFAULT naturally
            getLast = inst;
            while(getLast->next != nullptr){
                getLast = getLast->next;
            }
            getLast->next = covid_19;
            break;
        case FOR:
            inst = parse_for_stmt();
            break;
        case OUTPUT:
            inst = parse_output_stmt();
            break;
        case INPUT:
            inst = parse_input_stmt();
            break;
        default:
            syntax_error();
    }

    return inst;
}

struct InstructionNode* parse_assign_stmt(){
    auto* assignInst = new InstructionNode;
    assignInst->type = ASSIGN;
    //left hand side
    t = expect(ID);
    assignInst->assign_inst.left_hand_side_index = inputVar[t.lexeme];

    expect(EQUAL);

    //can b assigned to a NUM or an expr
    Token t1 = lexer.GetToken();
    Token t2 = peek();
    lexer.UngetToken(t1);
    if (t1.token_type == ID || t1.token_type == NUM){
        if (t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV) {
            //expr
            struct InstructionNode* kotori = parse_expr();
            assignInst->assign_inst.operand1_index = kotori ->assign_inst.operand1_index;
            assignInst->assign_inst.op = kotori->assign_inst.op;
            assignInst->assign_inst.operand2_index = kotori->assign_inst.operand2_index;
        } else if(t2.token_type == SEMICOLON){
            //primary
            assignInst->assign_inst.op = OPERATOR_NONE;
            assignInst->assign_inst.operand1_index = parse_primary();
        } else{
            syntax_error();
        }
    } else{
        syntax_error();
    }
    expect(SEMICOLON);
    //set next to null
    assignInst->next = nullptr;
    return assignInst;
}

struct InstructionNode* parse_expr(){
    auto *getInfo = new InstructionNode;
    getInfo->assign_inst.operand1_index = parse_primary();

    switch(parse_op()){
        case PLUS:
            getInfo->assign_inst.op = OPERATOR_PLUS;
            break;
        case MINUS:
            getInfo->assign_inst.op = OPERATOR_MINUS;
            break;
        case MULT:
            getInfo->assign_inst.op = OPERATOR_MULT;
            break;
        case DIV:
            getInfo->assign_inst.op = OPERATOR_DIV;
            break;
        default:
            break;
    }
    getInfo->assign_inst.operand2_index = parse_primary();

    return getInfo;
}

int parse_primary(){
    int index = -1;
    t = lexer.GetToken();
    if(t.token_type == ID || t.token_type == NUM){
        if(t.token_type == ID){
            index = inputVar[t.lexeme];
        } else{
            index = next_available;
            mem[next_available++] = stoi(t.lexeme);
        }
    } else{
        syntax_error();
    }
    return index;
}

TokenType parse_op(){
    t = lexer.GetToken();
    if(t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV){
        return t.token_type;
    } else{
        syntax_error();
    }
}

struct InstructionNode* parse_output_stmt(){
    auto *outputInst = new InstructionNode;
    expect(OUTPUT);
    outputInst->type = OUT;
    t = expect(ID);
    outputInst->output_inst.var_index = inputVar[t.lexeme];
    outputInst->next = nullptr;
    expect(SEMICOLON);
    return outputInst;
}

struct InstructionNode* parse_input_stmt(){
    auto *inputInst = new InstructionNode;
    expect(INPUT);
    inputInst->type = IN;
    t = expect(ID);
    inputInst->input_inst.var_index = inputVar[t.lexeme];
    inputInst->next = nullptr;
    expect(SEMICOLON);
    return inputInst;
}

struct InstructionNode* parse_while_stmt(){
    auto *whileInst = new InstructionNode;
    expect(WHILE);
    whileInst->type = CJMP;
    struct InstructionNode* nagisa = parse_condition();
    whileInst->cjmp_inst.operand1_index = nagisa->cjmp_inst.operand1_index;
    whileInst->cjmp_inst.condition_op = nagisa->cjmp_inst.condition_op;
    whileInst->cjmp_inst.operand2_index = nagisa->cjmp_inst.operand2_index;

    t = peek();
    if(t.token_type == LBRACE){
        whileInst->next = parse_body();
    } else{
        syntax_error();
    }

    auto* jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->jmp_inst.target = whileInst;

    auto* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;

    struct InstructionNode* getLast = whileInst;
    while(getLast->next != nullptr){
        getLast = getLast->next;
    }
    getLast->next = jmp;
    jmp->next = noop;
    whileInst->cjmp_inst.target = noop;

    return whileInst;
}

struct InstructionNode* parse_if_stmt(){
    auto *ifInst = new InstructionNode;
    expect(IF);

    ifInst->type = CJMP;
    struct InstructionNode* temp = parse_condition();
    ifInst->cjmp_inst.operand1_index = temp->cjmp_inst.operand1_index;
    ifInst->cjmp_inst.condition_op = temp->cjmp_inst.condition_op;
    ifInst->cjmp_inst.operand2_index = temp->cjmp_inst.operand2_index;

    t = peek();
    if(t.token_type == LBRACE){
        ifInst->next = parse_body();
    } else{
        syntax_error();
    }

    auto* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;

    struct InstructionNode* getLast = ifInst;
    while(getLast->next != nullptr){
        getLast = getLast->next;
    }
    getLast->next = noop;

    ifInst->cjmp_inst.target = noop;

    return ifInst;
}

struct InstructionNode* parse_condition(){
    auto* getCondInfo = new InstructionNode;
    //primary
    t = peek();
    if(t.token_type == ID || t.token_type == NUM){
        getCondInfo->cjmp_inst.operand1_index = parse_primary();
    } else{
        syntax_error();
    }
    //relop
    t = peek();
    if (t.token_type == GREATER || t.token_type == LESS || t.token_type == NOTEQUAL){
        switch(parse_relop()){
            case GREATER:
                getCondInfo->cjmp_inst.condition_op = CONDITION_GREATER;
                break;
            case LESS:
                getCondInfo->cjmp_inst.condition_op = CONDITION_LESS;
                break;
            case NOTEQUAL:
                getCondInfo->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
                break;
            default:
                break;
        }
    } else{
        syntax_error();
    }
    //primary
    t = peek();
    if (t.token_type == ID || t.token_type == NUM) {
        getCondInfo->cjmp_inst.operand2_index = parse_primary();
    } else{
        syntax_error();
    }
    return getCondInfo;
}

TokenType parse_relop(){
    t = lexer.GetToken();
    if(t.token_type == GREATER || t.token_type == LESS || t.token_type == NOTEQUAL){
        return t.token_type;
    } else{
        syntax_error();
    }
}

struct InstructionNode* parse_switch_stmt(struct InstructionNode* covid_19){
    auto* switchInst = new InstructionNode;
    expect(SWITCH);

    t = expect(ID);
    int switchOp1 = inputVar[t.lexeme];
    expect(LBRACE);
    //case_list
    t = peek();
    if(t.token_type == CASE){
        switchInst = parse_case_list(switchOp1, covid_19);


    } else{
        syntax_error();
    }

    //default_case
    t = peek();
    if(t.token_type == DEFAULT){

        struct InstructionNode* getLast = switchInst;
        while(getLast->next->next != nullptr){
            getLast = getLast->next;
        }
        getLast->next = parse_default_case();

        expect(RBRACE);
    } else if(t.token_type == RBRACE){
        t = lexer.GetToken();
        return switchInst;
    } else{
        syntax_error();
    }
    return switchInst;
}

struct InstructionNode* parse_for_stmt(){
    auto* forInst = new InstructionNode;
    auto* assignStmt = new InstructionNode;
    expect(FOR);
    expect(LPAREN);

    //assign_stmt
    t = peek();
    if(t.token_type == ID){
        forInst = parse_assign_stmt();
    } else{
        syntax_error();
    }

    auto* tempWhileInst = new InstructionNode;
    tempWhileInst->type = CJMP;
    //condition
    struct InstructionNode* sagiri = parse_condition();
    tempWhileInst->cjmp_inst.operand1_index = sagiri->cjmp_inst.operand1_index;
    tempWhileInst->cjmp_inst.condition_op = sagiri->cjmp_inst.condition_op;
    tempWhileInst->cjmp_inst.operand2_index = sagiri->cjmp_inst.operand2_index;

    expect(SEMICOLON);
    t = peek();
    if(t.token_type == ID){
        assignStmt = parse_assign_stmt();
        assignStmt->next = nullptr;
        expect(RPAREN);
    } else{
        syntax_error();
    }
    //body
    t = peek();
    if (t.token_type == LBRACE){
        tempWhileInst->next = parse_body();
    } else{
        syntax_error();
    }

    //add the second stmt like a = a + 1 (a++)
    auto* addStmt = tempWhileInst->next;
    while(addStmt->next != nullptr){
        addStmt = addStmt->next;
    }
    addStmt->next = assignStmt;

    auto* jmp = new InstructionNode;
    jmp->type = JMP;
    jmp->jmp_inst.target = tempWhileInst;

    auto* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;

    jmp->next = noop;

    struct InstructionNode* getLast = tempWhileInst;
    while(getLast->next != nullptr){
        getLast = getLast->next;
    }
    getLast->next = jmp;
    tempWhileInst->cjmp_inst.target = noop;

    forInst->next = tempWhileInst;

    return forInst;
}

struct InstructionNode* parse_case_list(int op, struct InstructionNode* covid_19){
    auto* caseNode = new InstructionNode;
    struct InstructionNode* caseList = nullptr;
    //case
    t = peek();
    if(t.token_type == CASE){
        caseNode = parse_case(op);

        auto* jmp = new InstructionNode;
        jmp->type = JMP;
        jmp->jmp_inst.target = covid_19;
        struct InstructionNode* getLast = caseNode->cjmp_inst.target;
        while(getLast->next->next != nullptr){
            getLast = getLast->next;
        }
        getLast->next = jmp;

    } else{
        syntax_error();
    }
    //case_list
    t = peek();
    if(t.token_type == CASE){
        caseList = parse_case_list(op, covid_19);

        //add caseList to the tail of caseNode
        struct InstructionNode* getLast = caseNode;
        while(getLast->next->next != nullptr){
            getLast = getLast->next;
        }
        getLast->next = caseList;
    }
    return caseNode;
}

struct InstructionNode* parse_case(int op){
    auto* caseInst = new InstructionNode;
    expect(CASE);
    caseInst->type = CJMP;
    caseInst->cjmp_inst.operand1_index = op;
    caseInst->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    t = expect(NUM);

    //add the NUM to mem
    int index = next_available;
    mem[next_available++] = stoi(t.lexeme);
    //assign the num to operand2
    caseInst->cjmp_inst.operand2_index = index;

    expect(COLON);
    //body
    t = peek();
    if (t.token_type == LBRACE){
        caseInst->cjmp_inst.target = parse_body();
    } else{
        syntax_error();
    }

    auto* noop = new InstructionNode;
    noop->type = NOOP;
    noop->next = nullptr;


    struct InstructionNode* getLast = caseInst->cjmp_inst.target;
    while(getLast->next != nullptr){
        getLast = getLast->next;
    }
    caseInst->next = noop;
    getLast->next = caseInst->next;
    return caseInst;
}

struct InstructionNode * parse_default_case(){
    auto* defaultInst = new InstructionNode;
    expect(DEFAULT);
    expect(COLON);
    //body
    t = peek();
    if (t.token_type == LBRACE){
        defaultInst = parse_body();
    } else{
        syntax_error();
    }
    return defaultInst;
}

void parse_inputs(){
    parse_num_list();
}

void parse_num_list(){
    t = expect(NUM);
    //append input number
    inputs.push_back(stoi(t.lexeme));
    //num_list
    t = peek();
    if(t.token_type == NUM){
        parse_num_list();
    }
}