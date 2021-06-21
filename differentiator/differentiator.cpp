//
// Created by 1 on 02.12.2020.
//

#include "differentiator.h"


#define PRINT_ERROR(numError, message) if (numError != NoTreeError) { \
                                            printf ("Error %d : %s! In function %s!\n", numError, message, __FUNCTION__); \
                                        }

void MainDifferentiator () {

    setlocale (LC_ALL, "Russian");

    FILE* writeInto = fopen ("../data/LaTex/result.tex", "w");
    SetPreambulIntoLaTex(writeInto);

    Tree mainTree = {};

    //char* expression = "(sin(x))^(ctg(x))*x^x+cos(x)$";//из командной строки.
    char* expression = (char*)calloc (MAX_EXPRESSION_SIZE, sizeof(char));
    scanf ("%s", expression);
    expression[strlen(expression)] = '$';

    mainTree.root = ReadExpression (expression);
    assert (mainTree.root);

    free (expression);

    Tree newTree = {};

    CopyTree (&mainTree, &newTree, mainTree.root);

    TexPrintPhrase (writeInto, FIRST_EQUATION);

    AutoTreeTexPrint (mainTree.root, writeInto, 0);

    newTree.root = DifferentiateTree (newTree.root, writeInto);

    TexPrintPhrase (writeInto, END_EQUATION);

    Optimizer (&newTree, newTree.root);

    AutoTreeTexPrint (newTree.root, writeInto, 0);

    AutoReplaces (writeInto, &newTree);

    SetEndIntoLaTex (writeInto);

    TreeDump (&newTree);
    fclose (writeInto);

    StartTex ();

    DeleteSubTree (&mainTree, mainTree.root);
    DeleteSubTree (&newTree, newTree.root);

}

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LaTex~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CHECK_FILE_POINTER(name) if (name == nullptr) { \
                                                        \
                                    printf ("Null pointer on FILE* %s! Error in function %s!\n", #name, __FUNCTION__); \
                                    return;             \
                                                        \
                                }

#define PR_R_BRACK fprintf (writeTexInto, " \\big( ");
#define PR_L_BRACK fprintf (writeTexInto, " \\big) ");

#define PR_L_TREE  PrintTreeTex (currentNode->leftNode, writeTexInto);
#define PR_R_TREE  PrintTreeTex (currentNode->rightNode, writeTexInto);

void DoLaTex (Node* currentNode) {

    FILE* writeLaTex = fopen ("../data/LaTex/result.tex", "w");

    CHECK_FILE_POINTER(writeLaTex)

    SetPreambulIntoLaTex (writeLaTex);
    PrintTreeTex (currentNode, writeLaTex);
    SetEndIntoLaTex (writeLaTex);

    fclose (writeLaTex);

    StartTex ();

}

void SetPreambulIntoLaTex (FILE* writeTexInto) {

    CHECK_FILE_POINTER(writeTexInto)

    fprintf (writeTexInto, "%s", PreambulaLaTex);

}

void SetEndIntoLaTex (FILE* writeTexInto) {

    CHECK_FILE_POINTER(writeTexInto)

    fprintf (writeTexInto, "\n");
    fprintf (writeTexInto, "%s", EndLaTexDocument);

}

void StartTex () {

    system ("latex ../data/LaTex/result.tex Document Class: article 2020/04/10 v1.4m Standard LaTeX document class");
    system ("dvips result.dvi");
    system ("ps2pdf result.ps");
    system ("start result.pdf");

}

void PrintTreeTex (Node* currentNode, FILE* writeTexInto) {

    CHECK_FILE_POINTER(writeTexInto)

    if (currentNode == nullptr) {

        printf ("Null pointer on currentNode in function %s!\n", __FUNCTION__);
        return;

    }

    switch (currentNode->type) {

        case CONST:
            fprintf (writeTexInto, "%lg", currentNode->value.constValue);
            break;
        case VAR:
            fprintf (writeTexInto, "%c", currentNode->value.var);
            break;
        case FUNC:
            switch (currentNode->value.func) {

                case ADD:
                    PR_R_BRACK
                    PR_L_TREE
                    fprintf (writeTexInto, " + ");
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case SUB:
                    PR_R_BRACK
                    PR_L_TREE
                    fprintf (writeTexInto, " - ");
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case MUL:
                    PR_L_TREE
                    fprintf (writeTexInto, " \\cdot ");
                    PR_R_TREE
                    break;
                case DIV:
                    fprintf (writeTexInto, " \\frac{");
                    PR_L_TREE
                    fprintf (writeTexInto, " }{ ");
                    PR_R_TREE
                    fprintf (writeTexInto, " } ");
                    break;
                case DEG: //шиворот навыворот замены. Поменяй вывод
                    PR_R_BRACK
                    PR_L_TREE
                    PR_L_BRACK
                    fprintf (writeTexInto, "^{");
                    PR_R_TREE
                    fprintf (writeTexInto, "}");
                    break;
                case LN:
                    fprintf (writeTexInto, "\\ln");
                    PR_R_BRACK
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case SIN:
                    fprintf (writeTexInto, "\\sin");
                    PR_R_BRACK
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case COS:
                    fprintf (writeTexInto, "\\cos");
                    PR_R_BRACK
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case TAN:
                    fprintf (writeTexInto, "\\tan");
                    PR_R_BRACK
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                case CTAN:
                    fprintf (writeTexInto, "\\cot");
                    PR_R_BRACK
                    PR_R_TREE
                    PR_L_BRACK
                    break;
                default:
                    PRINT_ERROR(WrongFuncType, "unexpected func type of currentNode")
                    return;

            }

            break;
        default:
            PRINT_ERROR(WrongNodeType, "unexpected node type of currentNode")
            return;

    }

}

void TexStartNewFormula (FILE* writeInto) {

    CHECK_FILE_POINTER(writeInto)

    fprintf (writeInto, "\n\n\\begin{center}"
                        "\n\\begin{math}\n");

}

void TexEndFormula (FILE* writeInto) {

    CHECK_FILE_POINTER(writeInto)

    fprintf (writeInto, "\n\\end{math}\n"
                        "\\end{center}\n\n");

}

void AutoTreeTexPrint (Node* startNode, FILE* writeTexInto, char treeName) {

    if (startNode == nullptr)
        return;

    if (writeTexInto == nullptr)
        return;

    TexStartNewFormula (writeTexInto);

    if (treeName != 0)
        fprintf (writeTexInto, "\\text{%c = }", treeName);

    PrintTreeTex (startNode, writeTexInto);
    TexEndFormula (writeTexInto);

}

void TexPrintPhrase (FILE* writeInto, PhraseChoice phraseNum) {

    CHECK_FILE_POINTER(writeInto)

    switch (phraseNum) {

        case FIRST_EQUATION:
            fprintf (writeInto, "\\text{Дважды продифференцируем и единожды проинтегрируем, забыв константу.}\n"
                                "\n\\text{Не мало бойцов полегло на этой константе... Һаулыҡ вафат булғандар}");
            break;
        case EVERYBODY_KNOWS:
            fprintf (writeInto, "\\text{Любой башкирский малай знает, что дифференцирование "
                                "~---~ известная техника счета древних рустарҙы}");
            break;
        case SALAM:
            fprintf (writeInto, "\\text{ҺАУМЫ, ДИФФЕРЕНЦИАЛ БУДЕТ}");
            break;
        case SINUS_FOCUS:
            fprintf (writeInto, "\\text{Синус факиус}");
            break;
        case COSINUS_FOCUS:
            fprintf (writeInto, "\\text{Косинус чпокиус}");
            break;
        case END_EQUATION:
            fprintf (writeInto, "\\text{В конце получаем абсолютно очевидный результат. }\n\n"
                                "\\text{Улар өсөн яндырылған ауылдар, ҡалалар. }\n\n"
                                "\\text{Күп кенә батыр уның өсөн барыла, машиналар ҙа ғына етә.}\n\n");
            break;
        case TRANSFORM:
            fprintf (writeInto, "\\text{Под действием семи ветров точится камень "
                                " и дифференцируется недифференцируемое}");
            break;
        case START_TAN:
            fprintf (writeInto, "\\text{Наш тангенс ~---~ очень страшная функция из трех букв}");
            break;
        case START_CTAN:
            fprintf (writeInto, "\\text{Разрешите представиться, КОТангенс!}");
            break;
        case EXPENSIVE:
            fprintf (writeInto, "\\text{Дорогостоящие исследования показали, что}");
            break;
        case DEG_PHRASE:
            fprintf (writeInto, "\\text{Дифференцируй, дифференцируй, мы же миллионеры. Ещё функций придумаем}");
        default:
            break;

    }

    fprintf (writeInto, "\n\n");

}

void PrintFormula (FILE* writeInto, Node* currentNode) {

    CHECK_FILE_POINTER(writeInto)
    if (currentNode == nullptr) {

        printf ("Null pointer on currentNode in function %s!\n", __FUNCTION__);
        return;

    }

    TexStartNewFormula (writeInto);
    PrintTreeTex (currentNode, writeInto);
    TexEndFormula (writeInto);

}

void AutoReplaces (FILE* writeInto, Tree* newTree) {

    bool isReplace = false;

    for (size_t replaceNum = 0; replaceNum < ReplaceCount; replaceNum++) {

        Tree replacement = {};
        if ( DoReplacement (newTree, newTree->root, &replacement, 'A' + replaceNum) ) {

            isReplace = true;

            if (replaceNum == 0)
                fprintf (writeInto, "Провернем замену, где");

            AutoTreeTexPrint (replacement.root, writeInto, 'A' + replaceNum);
            DeleteSubTree (&replacement, replacement.root);

        }

    }

    if (isReplace) {

        fprintf(writeInto, "И получим, что");
        AutoTreeTexPrint (newTree->root, writeInto, 0);

    }

}

#undef CHECK_FILE_POINTER

#undef PR_R_BRACK
#undef PR_L_BRACK

#undef PR_L_TREE
#undef PR_R_TREE

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Оптимизации~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define VALUE currentNode->value
#define TYPE currentNode->type

#define RIGHT_VALUE currentNode->rightNode->value
#define LEFT_VALUE  currentNode->leftNode->value

#define RIGHT_TYPE currentNode->rightNode->type
#define LEFT_TYPE  currentNode->leftNode->type

bool MulOptimizer (Tree* mainTree, Node* currentNode) {

    if (mainTree == nullptr) {

        printf ("null pointer on mainTree in function %s!\n", __FUNCTION__);
        return 0;

    }

    if ( currentNode == nullptr ) {

        printf ("Nullptr on currentNode in function %s!\n", __FUNCTION__);

    }

    if (TYPE == FUNC && ( VALUE.func == MUL || VALUE.func == DIV) ) {

        if ( (RIGHT_TYPE == CONST && RIGHT_VALUE.constValue == 0) || (LEFT_TYPE == CONST && LEFT_VALUE.constValue == 0) ) {

            TYPE = CONST;
            VALUE.constValue = 0;

            DeleteSubTree (mainTree, currentNode->leftNode);
            DeleteSubTree (mainTree, currentNode->rightNode);

            currentNode->leftNode = nullptr;
            currentNode->rightNode = nullptr;

            return true;

        }

        if ( RIGHT_TYPE == CONST && RIGHT_VALUE.constValue == 1 ) {

            TYPE = LEFT_TYPE;

            switch (TYPE) {

                case CONST:
                    VALUE.constValue = LEFT_VALUE.constValue;
                    break;
                case VAR:
                    VALUE.var = LEFT_VALUE.var;
                    break;
                case FUNC:
                    VALUE.func = LEFT_VALUE.func;
                    break;
                default:
                    printf ("Unexpected left node type in function %s\n", __FUNCTION__ );
                    return false;

            }

            Node* oldRightNode = currentNode->rightNode;
            Node* oldLeftNode  = currentNode->leftNode;

            currentNode->leftNode = oldLeftNode->leftNode;
            currentNode->rightNode = oldLeftNode->rightNode;

            free (oldRightNode);
            free (oldLeftNode);

            return true;

        }

        if ( LEFT_TYPE == CONST && LEFT_VALUE.constValue == 1 && VALUE.func != DIV) {

            TYPE = RIGHT_TYPE;

            switch (TYPE) {

                case CONST:
                    VALUE.constValue = RIGHT_VALUE.constValue;
                    break;
                case VAR:
                    VALUE.var = RIGHT_VALUE.var;
                    break;
                case FUNC:
                    VALUE.func = RIGHT_VALUE.func;
                    break;
                default:
                    printf ("Unexpected left node type in function %s\n", __FUNCTION__ );
                    return false;

            }

            Node* oldRightNode = currentNode->rightNode;
            Node* oldLeftNode  = currentNode->leftNode;

            currentNode->leftNode = oldRightNode->leftNode;
            currentNode->rightNode = oldRightNode->rightNode;

            free (oldRightNode);
            free (oldLeftNode);

            return true;

        }

    }

    return false;

}

bool AddOptimizer (Tree* mainTree, Node* currentNode) {

    if ( mainTree == nullptr ) {

        printf ("nullpointer on mainTree in function %s!\n", __FUNCTION__);
        return false;

    }

    if ( currentNode == nullptr ) {

        printf ("Nullptr on currentNode in function %s!\n", __FUNCTION__);

    }

    if ( TYPE == FUNC && VALUE.func == ADD ) { //sub

        if ( LEFT_TYPE == CONST && LEFT_VALUE.constValue == 0 ) {

            TYPE = RIGHT_TYPE;

            switch (TYPE) {

                case CONST:
                    VALUE.constValue = RIGHT_VALUE.constValue;
                    break;
                case VAR:
                    VALUE.var = RIGHT_VALUE.var;
                    break;
                case FUNC:
                    VALUE.func = RIGHT_VALUE.func;
                    break;
                default:
                    printf ("Unexpected right node type in function %s\n", __FUNCTION__ );
                    return false;

            }

            Node* oldRightNode = currentNode->rightNode;
            Node* oldLeftNode  = currentNode->leftNode;

            currentNode->leftNode = oldRightNode->leftNode;
            currentNode->rightNode = oldRightNode->rightNode;

            free (oldRightNode);
            free (oldLeftNode);

            return true;

        }

        if ( RIGHT_TYPE == CONST && RIGHT_VALUE.constValue == 0 ) {

            TYPE = LEFT_TYPE;

            switch (TYPE) {

                case CONST:
                    VALUE.constValue = LEFT_VALUE.constValue;
                    break;
                case VAR:
                    VALUE.var = LEFT_VALUE.var;
                    break;
                case FUNC:
                    VALUE.func = LEFT_VALUE.func;
                    break;
                default:
                    printf ("Unexpected left node type in function %s\n", __FUNCTION__ );
                    return false;

            }

            Node* oldRightNode = currentNode->rightNode;
            Node* oldLeftNode  = currentNode->leftNode;

            currentNode->leftNode = oldLeftNode->leftNode;
            currentNode->rightNode = oldLeftNode->rightNode;

            free (oldRightNode);
            free (oldLeftNode);

            return true;

        }

    }

    return false;

}

Node* FindNodeToReplace (Node* startResearches, size_t currentDepth) {

    if (startResearches == nullptr)
        return nullptr;

    if (currentDepth >= MinDepthToReplace) {

        size_t subTreeSize = CountSubTreeSize (startResearches);

        if (subTreeSize >= MinSubTreeSizeToReplace)
            return startResearches;
        else
            return nullptr;

    }

    Node* leftSearch  = FindNodeToReplace (startResearches->leftNode, currentDepth + 1);
    Node* rightSearch = FindNodeToReplace (startResearches->rightNode, currentDepth + 1);

    if ( leftSearch )
        return leftSearch;

    if ( rightSearch )
        return rightSearch;

    return nullptr;

}

bool DoReplacement (Tree* mainTree, Node* currentNode, Tree* newTree, char symb) {

    Node* lastNodeAfterDfs = nullptr;

    if ( lastNodeAfterDfs = FindNodeToReplace(currentNode, 0) ) {

        newTree->root = CopyNode( lastNodeAfterDfs );

        DeleteSubTree (mainTree, lastNodeAfterDfs->leftNode);
        DeleteSubTree (mainTree, lastNodeAfterDfs->rightNode);

        lastNodeAfterDfs->leftNode = lastNodeAfterDfs->rightNode = nullptr;
        lastNodeAfterDfs->type = VAR;
        lastNodeAfterDfs->value.var = symb;

        return true;

    }

    return false;

}

size_t Optimizer (Tree* mainTree, Node* currentNode) {

    if (mainTree == nullptr) {

        printf ("null pointer on mainTree in function %s!\n", __FUNCTION__);
        return 0;

    }

    if ( currentNode == nullptr )
        return 0;

    size_t changeCount = 0;

    changeCount += Optimizer (mainTree, currentNode->rightNode);
    changeCount += Optimizer (mainTree, currentNode->leftNode);

    if (MulOptimizer(mainTree, currentNode)) changeCount++;

    if (AddOptimizer(mainTree, currentNode)) changeCount++;

    return changeCount;

}

#undef RIGHT_VALUE
#undef LEFT_VALUE

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~Дифференцирование~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define CHECK_CURR_NODE if (currentNode == nullptr) { \
                                                      \
                            PRINT_ERROR(NullPointerOnNode, "null pointer on currentNode") \
                            return nullptr;\
                                                      \
                        }

Node* DifferentiateTree (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    switch (currentNode->type) {

        case CONST:
            return MakeConstNode (0, nullptr, nullptr);
        case VAR:
            return MakeConstNode(1, nullptr, nullptr);
        case FUNC:

            switch (currentNode->value.func) {

                case ADD:
                    return AddDiff (currentNode, writeInto);
                case SUB:
                    return SubDiff (currentNode, writeInto);
                case MUL:
                    return MulDiff (currentNode, writeInto);
                case DIV:
                    return DivDiff (currentNode, writeInto);
                case SIN:
                    return SinDiff (currentNode, writeInto);
                case COS:
                    return CosDiff (currentNode, writeInto);
                case TAN:
                    return TanDiff (currentNode, writeInto);
                case CTAN:
                    return CTanDiff (currentNode, writeInto);
                case DEG:
                    return DegDiff (currentNode, writeInto);
                case LN:
                    return LnDiff (currentNode, writeInto);
                default:
                    PRINT_ERROR(WrongFuncType, "unexpected func type of currentNode")
                    return nullptr;

            }

        default:
            PRINT_ERROR(WrongNodeType, "unexpected currentNode type")
            return nullptr;

    }

}

#define y  CopyNode (currentNode)

#define cL CopyNode (currentNode->leftNode)
#define cR CopyNode (currentNode->rightNode)

#define dL DifferentiateTree (cL, writeInto)
#define dR DifferentiateTree (cR, writeInto)

#define MAKE_MUL(left, right) MakeFuncNode  (MUL, left, right)
#define MAKE_ADD(left, right) MakeFuncNode  (ADD, left, right)
#define MAKE_DIV(left, right) MakeFuncNode  (DIV, left, right)
#define MAKE_SUB(left, right) MakeFuncNode  (SUB, left, right)
#define MAKE_SIN(value)       MakeFuncNode  (SIN, nullptr, value)
#define MAKE_COS(value)       MakeFuncNode  (COS, nullptr, value)
#define MAKE_DEG(left, right) MakeFuncNode  (DEG, left, right)
#define MAKE_LN(right)        MakeFuncNode  (LN,  nullptr, right)

#define MAKE_CONST(value)     MakeConstNode (value, nullptr, nullptr)

#define COMPOSITION_FUNC(difExternalFunc, internalFunc) MAKE_MUL(difExternalFunc, DifferentiateTree(internalFunc, writeInto))

Node* AddDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    Node* result = MakeFuncNode (ADD, dL, dR);

    return result;

}

