//
// Created by 1 on 02.12.2020.
//

#ifndef DIFFERENTIATOR_TREE_H
#define DIFFERENTIATOR_TREE_H

#endif //DIFFERENTIATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <cctype>

enum FuncList {

    ADD,
    SUB,
    MUL,
    DIV,
    SIN,
    COS,
    TAN,
    CTAN,
    DEG,
    LN

};

enum FuncType {

    CONST,
    VAR,
    FUNC

};

enum SideEnum {

    LeftSide,
    RightSide

};

enum TreeErr {

    NoTreeError,
    NullPointerOnTree,
    NullPointerOnParent,
    NullPointerOnNode,
    WrongNodeType,
    WrongFuncType,
    MemError,
    WrongStartRead,
    WrongTreeSize,
    BadValuePointer,
    LeftNodeAlreadyExist,
    RightNodeAlreadyExist,
    RootAlreadyExist,
    UnexpectedFunc,
    WrongAddSide

};

enum VerificateErr {

    NoRes,
    YesRes

};

static size_t globalTreeErrno = NoTreeError;

union nodeValueType {

    double constValue;
    char var;
    FuncList func;

};

typedef union nodeValueType ElementTree;

struct Node {

    Node* leftNode  = nullptr;
    Node* rightNode = nullptr;

    ElementTree value = {};

    char type = CONST;

};

struct Tree {

    Node* root = nullptr;

    size_t size = 0;

};

Node*   CreateNode              (ElementTree* value, char type, Node* left, Node* right);
Node*   AddTreeRoot             (Tree* mainTree, ElementTree* value, char type);
Node*   AddNodeSide             (Tree* mainTree, Node* parent, ElementTree* value, char type, int AddSide);
TreeErr DeleteSubTree           (Tree* mainTree, Node* currentRoot);
TreeErr TreeVerificate          (Tree* mainTree);
size_t  CountSubTreeSize        (Node* currentNode);
void    TreeDump                (Tree* mainTree);
void    PrintNodeIntoGraphviz   (Node* currentNode, FILE* output);
Node*   SetTree                 (char** startEquation, Node* parent, Tree* mainTree);
Node*   CopyTree                (Tree* mainTree, Tree* newTree, Node* currentNode);
Node*   CopyNode                (Node* originalNode);
Node*   DeleteNode              (Node* currentNode);


Node* MakeConstNode (double value, Node* left, Node* right);

Node* MakeVarNode (char var, Node* left, Node* right);

Node* MakeFuncNode (FuncList funcName, Node* left, Node* right);
