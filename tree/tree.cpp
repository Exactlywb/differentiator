//
// Created by 1 on 02.12.2020.
//

#include "tree.h"

#define PRINT_ERROR(numError, message) if (numError != NoTreeError) { \
                                            printf ("Error %d : %s! In function %s!\n", numError, message, __FUNCTION__); \
                                        }

#define ASSERT_TREE if ( TreeVerificate (mainTree) != NoTreeError ) printf ("Error %d in function %s!\n", globalTreeErrno, __FUNCTION__);

Node* CreateNode  (ElementTree* value, char type, Node* left, Node* right) {

    if (type != CONST && type != VAR && type != FUNC) {

        PRINT_ERROR(WrongNodeType, "unexpected node type")
        return nullptr;

    }

    if (value == nullptr) {

        PRINT_ERROR(BadValuePointer, "null pointer on value")
        return nullptr;

    }

    Node* newNode = (Node*)calloc(1, sizeof(Node));

    if (newNode == nullptr) PRINT_ERROR(MemError, "can't make newNode")

    newNode->leftNode  = left;
    newNode->rightNode = right;

    newNode->type = type;

    if (type == CONST) {

        newNode->value.constValue = value->constValue;

    } else if (type == VAR) {

        newNode->value.var = value->var;

    } else {

        newNode->value.func = value->func;

    }

    return newNode;

}

Node* AddTreeRoot (Tree* mainTree, ElementTree* value, char type) {

    ASSERT_TREE

    if (value == nullptr) {

        PRINT_ERROR(BadValuePointer, "null pointer on value")
        return nullptr;

    }

    if (mainTree->root != nullptr) {

        PRINT_ERROR(RootAlreadyExist, "tree's root is already exist")
        return nullptr;

    }

    Node* newRoot = CreateNode (value, type, nullptr, nullptr);
    mainTree->root = newRoot;

    mainTree->size++;

    ASSERT_TREE

    return newRoot;

}

Node* AddNodeSide (Tree* mainTree, Node* parent, ElementTree* value, char type, int AddSide) {

    ASSERT_TREE
    //мб есть смысл поменять ASSERT OK и более легкие проверки местами. спроси ментора
    if (AddSide != LeftSide && AddSide != RightSide) {

        PRINT_ERROR(WrongAddSide, "unexpected addside")
        return nullptr;

    }

    if (value == nullptr) {

        PRINT_ERROR(BadValuePointer, "null pointer on value")
        return nullptr;

    }

    if (parent == nullptr) {

        PRINT_ERROR(NullPointerOnParent, "null pointer on parent")
        return nullptr;

    }

    if (AddSide == LeftSide && parent->leftNode != nullptr) {

        PRINT_ERROR(LeftNodeAlreadyExist, "left node is already exist")
        return nullptr;

    }

    if (AddSide == RightSide && parent->rightNode != nullptr) {

        PRINT_ERROR(RightNodeAlreadyExist, "left node is already exist")
        return nullptr;

    }

    Node* newNode = CreateNode (value, type, nullptr, nullptr);

    if (AddSide == LeftSide) {

        parent->leftNode = newNode;

    } else {

        parent->rightNode = newNode;

    }

    ASSERT_TREE

}

TreeErr DeleteSubTree (Tree* mainTree, Node* currentRoot) {

    if (mainTree == nullptr) {

        PRINT_ERROR(NullPointerOnTree, "null pointer on tree")
        return NullPointerOnTree;

    }

    if (currentRoot == nullptr) {

        PRINT_ERROR(NullPointerOnTree, "null pointer on currentRoot")
        return NullPointerOnTree;

    }

    Node* leftDown = currentRoot->leftNode;
    Node* rightDown = currentRoot->rightNode;

    if (leftDown != nullptr) DeleteSubTree (mainTree, leftDown);
    if (rightDown != nullptr) DeleteSubTree (mainTree, rightDown);

    mainTree->size--;

    free (currentRoot);

    return NoTreeError;

}

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~Функции копирования~~~~~~~~~~~~~~~~~~~~~~~~~~