Node* DegDiff (Node* currentNode, FILE* writeInto) { //дописать

    CHECK_CURR_NODE

    PrintFormula (writeInto, currentNode);
    TexPrintPhrase (writeInto, DEG_PHRASE);

    Node* result = nullptr;

    if (cR->type == CONST) {

        Node* difExternalFunc = MAKE_DEG(cL, MAKE_CONST(cR->value.constValue - 1));
        difExternalFunc = MAKE_MUL(MAKE_CONST(cR->value.constValue), difExternalFunc);

        result = COMPOSITION_FUNC(difExternalFunc, cL);

    } else {

        //тут рассматривается случай (f^g)'
        //в действительности, производная от такой функции является
        //(e^(g * ln(f)))'
        //f^g * ( g' * ln(f) + g * [1/f * f'] )
        //leftBrackNode     - у нас g' * ln(f)
        //rightBrackNode    - у нас g * [1/f * f']
        //subRightBrackNode - у нас 1/f * f'
        //brackNode         - сама скобка ( g' * ln(f) + g * [1/f * f'] )

        Node* leftBrackNode     = MAKE_MUL(dR, MAKE_LN(cL));
        Node* subRightBrackNode = MAKE_MUL(MAKE_DIV(MAKE_CONST(1), cL), dL);
        Node* rightBrackNode    = MAKE_MUL(cR, subRightBrackNode);

        Node* brackNode         = MAKE_ADD(leftBrackNode, rightBrackNode);

        result                  = MAKE_MUL(y, brackNode);

    }

    return result;

}

