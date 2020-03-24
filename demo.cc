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
vector<int> inputNum;
map<string, int> inputVar;
struct InstructionNode* programBody;

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
InstructionNode * parse_switch_stmt();
InstructionNode * parse_for_stmt();
void parse_case_list();
void parse_case();
void parse_default_case();
void parse_inputs();
void parse_num_list();

struct InstructionNode * parse_generate_intermediate_representation()
{
    parse_program();
    return programBody;
    /* Sample program for demonstration purpose only
     Replace the following with a parser that reads the program from stdin &
     creates appropriate data structures to be executed by execute_program()
     This is the imaginary input for the following construction:

     a, b, c, d;
     {
         input a;
         input b;
         c = 10;

         IF c <> a
         {
             output b;
         }

         IF c > 1
         {
             a = b + 900;
             input d;
             IF a > 10
             {
                 output d;
             }
         }

         d = 0;
         WHILE d < 4
         {
             c = a + d;
             IF d > 1
             {
                 output d;
             }
             d = d + 1;
         }
     }
     1 2 3 4 5 6

     Assigning location for variable "a"*/
    /*int address_a = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "b"
    int address_b = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "c"
    int address_c = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "d"
    int address_d = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 10
    int address_ten = next_available;
    mem[next_available] = 10;
    next_available++;

    // Assigning location for constant 1
    int address_one = next_available;
    mem[next_available] = 1;
    next_available++;

    // Assigning location for constant 900
    int address_ninehundred = next_available;
    mem[next_available] = 900;
    next_available++;

    // Assigning location for constant 3
    int address_three = next_available;
    mem[next_available] = 3;
    next_available++;

    // Assigning location for constant 0
    int address_zero = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 4
    int address_four = next_available;
    mem[next_available] = 4;
    next_available++;

    struct InstructionNode * i1 = new InstructionNode;
    struct InstructionNode * i2 = new InstructionNode;
    struct InstructionNode * i3 = new InstructionNode;
    struct InstructionNode * i4 = new InstructionNode;
    struct InstructionNode * i5 = new InstructionNode;
    struct InstructionNode * i6 = new InstructionNode;
    struct InstructionNode * i7 = new InstructionNode;
    struct InstructionNode * i8 = new InstructionNode;
    struct InstructionNode * i9 = new InstructionNode;
    struct InstructionNode * i10 = new InstructionNode;
    struct InstructionNode * i11 = new InstructionNode;
    struct InstructionNode * i12 = new InstructionNode;
    struct InstructionNode * i13 = new InstructionNode;
    struct InstructionNode * i14 = new InstructionNode;
    struct InstructionNode * i15 = new InstructionNode;
    struct InstructionNode * i16 = new InstructionNode;
    struct InstructionNode * i17 = new InstructionNode;
    struct InstructionNode * i18 = new InstructionNode;
    struct InstructionNode * i19 = new InstructionNode;
    struct InstructionNode * i20 = new InstructionNode;
    struct InstructionNode * i21 = new InstructionNode;
    struct InstructionNode * i22 = new InstructionNode;

    i1->type = IN;                                      // input a
    i1->input_inst.var_index = address_a;
    i1->next = i2;

    i2->type = IN;                                      // input b
    i2->input_inst.var_index = address_b;
    i2->next = i3;

    i3->type = ASSIGN;                                  // c = 10
    i3->assign_inst.left_hand_side_index = address_c;
    i3->assign_inst.op = OPERATOR_NONE;
    i3->assign_inst.operand1_index = address_ten;
    i3->next = i4;

    i4->type = CJMP;                                    // if c <> a
    i4->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    i4->cjmp_inst.operand1_index = address_c;
    i4->cjmp_inst.operand2_index = address_a;
    i4->cjmp_inst.target = i6;                          // if not (c <> a) skip forward to NOOP
    i4->next = i5;

    i5->type = OUT;                                     // output b
    i5->output_inst.var_index = address_b;
    i5->next = i6;

    i6->type = NOOP;                                    // NOOP after IF
    i6->next = i7;

    i7->type = CJMP;                                    // if c > 1
    i7->cjmp_inst.condition_op = CONDITION_GREATER;
    i7->cjmp_inst.operand1_index = address_c;
    i7->cjmp_inst.operand2_index = address_one;
    i7->cjmp_inst.target = i13;                         // if not (c > 1) skip forward to NOOP (way down)
    i7->next = i8;

    i8->type = ASSIGN;                                  // a = b + 900
    i8->assign_inst.left_hand_side_index = address_a;
    i8->assign_inst.op = OPERATOR_PLUS;
    i8->assign_inst.operand1_index = address_b;
    i8->assign_inst.operand2_index = address_ninehundred;
    i8->next = i9;

    i9->type = IN;                                      // input d
    i9->input_inst.var_index = address_d;
    i9->next = i10;

    i10->type = CJMP;                                   // if a > 10
    i10->cjmp_inst.condition_op = CONDITION_GREATER;
    i10->cjmp_inst.operand1_index = address_a;
    i10->cjmp_inst.operand2_index = address_ten;
    i10->cjmp_inst.target = i12;                        // if not (a > 10) skipp forward to NOOP
    i10->next = i11;

    i11->type = OUT;                                    // output d
    i11->output_inst.var_index = address_d;
    i11->next = i12;

    i12->type = NOOP;                                   // NOOP after inner IF
    i12->next = i13;

    i13->type = NOOP;                                   // NOOP after outer IF
    i13->next = i14;

    i14->type = ASSIGN;                                 // d = 0
    i14->assign_inst.left_hand_side_index = address_d;
    i14->assign_inst.op = OPERATOR_NONE;
    i14->assign_inst.operand1_index = address_zero;
    i14->next = i15;

    i15->type = CJMP;                                   // if d < 4
    i15->cjmp_inst.condition_op = CONDITION_LESS;
    i15->cjmp_inst.operand1_index = address_d;
    i15->cjmp_inst.operand2_index = address_four;
    i15->cjmp_inst.target = i22;                        // if not (d < 4) skip whole WHILE body
    i15->next = i16;

    i16->type = ASSIGN;                                 // c = a + d
    i16->assign_inst.left_hand_side_index = address_c;
    i16->assign_inst.op = OPERATOR_PLUS;
    i16->assign_inst.operand1_index = address_a;
    i16->assign_inst.operand2_index = address_d;
    i16->next = i17;

    i17->type = CJMP;                                   // if d > 1
    i17->cjmp_inst.condition_op = CONDITION_GREATER;
    i17->cjmp_inst.operand1_index = address_d;
    i17->cjmp_inst.operand2_index = address_one;
    i17->cjmp_inst.target = i19;                        // if not (d > 1) skip body of IF
    i17->next = i18;

    i18->type = OUT;                                    // output d
    i18->output_inst.var_index = address_d;
    i18->next = i19;

    i19->type = NOOP;                                   // NOOP after body of IF
    i19->next = i20;

    i20->type = ASSIGN;                                 // d = d + 1
    i20->assign_inst.left_hand_side_index = address_d;
    i20->assign_inst.op = OPERATOR_PLUS;
    i20->assign_inst.operand1_index = address_d;
    i20->assign_inst.operand2_index = address_one;
    i20->next = i21;

    i21->type = JMP;
    i21->jmp_inst.target = i15;
    i21->next = i22;

    i22->type = NOOP;                                   // NOOP after body of WHILE
    i22->next = NULL;

    // Inputs
    inputs.push_back(1);
    inputs.push_back(2);
    inputs.push_back(3);
    inputs.push_back(4);
    inputs.push_back(5);
    inputs.push_back(6);

    return i1;*/
}

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
        programBody = parse_body();
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
    inst = parse_stmt();

    t = peek();
    if(t.token_type == ID || t.token_type == WHILE || t.token_type == IF || t.token_type == SWITCH
       || t.token_type == FOR || t.token_type == OUTPUT || t.token_type == INPUT){
        instl = parse_stmt_list();
        inst->next = instl;
    }/*else{
        //set the tail of stmt_list to nullptr
        inst->next = nullptr;
    }*/
    return inst;
}

