//
// Created by 1 on 02.12.2020.
//

#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <stdlib.h>
#include <stdio.h>
#include "../tree/tree.h"
#include <clocale>

const size_t MinSubTreeSizeToReplace = 10;
const size_t MinDepthToReplace       = 3;
const size_t ReplaceCount            = 2;

const size_t MAX_EXPRESSION_SIZE = 50;

static const char* PreambulaLaTex = "\\documentclass[a4paper,12pt]{article}\n"
                             "\\usepackage{cmap}\n"
                             "\\usepackage[T2A]{fontenc}\n"
                             "\\usepackage[utf8]{inputenc}\n"
                             "\\usepackage[english, russian]{babel}\n"
                             "\\usepackage{mathtext}\n"
                             "\\usepackage{amsmath, amsfonts,amssymb,amsthm,mathtools}\n"
                             "\\usepackage{textcomp}\n"
                             "\\usepackage{euscript}\n"
                             "\\usepackage{xcolor}\n"
                             "\\usepackage{hyperref}\n"
                             "\\usepackage[normalem]{ulem}\n"
                             "\\definecolor{urlcolor}{HTML}{799B03}\n"
                             "\\addto\\captionsrussian{\\def\\refname{Сығанаҡтар исемлеге}}\n"
                             "\\usepackage[left=2cm,right=2cm,\n"
                             "top=2cm,bottom=2cm,bindingoffset=0cm]{geometry}\n"
                             "\\newcommand*{\\hm}[1]{#1\\nobreak\\discretionary{}\n"
                             "{\\hbox{$\\mathsurround=0pt #1$}}{}}\n"
                             "\\setlength{\\parindent}{0ex}\n"
                             "\\author{Prod. by Robert b. Weide}\n"
                             "\\title{Дифференциальные загадки башкирских лесов}\n"
                             "\\date{2020 до н.э.}\n"
                             "\\begin{document}\n"
                             "\\maketitle\n"
                             "\\section{Урал Батыр. Тәүге шиғырҙар.}"
                             "\n\nДавным давно, когда ещё Деды писали на ДОСе, "
                             "\n\nКогда башкирские малаи с коня ещё не попадали в десяточку Беклемишева, "
                             "\n\nКогда на свете был лишь один РТ, "
                             "\n\nСказал Кудрявцев <<Да будет свет>>. И создал дифференциатор, "
                             "\n\nИ увидел он, что \\sout{с пивом пойдет} это хорошо, "
                             "\n\nИ отделил Останин Пепси от Колы, "
                             "\n\nИ увидел Кудрявцев, что это хорошо, (1)"
                             "\n\nИ провели кабель электронов, "
                             "\n\nИ отрезал для себя МФТИ Телеком самый маленький кусочек, "
                             "\n\nИ раздал он этот кусочек на все общаги Долгопрудного, "
                             "\n\nИ увидел Кудрявцев, что это хорошо, (2)"
                             "\n\nИ прекратила кафедра матана контрольные Знаменской (rly?), "
                             "\n\nИ увидел Кудрявцев, что это хорошо, (3)"
                             "\n\nИ почил Кудрявцев в день седьмой от всех дел своих, "
                             "\n\nИ благословил Кудрявцев седьмой день, \\sout{обмыл} освятил его посвят РТ.\n\n"
                             "\\section{Эш}";

static const char* EndLaTexDocument = "\\begin{thebibliography}{}\n"
                               "\\bibitem{}\n"
                               "История России. 10 класс. Учебник. В 3-х частях. Горинов Михаил Михайлович, Моруков Михаил Юрьевич\n"
                               "\\bibitem{}\n"
                               "Фил Уилкинсон: Как взрослеют мальчики. Гид по изменениям тела и настроения\n"
                               "Подробнее.\n"
                               "\\bibitem{}\n"
                               "Язык программирования Си Брайан Керниган, Деннис Ритчи\n"
                               "\\bibitem{}\n"
                               "Гарри Поттер и 425-я комната. Ганцев Матвей, Долгодворов Егор, Фатыхов Тимур, Фролов Даниил и другие.\n"
                               "\\bibitem{}\n"
                               "\\href{https://github.com/Exactlywb}{Төп малай}\n"
                               "\\end{thebibliography}\n"
                               "\\end{document}";

enum PhraseChoice {

    FIRST_EQUATION,
    EVERYBODY_KNOWS,
    SALAM,
    SINUS_FOCUS,
    COSINUS_FOCUS,
    TRANSFORM,
    START_TAN,
    START_CTAN,
    EXPENSIVE,
    DEG_PHRASE,
    END_EQUATION

};

void  MainDifferentiator ();
Node* DifferentiateTree  (Node* currentNode, FILE* writeInto);

Node* MakeConstNode      (double value, Node* left, Node* right);
Node* MakeVarNode        (char var, Node* left, Node* right);
Node* MakeFuncNode       (FuncList funcName, Node* left, Node* right);

bool    DoReplacement (Tree* mainTree, Node* currentNode, Tree* newTree, char symb);
size_t  Optimizer     (Tree* mainTree, Node* currentNode);

Node* AddDiff   (Node* currentNode, FILE* writeInto);
Node* SubDiff   (Node* currentNode, FILE* writeInto);
Node* MulDiff   (Node* currentNode, FILE* writeInto);
Node* DivDiff   (Node* currentNode, FILE* writeInto);
Node* SinDiff   (Node* currentNode, FILE* writeInto);
Node* CosDiff   (Node* currentNode, FILE* writeInto);
Node* TanDiff   (Node* currentNode, FILE* writeInto);
Node* CTanDiff  (Node* currentNode, FILE* writeInto);
Node* DegDiff   (Node* currentNode, FILE* writeInto);
Node* LnDiff    (Node* currentNode, FILE* writeInto);

void    DoLaTex              (Node* currentNode);
void    SetPreambulIntoLaTex (FILE* writeTexInto);
void    SetEndIntoLaTex      (FILE* writeTexInto);
void    StartTex             ();
void    PrintTreeTex         (Node* currentNode, FILE* writeTexInto);
void    TexStartNewFormula   (FILE* writeInto);
void    TexEndFormula        (FILE* writeInto);
void    TexPrintPhrase       (FILE* writeInto, PhraseChoice phraseNum);
void    AutoTreeTexPrint     (Node* startNode, FILE* writeTexInto, char treePhrase);
void    AutoReplaces         (FILE* writeInto, Tree* newTree);

//~~~~~~Foundation~~~~~~
Node* ReadExpression (char* expression);
Node* GetAdd         ();
Node* GetMul         ();
Node* GetBrack       ();
Node* GetNum         ();
//~~~~~~~~~~~~~~~~~~~~~~

//~~Trigonometry~~
Node* GetSin   ();
Node* GetCos   ();
Node* GetTan   ();
Node* GetCotan ();
//~~~~~~~~~~~~~~~~

//~~Deg and log~~
Node* GetLn    ();
Node* GetDeg   ();
//~~~~~~~~~~~~~~~~

void SyntaxError (const char* funcName);
char Require     (char symbToRequire, int line);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