Node* SubDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    PrintFormula (writeInto, currentNode);

    return MAKE_SUB(dL, dR);

}

Node* MulDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    return MAKE_ADD(MAKE_MUL(cL, dR), MAKE_MUL(dL, cR));

}

Node* DivDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    PrintFormula (writeInto, currentNode);

    return MAKE_DIV(MAKE_SUB(MAKE_MUL(dL, cR), MAKE_MUL(cL, dR)), MAKE_MUL(cR, cR));

}

Node* SinDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    TexPrintPhrase (writeInto, SINUS_FOCUS);
    PrintFormula   (writeInto, currentNode);

    Node* result = COMPOSITION_FUNC(MAKE_COS(cR), cR);

    PrintFormula (writeInto, result);

    return result;

}

Node* CosDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    TexPrintPhrase (writeInto, COSINUS_FOCUS);
    PrintFormula (writeInto, currentNode);

    Node* difExternalFunc = MAKE_MUL(MAKE_CONST(-1), MAKE_SIN(cR));
    Node* result = COMPOSITION_FUNC(difExternalFunc, cR);

    TexPrintPhrase (writeInto, TRANSFORM);
    PrintFormula (writeInto, result);

    return result;

}

Node* TanDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    TexPrintPhrase (writeInto, START_TAN);
    PrintFormula (writeInto, currentNode);

    Node* difExternalFunc = MAKE_DIV(MAKE_CONST(1), MAKE_MUL(MAKE_COS(cR), MAKE_COS(cR)));
    Node* result = COMPOSITION_FUNC(difExternalFunc, cR);

    TexPrintPhrase (writeInto, TRANSFORM);
    PrintFormula (writeInto, result);

    return result;

}

