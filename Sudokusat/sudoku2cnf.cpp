#include "sudoku2cnf.h"
#include <sstream>
#include <set>
#include <vector>
#include <string>
#include <assert.h>
#include "pos.h"
#include <cmath>
#include <cstdlib>

using namespace std;



Sudoku2CNF::Sudoku2CNF(): countClausel(0), VarCountAdd(0){ }



int Sudoku2CNF::sudoku2Logic(const Sudokufield& field, std::stringstream& result)
{

	// row and column
	for( short i=0 ; i < field.getSize() ; ++i )
	{
        getCNFForCol( i , Preprocessor::getColPossiblities() , result );
        getCNFForRow( i , Preprocessor::getRowPossiblities() , result );
		
	}
	
    // cells
    std::vector<Pos> emptyCells = field.getAllEmptyFields();
    for( Pos p : emptyCells )
    {
        getExactlyOne( Preprocessor::getAllPossibleNumbers()[p.getX()][p.getY()] , result );
    }
	
	// squares
	int sizeSqrt = (int) std::sqrt( field.getSize() );
	
	for( int i = 0 ; i < sizeSqrt ; ++i )
	{
		for( int j = 0 ; j < sizeSqrt ; ++j)
		{
			//for( std::set<Pos> vars : Preprocessor::getSquarePossibilities()[i][j] )
			for( std::vector<std::set<Pos>>::iterator iter = Preprocessor::getSquarePossibilities()[i][j].begin() + 1; iter != Preprocessor::getSquarePossibilities()[i][j].end() ; ++iter)
			{
				getExactlyOne( *iter , result);
			}
		}
	}

    return 0;
}




/**************************************************************************************************
 *                                      Exactly One
 * ***********************************************************************************************/

int Sudoku2CNF::getExactlyOne( const std::vector< Pos >& vars, std::stringstream& result )
{

    std::stringstream clause;

// pairwaise to (-x1 or -x2)
	for( std::vector< Pos >::const_iterator it1 = vars.begin() ; it1 != vars.end() ; ++it1 )
	{

        clause << Preprocessor::getIntForPos((*it1).toString()) << " ";

		for( std::vector< Pos >::const_iterator it2 = it1 + 1 ; it2 != vars.end() ; ++it2 )
		{
            countClausel++;
            result << "-" << Preprocessor::getIntForPos((*it1).toString()) << " -" << Preprocessor::getIntForPos((*it2).toString()) << " 0" << std::endl;

		}

	}


    if( !clause.str().empty()){
        countClausel++;
        result << clause.str() << "0" << std::endl;
	}

	return 0;
}


int Sudoku2CNF::getExactlyOne( const std::set< Pos >& vars, std::stringstream& result )
{

    std::stringstream clause;

// pairwaise to (-x1 or -x2)
	for( std::set< Pos >::const_iterator it1 = vars.begin() ; it1 != vars.end() ; ++it1 )
	{

        clause << Preprocessor::getIntForPos((*it1).toString()) << " ";
        bool found(false);
        for( std::set< Pos >::const_iterator it2 = it1; it2 != vars.end() ; ++it2)
		{
            if(!found){
                found = true;
                continue;
            }
            countClausel++;
            result << "-" << Preprocessor::getIntForPos((*it1).toString()) << " -" << Preprocessor::getIntForPos((*it2).toString()) << " 0" << std::endl;

		}

	}

// for all x1..xn: ( x1 OR x2 OR .. OR xn-1 OR xn )

    if( !clause.str().empty()){
        countClausel++;
        result << clause.str() << "0" << std::endl;
	}

	return 0;
}

/**************************************************************************************************
 *                                          get Cnf for something
 * ***********************************************************************************************/

int Sudoku2CNF::getCNFForRow( const int row,
								std::vector<std::vector< std::set< Pos> > >& possibilities,
                                std::stringstream& result)
{
    return getCNFForRowOrCol( row , possibilities , result);

}


int Sudoku2CNF::getCNFForCol( const int col ,
								std::vector< std::vector< std::set< Pos> > >& possibilities ,
                                std::stringstream& result )
{
    return getCNFForRowOrCol( col, possibilities , result);
}


int Sudoku2CNF::getCNFForRowOrCol( const int rowOrCol ,
									std::vector< std::vector< std::set< Pos> > >& possibilities ,
                                    std::stringstream& result )
{

    for( unsigned int value = 0 ; value < possibilities[rowOrCol].size() ; ++value )
	{
		getExactlyOne( possibilities[ rowOrCol ][ value ] , result);
	}

	return 0;

}


std::vector<std::string> Sudoku2CNF::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

int Sudoku2CNF::getClauselCount()const{
    return countClausel;
}

int Sudoku2CNF::getVarCountAdd()const{
    return VarCountAdd;
}
