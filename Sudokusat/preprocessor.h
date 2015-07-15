/**
    This Class test following Thinks
    1. If the Class is empty, it will be filled
    2. It will be filled all Positions, where only 1 Possiblity is given
*/

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "sudokufield.h"
#include "pos.h"
#include <set>
#include <vector>

class Preprocessor
{
public:
    //Preprocessor();
    static bool doIt(Sudokufield& field);

    static std::vector<std::vector< std::set< Pos> > >& getAllPossibleNumbers();
    static std::vector<std::vector< std::set< Pos> > >& getRowPossiblities();
    static std::vector<std::vector< std::set< Pos> > >& getColPossiblities();
    static std::vector<std::vector<std::vector< std::set< Pos> > > >& getSquarePossibilities();
    static int getIntForPos(const Pos& t);
    static Pos getPosForInt(const int& t);
    static int getVarCount();
private:
// variables
    static bool defined;
    static std::vector< std::vector<std::set<Pos> > > possibleNumber;
    static std::set<Pos> onlyOnePossible;
    static std::vector<std::vector< std::set< Pos> > > possiblitiesRow, possiblitiesCol;
    static std::vector<std::vector<std::vector< std::set< Pos> > > > possiblitiesSquare;
    static std::vector<std::vector<std::map<Pos, int> > > varToNumber;
    static std::vector<Pos> numberToVar;
// functions
// Special Case it is Empty
    static bool isEmpty(Sudokufield& field);
    static void fillEmpty(Sudokufield& field);
// Fill in the Positions where only one Possiblity is going
    static void fillInAllNumbersWithOnePossiblity(Sudokufield& field);
    static void calculatePossbileNumbers(Sudokufield& field);
    static int fillAllPossible(Sudokufield& field);
// Delete
    static void deleteDoubleColAndRow(int x, int y, int value, Sudokufield& field);
    static void deleteDoubleCell(int x, int y, int value, Sudokufield& field);
    static void deleteDoubleSquare(int x, int y, int value, Sudokufield& field);
// fill in possiblities where a number can only stand at one position
    static int fillInRows(Sudokufield& field);
    static int fillInCols(Sudokufield& field);
    static int fillInSquares(Sudokufield& field);
// find all possible Numbers in Row, Col and Square
    static void calcPossibleNumbersInCols(Sudokufield& field);
    static void calcPossibleNumbersInRows(Sudokufield& field);
    static void calcPossibleNumbersInSquare(Sudokufield& field);
// get NUmbers
    static void calcVarTransform(Sudokufield& t);
};

#endif // PREPROCESSOR_H