Node* CTanDiff (Node* currentNode, FILE* writeInto) {

    CHECK_CURR_NODE

    TexPrintPhrase (writeInto, START_CTAN);
    PrintFormula (writeInto, currentNode);

    Node* difExternalFunc = MAKE_DIV(MAKE_CONST(-1), MAKE_MUL(MAKE_SIN(cR), MAKE_SIN(cR)));
    Node* result = COMPOSITION_FUNC(difExternalFunc, cR);

    TexPrintPhrase (writeInto, TRANSFORM);
    PrintFormula (writeInto, result);

    return result;

}

Node* LnDiff (Node* currentNode, FILE* writeInto) {

    TexPrintPhrase (writeInto, EXPENSIVE);

    Node* difExternalFunc = MAKE_DIV(MAKE_CONST(1), cR);
    Node* result = COMPOSITION_FUNC(difExternalFunc, cR);

    return result;

}

#undef COMPOSITION_FUNC

#undef MAKE_COS
#undef MAKE_SIN
#undef MAKE_DIV
#undef MAKE_ADD
#undef MAKE_MUL

#undef cL
#undef cR

#undef dL
#undef dR

#undef y

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================

#undef PRINT_ERROR


//========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~Рекурсивный спуск~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PRINT_ERR_IN_FUNC(message) printf ("%s in function %s", message, __FUNCTION__);

