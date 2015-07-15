#ifndef SUDOKU2CNF_H
#define SUDOKU2CNF_H
//https://github.com/R053NR07/hidoku_sat/blob/master/write_logic.cpp
#include <string>
#include "sudokufield.h"
#include <map>
#include "pos.h"
#include <set>
#include "preprocessor.h"

class Sudoku2CNF
{
public:
    Sudoku2CNF();


    int sudoku2Logic(const Sudokufield& field, std::stringstream& result);

    int getClauselCount()const;
    int getVarCountAdd() const;
/*
 * transforms the given cnf formula to the standard dimacs cnf format.
 * each string in the vector "cnfDimacs" contains a line for the dimacs format
 * 
 */     

    
private:

 /*
  * creates a propositional formula that is true iff exactly one variable in "vars" is true
  * the formuala is in cnf and is a vector of strings, where one string defines a clause. the literals are separated by whitespaces
  *
  * returns 0 if everything is fine
  */
    int getExactlyOne(const std::vector<std::string>& vars, std::stringstream&);
    int getExactlyOne(const std::vector< Pos >& vars, 		std::stringstream& result);
    int getExactlyOne(const std::set< Pos >& vars, 			std::stringstream& result);

    int getAtMostOne(const std::set< Pos >& vars, 			std::stringstream& result);
    int getExactlyOneLinear(const std::set< Pos >& vars, 			std::stringstream& result);
    int getCMDRAtMostOne(const std::set<Pos> &vars, std::set< Pos > &result);

/*
 * Semantik von possibilities:
 * Der äusere Vektor ist die Zeile/Spalte
 * Der zweite Vektor symbolisiert den Wert (von 1 bis n^2)
 * der innere vector (enthält Pos) zeigt an, an welchen Positionen der Wert möglich ist
 */
    int getCNFForRow( const int row, std::vector< std::vector< std::set< Pos> > >& possibilities , std::stringstream& result );
    int getCNFForCol( const int col, std::vector< std::vector< std::set< Pos> > >& possibilities , std::stringstream& result );

    int getCNFForRowOrCol( const int rowOrCol , std::vector< std::vector< std::set< Pos> > >& possibilities , std::stringstream& result );
/*
* Semantik von possibilities:
* Der äusere Vektor ist die Zeile/Spalte
* Der zweite Vektor symbolisiert den Wert (von 1 bis n^2)
* der innere vector (enthält Pos) zeigt an, an welchen Positionen der Wert möglich ist
*/
    int getCNFForSquare(int row, int col, std::vector< std::vector< std::vector< std::set< Pos> > > >& possibilities, std::stringstream& result);
    int getCNFForSquare(int row, int col, std::vector< std::set< Pos> > & possibilities, std::stringstream& result);
    
 /*
  * helpermethod for string splitting
  */   
   std::vector<std::string> split(const std::string &s, char delim);

   int countClausel;
   int VarCountAdd;
   int comCount;
};

#endif // SUDOKU2CNF_H