struct InstructionNode* parse_stmt(){
    struct InstructionNode *inst = nullptr;
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
            inst = parse_switch_stmt();
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
    Token t1 = lexer.GetToken();
    Token t2 = peek();
    lexer.UngetToken(t1);
    if (t1.token_type == ID || t1.token_type == NUM){
        if (t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV) {
            //expr
            struct InstructionNode* temp = parse_expr();
            assignInst->assign_inst.operand1_index = temp ->assign_inst.operand1_index;
            assignInst->assign_inst.op = temp->assign_inst.op;
            assignInst->assign_inst.operand2_index = temp->assign_inst.operand2_index;
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

InstructionNode* parse_output_stmt(){
    auto *outputInst = new InstructionNode;
    expect(OUTPUT);
    outputInst->type = OUT;
    t = expect(ID);
    outputInst->output_inst.var_index = inputVar[t.lexeme];
    expect(SEMICOLON);
    return outputInst;
}

InstructionNode* parse_input_stmt(){
    auto *inputInst = new InstructionNode;
    expect(INPUT);
    inputInst->type = IN;
    t = expect(ID);
    inputInst->input_inst.var_index = inputVar[t.lexeme];
    expect(SEMICOLON);
    return inputInst;
}

struct InstructionNode* parse_while_stmt(){
    auto *whileInst = new InstructionNode;
    expect(WHILE);
    whileInst->type = CJMP;
    struct InstructionNode* temp = parse_condition();
    whileInst->cjmp_inst.operand1_index = temp->cjmp_inst.operand1_index;
    whileInst->cjmp_inst.condition_op = temp->cjmp_inst.condition_op;
    whileInst->cjmp_inst.operand2_index = temp->cjmp_inst.operand2_index;

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

    jmp->next = noop;

    struct InstructionNode* getLast = whileInst;
    while(getLast->next != nullptr){
        getLast = getLast->next;
    }
    getLast->next = jmp;

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

InstructionNode * parse_switch_stmt(){
    auto* switchInst = new InstructionNode;
    expect(SWITCH);
    //switchInst->type = JMP;
    expect(ID);
    // some shit
    expect(LBRACE);
    //case_list
    t = peek();
    if(t.token_type == CASE){
        parse_case_list();
    } else{
        syntax_error();
    }
    //default_case
    t = peek();
    if(t.token_type == DEFAULT){
        parse_default_case();
        expect(RBRACE);
    } else if(t.token_type == RBRACE){
        t = lexer.GetToken();
        return switchInst;
    } else{
        syntax_error();
    }

}

struct InstructionNode * parse_for_stmt(){
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
    struct InstructionNode* temp = parse_condition();
    tempWhileInst->cjmp_inst.operand1_index = temp->cjmp_inst.operand1_index;
    tempWhileInst->cjmp_inst.condition_op = temp->cjmp_inst.condition_op;
    tempWhileInst->cjmp_inst.operand2_index = temp->cjmp_inst.operand2_index;

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

void parse_case_list(){
    //case
    t = peek();
    if(t.token_type == CASE){
        parse_case();
    } else{
        syntax_error();
    }
    //case_list
    t = peek();
    if(t.token_type == CASE){
        parse_case_list();
    }

}

void parse_case(){
    expect(CASE);
    expect(NUM);
    //some shit
    expect(COLON);
    //body
    t = peek();
    if (t.token_type == LBRACE){
        parse_body();
    } else{
        syntax_error();
    }
}

void parse_default_case(){
    expect(DEFAULT);
    expect(COLON);
    //body
    t = peek();
    if (t.token_type == LBRACE){
        parse_body();
    } else{
        syntax_error();
    }
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
    } else if(t.token_type == END_OF_FILE){
        //??
    } else{
        //syntax_error();
    }
}