char* originalExpression = nullptr;
size_t symbolPosition    = 0;

Node* ReadExpression (char* expression) {

    if (expression == nullptr) {

        PRINT_ERR_IN_FUNC("Null pointer on expression")
        return nullptr;

    }

    originalExpression = expression;

    symbolPosition = 0;

    Node* root = GetAdd ();

    if (root == nullptr)
        PRINT_ERR_IN_FUNC("Bad format of expression")

    Require ('$', __LINE__);

    return root;

}

Node* GetAdd () {

    Node* firstNode = GetMul ();

    while (originalExpression [symbolPosition] == '+' || originalExpression[symbolPosition] == '-') {

        char operation = originalExpression[symbolPosition];

        symbolPosition++;

        Node* secondNode = GetMul ();

        if (operation == '+')
            firstNode = MakeFuncNode (ADD, firstNode, secondNode);
        else
            firstNode = MakeFuncNode (SUB, firstNode, secondNode);

    }

    return firstNode;

}

Node* GetMul () {

    Node* resultNode = GetDeg ();

    while (originalExpression[symbolPosition] == '*' || originalExpression[symbolPosition] == '/') {

        char operation = originalExpression[symbolPosition];
        symbolPosition++;

        Node* secondNode = GetDeg ();

        if (operation == '*')
            resultNode =  MakeFuncNode (MUL, resultNode, secondNode);
        else
            resultNode =  MakeFuncNode (DIV, resultNode, secondNode);

    }

    return resultNode;

}