Node* CopyNode (Node* originalNode) {

    if (originalNode == nullptr) {

        PRINT_ERROR(NullPointerOnNode, "null pointer on originalNode")
        return nullptr;

    }

    Node* newNode = (Node*)calloc(1, sizeof(Node));
    if (newNode == nullptr) {

        PRINT_ERROR(MemError, "memory error on newNode")
        return nullptr;

    }

    newNode->type = originalNode->type;

    switch (originalNode->type) {

        case FUNC:
            newNode->value.func = originalNode->value.func;
            break;
        case VAR:
            newNode->value.var = originalNode->value.var;
            break;
        case CONST:
            newNode->value.constValue = originalNode->value.constValue;
            break;
        default:
            PRINT_ERROR(WrongNodeType, "unexpected type of originalNode")
            free (newNode);
            return nullptr;

    }

    if (originalNode->leftNode != nullptr)
        newNode->leftNode = CopyNode (originalNode->leftNode);

    if (originalNode->rightNode != nullptr)
        newNode->rightNode = CopyNode (originalNode->rightNode);

    return newNode;

}

Node* CopyTree (Tree* mainTree, Tree* newTree, Node* currentNode) {

    if (newTree == nullptr) {

        PRINT_ERROR(NullPointerOnTree, "null pointer on newTree")
        return nullptr;

    }

    if (currentNode == nullptr)
        return nullptr;

    newTree->size = mainTree->size;

    Node* newNode = CopyNode (currentNode);

    if (currentNode == mainTree->root)
        newTree->root = newNode;

    newNode->leftNode  = CopyTree (mainTree, newTree, currentNode->leftNode);
    newNode->rightNode = CopyTree (mainTree, newTree, currentNode->rightNode);

    return newNode;

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~Дорогостоящие функции~~~~~~~~~~~~~~~~~~~~~~~~~

size_t CountSubTreeSize (Node* currentNode) {

    if (currentNode == nullptr) return 0;

    return 1 + CountSubTreeSize(currentNode->rightNode) + CountSubTreeSize(currentNode->leftNode);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

char* GetFuncSymbols (int FuncNumber) {

    char* OperationWrite = (char*)calloc(4, sizeof(char));

    switch (FuncNumber) {

        case ADD:
            OperationWrite = "+";
            break;
        case MUL:
            OperationWrite = "*";
            break;
        case SUB:
            OperationWrite = "-";
            break;
        case DIV:
            OperationWrite = "/";
            break;
        case SIN:
            OperationWrite = "sin";
            break;
        case COS:
            OperationWrite = "cos";
            break;
        case TAN:
            OperationWrite = "tg";
            break;
        case CTAN:
            OperationWrite = "ctg";
            break;
        case DEG:
            OperationWrite = "^";
            break;
        case LN:
            OperationWrite = "ln";
            break;
        default:
            PRINT_ERROR(UnexpectedFunc, "unexpected function")
            free (OperationWrite);
            return nullptr;

    }

    return OperationWrite;

}

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Защита~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TreeErr TreeVerificate (Tree* mainTree) {

    if (mainTree == nullptr)  {

        globalTreeErrno = NullPointerOnTree;
        return NullPointerOnTree;

    }

    size_t realTreeSize = CountSubTreeSize (mainTree->root);
    if (realTreeSize != mainTree->size) {

        printf ("True size is %zu, but in tree written %zu. ", realTreeSize, mainTree->size);
        globalTreeErrno = WrongTreeSize;
        return WrongTreeSize;

    }

    return NoTreeError;

}

void PrintNodeIntoGraphviz (Node* currentNode, FILE* output) {

    assert (output);
    if (currentNode == nullptr) return;

    static size_t index = 0;

    size_t currentIndex = index;

    if (currentNode->type == FUNC) {

        char* Operation = GetFuncSymbols (currentNode->value.func);

        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%s}\"]\n", index, Operation);

    }

    if (currentNode->type == VAR)
        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%c}\"]\n", index, currentNode->value.var);

    if (currentNode->type == CONST)
        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%lg}\"]\n", index, currentNode->value.constValue);

    index++;

    if (currentNode->leftNode != nullptr) {

        size_t nextIndex = index;
        PrintNodeIntoGraphviz (currentNode->leftNode, output);
        fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }

    if (currentNode->rightNode != nullptr) {

        size_t nextIndex = index;
        PrintNodeIntoGraphviz (currentNode->rightNode, output);
        fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }


}

