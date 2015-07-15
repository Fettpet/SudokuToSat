/**
    // Create a Sudokufield with size 12x12
    Sudokufield test(12);
    // set the field x=8, y=3 to 10
    test(8,3) = 10;
    // Output the complete Sudoku
    cout << test << std::endl;
    // Output the cell x=8, y=3;
    cout << test(8,3) << std::endl;
*/

#ifndef SUDOKUFIELD_H
#define SUDOKUFIELD_H
#include <vector>
#include <iostream>
#include <fstream>
#include "pos.h"
#include <map>

#define DEBUG
class Sudokufield
{
public:
//Constructors
    Sudokufield(std::string fileName);
    Sudokufield(int n);
// Access
  //Getter
    short operator()(short x, short y) const;
    std::map<std::string, int>& getVarToNumber();
    std::vector<std::string>& getNumberToVar();
  //Setter
    short& operator()(short x, short y);
    
    void setVarToNumber(const std::map<std::string, int>& varToNumber );
    void setNumberToVar(const std::vector<std::string>& numberToVar );
    
// tester
    bool isCompleteFilledIn() const;
    bool isRight() const;
    bool isEmpty(short x, short y) const;
    bool isSatifable() const;
//getter
    std::vector<Pos> getAllEmptyFields() const;

    unsigned short getSize()const;
    
private:
    unsigned short size;
    std::map<std::string, int> varToNumber;
	std::vector<std::string> numberToVar;
	
    std::vector< std::vector< short> > field;
};

std::ostream& operator <<(std::ostream& out, const Sudokufield& f);
#endif // SUDOKUFIELD_H