Node* GetDeg () {

    Node* resultNode = GetBrack ();

    while (originalExpression[symbolPosition] == '^') {

        symbolPosition++;

        Node* secondNode = GetBrack ();

        resultNode =  MakeFuncNode (DEG, resultNode, secondNode);

    }

    return resultNode;

}

Node* GetBrack () {

    if (originalExpression[symbolPosition] == '(') {

        symbolPosition++;
        Node* firstNode = GetAdd ();

        Require (')', __LINE__);

        return firstNode;

    } else {

        return GetSin ();

    }

}

Node* GetNum () {

    double value = 0;

    if (originalExpression[symbolPosition] == 'x') {

        symbolPosition++;
        return MakeVarNode('x', nullptr, nullptr);

    }

    while ( isdigit(originalExpression[symbolPosition]) ) {

        char addValue = originalExpression[symbolPosition] - '0';

        value = value * 10 + addValue;
        symbolPosition++;

    }

    return MakeConstNode (value, nullptr, nullptr);

}

Node* GetSin () {

    if ( !strncmp("sin", originalExpression + symbolPosition, 3) ) {

        symbolPosition += 3;

        Node* valueNode = GetBrack ();
        return MakeFuncNode (SIN, nullptr, valueNode);

    }

    return GetCos ();

}