void TreeDump (Tree* mainTree) {

    FILE* graph = fopen("tree.dot", "w");
    assert (graph);

    fprintf (graph, "digraph List {\n"
                    "node [shape=\"circle\"]\n");

    PrintNodeIntoGraphviz (mainTree->root, graph);
    fprintf (graph, "}\n");

    fclose (graph);

    system ("dot -Tpng tree.dot -o tree.png");

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Чтение~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int HandleFunc (char** startEquation, int commandSize, Node* currentNode, FuncList value) {

    *startEquation += commandSize;
    currentNode->value.func = value;
    currentNode->type = FUNC;

    return YesRes;

}

int HandleVar (char** startEquation, int commandSize, Node* currentNode, char varName) {

    *startEquation += commandSize;
    currentNode->value.var = varName;
    currentNode->type = VAR;

    return YesRes;

}

int HandleConst (char** startEquation, int commandSize, Node* currentNode, double value) {

    *startEquation += commandSize;
    currentNode->value.constValue = value;
    currentNode->type = CONST;

    return YesRes;

}

int VerificateCommands (char** startEquation, Node* currentNode) {

    char* endEquation = strpbrk (*startEquation, "()");

    int commandSize = endEquation - *startEquation;
    if (commandSize <= 0) return NoRes;

    if ( !strncmp("+", *startEquation, 1) )
        return HandleFunc (startEquation, commandSize, currentNode, ADD);

    if ( !strncmp("-", *startEquation, 1) )
        return HandleFunc (startEquation, commandSize, currentNode, SUB);

    if ( !strncmp("*", *startEquation, 1) )
        return HandleFunc (startEquation, commandSize, currentNode, MUL);

    if ( !strncmp("/", *startEquation, 1) )
        return HandleFunc (startEquation, commandSize, currentNode, DIV);

    if ( !strncmp("sin", *startEquation, 3) )
        return HandleFunc (startEquation, commandSize, currentNode, SIN);

    if ( !strncmp("ln", *startEquation, 2) )
        return HandleFunc (startEquation, commandSize, currentNode, LN);

    if ( !strncmp("cos", *startEquation, 3) )
        return HandleFunc (startEquation, commandSize, currentNode, COS);

    if ( !strncmp("tg", *startEquation, 2) )
        return HandleFunc (startEquation, commandSize, currentNode, TAN);

    if ( !strncmp("ctg", *startEquation, 3) )
        return HandleFunc (startEquation, commandSize, currentNode, CTAN);

    if ( !strncmp("^", *startEquation, 1) )
        return HandleFunc (startEquation, commandSize, currentNode, DEG);

    if (isalpha(**startEquation))
        return HandleVar (startEquation, commandSize, currentNode, **startEquation);

    if (isdigit(**startEquation))
        return HandleConst (startEquation, commandSize, currentNode, atof(*startEquation));

    return NoRes;

}

Node* SetTree (char** startEquation, Node* parent, Tree* mainTree) {

    assert (startEquation);
    assert (mainTree);

    if (**startEquation != '(')
        return nullptr;

    (*startEquation)++;

    Node* currentNode = (Node*)calloc (1, sizeof(Node));
    Node* right = nullptr;
    Node* left  = nullptr;

    if (**startEquation == '(')
        left = SetTree (startEquation, currentNode, mainTree);

    if (VerificateCommands (startEquation, currentNode) == NoRes)
        return nullptr;

    if (**startEquation == '(')
        right = SetTree (startEquation, currentNode, mainTree);

    if (**startEquation != ')')
        return nullptr;

    (*startEquation)++;

    currentNode->leftNode = left;
    currentNode->rightNode = right;

    mainTree->size++;

    return currentNode;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

//========================================================================
//~~~~~~~~~~~~~~~~~Создание узлов с определенными функциями~~~~~~~~~~~~~~~

Node* MakeConstNode (double value, Node* left, Node* right) {

    ElementTree newElem = {};
    newElem.constValue = value;

    return CreateNode (&newElem, CONST, left, right);

}

Node* MakeVarNode (char var, Node* left, Node* right) {

    ElementTree newElem = {};
    newElem.var = var;

    return CreateNode (&newElem, VAR, left, right);

}

Node* MakeFuncNode (FuncList funcName, Node* left, Node* right) {

    ElementTree newElem = {};
    newElem.func = funcName;

    return CreateNode(&newElem, FUNC, left, right);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================