Node* GetCos () {

    if ( !strncmp("cos", originalExpression + symbolPosition, 3) ) {

        symbolPosition += 3;

        Node* valueNode = GetBrack ();
        return MakeFuncNode (COS, nullptr, valueNode);

    }

    return GetTan ();

}

Node* GetTan () {

    if ( !strncmp("tg", originalExpression + symbolPosition, 2) ) {

        symbolPosition += 2;

        Node* valueNode = GetBrack ();
        return MakeFuncNode (TAN, nullptr, valueNode);

    }

    return GetCotan ();

}

Node* GetCotan () {

    if ( !strncmp("ctg", originalExpression + symbolPosition, 3) ) {

        symbolPosition += 3;

        Node* valueNode = GetBrack ();
        return MakeFuncNode (CTAN, nullptr, valueNode);

    }

    return GetLn ();

}

Node* GetLn () {

    if ( !strncmp("ln", originalExpression + symbolPosition, 2) ) {

        symbolPosition += 2;

        Node* valueNode = GetBrack ();
        return MakeFuncNode (LN, nullptr, valueNode);

    }

    return GetNum ();

}

char Require (const char symbToRequire, int line) {

    char lastSymbol = originalExpression[symbolPosition];

    if (lastSymbol != symbToRequire) {
        printf ("On line %d lost %c\n", line, lastSymbol);
        SyntaxError(__FUNCTION__);
    }
    else
        symbolPosition++;

    return lastSymbol;

}

void SyntaxError (const char* funcName) {

    printf ("Syntax error! From %s function. \n", funcName);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//========================================================================
