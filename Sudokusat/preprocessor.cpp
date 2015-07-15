// Tseitin Transformation (?)
// tools.computational-logic.org
#include "preprocessor.h"
#include <cmath>
#include <cstdlib>
bool Preprocessor::defined(false);
std::vector< std::vector <std::set<Pos> > > Preprocessor::possibleNumber;
std::set<Pos> Preprocessor::onlyOnePossible;
std::vector<std::vector< std::set< Pos> > > Preprocessor::possiblitiesRow, Preprocessor::possiblitiesCol;
std::vector<std::vector<std::vector< std::set< Pos> > > > Preprocessor::possiblitiesSquare;

std::vector< std::vector< std::map<Pos, int> > >Preprocessor::varToNumber;
std::vector<Pos> Preprocessor::numberToVar;

/**************************************************************************************************
 *                                          public functions
 ********************************************************** ***************************************/

/**
 *  returns true if is complete filled
 *  return false in other cases
*/
bool Preprocessor::doIt(Sudokufield &field){
    // Create a array for the possible Numbers
    possibleNumber.clear();
    possiblitiesCol.clear();
    possiblitiesRow.clear();
    possiblitiesSquare.clear();
    varToNumber.clear();
    numberToVar.clear();

    for(int i=0; i<field.getSize(); ++i){
        std::vector< std::set < Pos> > buffer;
        for(int j=0; j<field.getSize(); ++j){
            std::set<Pos> t;
            buffer.push_back(t);
        }
        possibleNumber.push_back(buffer);
    }
// Test if it is Empty, fill it whit a given pattern
    if(isEmpty(field)){
        fillEmpty(field);
    } else {
// in other cases calc all Possible Numbers and set all Numbers to one, where only one possiblity is given
        fillInAllNumbersWithOnePossiblity(field);
        calcVarTransform(field);
    }
    std::cout << "number Vars: " << getVarCount() << std::endl;
    return field.isCompleteFilledIn();
}
/**
 * @brief Preprocessor::getAllPossibleNumbers
 * @return a Pointer of Pointer of std::set, which descripe the possiblities of numbers in a field
 * call:
 * std::set<int>** i = Preprocessor::getAllPossibleNumbers(); // get it
    i[x][y]->find(something); // do something with it
*/
std::vector< std::vector < std::set<Pos> > >& Preprocessor::getAllPossibleNumbers(){
    return possibleNumber;
}
/**
 * @brief Preprocessor::getRowPossiblities
 * @return a Datastructure that gives you all Possiblities for a row
 * example call:
 * std::vector<std::vector< std::set< Pos> > >&  test = Preprocessor::getRowPossiblities();
 * test[row][value].size(); // how many Positions for this value are possible in this row
 */
std::vector<std::vector< std::set< Pos> > >& Preprocessor::getRowPossiblities(){
    return possiblitiesRow;
}
/**
 * @brief Preprocessor::getColPossiblities
 * @return a Datastructure that gives you all Possiblities for a column
 * example call:
 * std::vector<std::vector< std::set< Pos> > >&  test = Preprocessor::getColPossiblities();
 * test[column][value].size(); // how many Positions for this value are possible in this col
 */
std::vector<std::vector< std::set< Pos> > >& Preprocessor::getColPossiblities(){
    return possiblitiesCol;
}
/**
 * @brief Preprocessor::getSquarePossiblities
 * @return a Datastructure that gives you all Possiblities for a square
 * example call:
 * std::vector<std::vector<std::vector< std::set< Pos> > > >&  test = Preprocessor::getSquarePossiblities();
 * test[row][column][value].size(); // how many Positions for this value are possible in square(row, column)
 */
std::vector<std::vector<std::vector< std::set< Pos> > > >& Preprocessor::getSquarePossibilities(){
    return possiblitiesSquare;
}

int Preprocessor::getIntForPos(const Pos& t){
    return varToNumber[t.getX()][t.getY()][t];
}

Pos Preprocessor::getPosForInt(const int& t){
    return numberToVar[t];
}

int Preprocessor::getVarCount(){
    return numberToVar.size()-1;
}

/**************************************************************************************************
                                Special Case: filled a Empty field
**************************************************************************************************/
/**
  Test a Sudokufield if it is Empty. The count of given Numbers is equal to 0
 * @brief isEmpty
 * @param field
 * @return true if it is empty, false in other cases
*/
bool Preprocessor::isEmpty(Sudokufield& field){
    for(int i=0; i<field.getSize(); ++i){
        for(int j=0; j<field.getSize(); ++j){
            if( field(i, j) != 0) return false;
        }
    }
    return true;
}
/**
    This function use an easy method to fill an Empty Sudoku
 * @brief fillEmpty
 * @param field
*/

void Preprocessor::fillEmpty(Sudokufield& field){
    int sqrtSize( std::sqrt(field.getSize()));
// rows
    for(int versatzRow=0; versatzRow < sqrtSize; ++versatzRow){
        for(int row=0; row < sqrtSize; ++row){
// cols
            for(int versatzCol=0; versatzCol< sqrtSize; ++versatzCol){
                for(int col=0; col< sqrtSize; ++col){
                    field(versatzRow*sqrtSize+row, versatzCol * sqrtSize + col)
                            = (((versatzCol+row)*sqrtSize) + col+versatzRow) % field.getSize() + 1;
                }
            }
        }
    }
}

/**************************************************************************************************
                            Filled Numbers with one possiblity
**************************************************************************************************/
/**
  This function filled in all Numbers where only one possiblity is given
 * @brief Preprocessor::fillInAllNumbersWithOnePossiblity
 * @param field
 */
void Preprocessor::fillInAllNumbersWithOnePossiblity(Sudokufield& field){
    calculatePossbileNumbers(field);
    calcPossibleNumbersInCols(field);
    calcPossibleNumbersInRows(field);
    calcPossibleNumbersInSquare(field);
    fillAllPossible(field);
}

/**
 * @brief calculatePossbileNumbers
 * @param field
*/
void Preprocessor::calculatePossbileNumbers(Sudokufield& field){
    int sqrtSize = std::sqrt(field.getSize());

// Create new Possibility Container
    std::set<int> givenNumberCol[field.getSize()], givenNumberRow[field.getSize()];
    std::set<int> givenNumberSquare[sqrtSize][sqrtSize];

// Find all possibilities for Cols and rows
    for(int i=0; i<field.getSize(); ++i){
        for(int j=0; j<field.getSize(); ++j){
            if(field(i,j) != 0){
                givenNumberCol[i].insert(field(i,j));
                givenNumberRow[j].insert(field(i,j));
            }
        }
    }
// Find all possibilities in Squares
    for(int posX=0; posX<sqrtSize; ++posX){
        for(int posY=0; posY<sqrtSize; ++posY){
            for(int x=0; x<sqrtSize; ++x){
                for(int y=0; y<sqrtSize; ++y){
                    if(field(posX*sqrtSize + x, posY * sqrtSize + y) != 0){
                        givenNumberSquare[posX][posY].insert(field(posX*sqrtSize + x, posY * sqrtSize + y));
                    }
                }
            }
        }
    }
// Create the possiblitieField
    for(int nr=1; nr <= field.getSize(); ++nr)
        for(int x=0; x<field.getSize(); ++x)
            if(givenNumberCol[x].find(nr) == givenNumberCol[x].end())
                for(int y=0; y<field.getSize(); ++y)
                    if(field(x,y) == 0)
                        if(givenNumberRow[y].find(nr) == givenNumberRow[y].end())
                            if(givenNumberSquare[x/sqrtSize][y/sqrtSize].find(nr) == givenNumberSquare[x/sqrtSize][y/sqrtSize].end())
                                possibleNumber[x][y].insert(Pos(x, y, nr));

// Check if the Number can be filled out
    for(int x=0; x< field.getSize(); ++x)
        for(int y=0; y < field.getSize(); ++y){
            if(possibleNumber[x][y].size() == 1)
                onlyOnePossible.insert(*(possibleNumber[x][y].begin()));
        }


}

/**
  set all positions with only one possiblity to the right number
 * @brief Preprocessor::findOnePossible
*/

int Preprocessor::fillAllPossible(Sudokufield& field){
    int anz( onlyOnePossible.size());
    while(!onlyOnePossible.empty()){
        Pos curr = *(onlyOnePossible.begin());
        onlyOnePossible.erase(onlyOnePossible.begin());
        field(curr.getX(), curr.getY()) = curr.getValue();
        deleteDoubleCell(curr.getX(), curr.getY(), curr.getValue(), field);
        deleteDoubleColAndRow(curr.getX(), curr.getY(), curr.getValue(), field);
        deleteDoubleSquare(curr.getX(), curr.getY(), curr.getValue(), field);
        
    }
    return anz;
}
/**************************************************************************************************
 *                              delete functions
 * ***********************************************************************************************/

/**
  if you set an value to a value, you can erase this possibily in the row, column and square with this function
 * @brief Preprocessor::deleteInColAndRow
 * @param x: the row
 * @param y: the colum
 * @param value: the value what must be erase
 * @param field: the Sudokufield for size
 */
void Preprocessor::deleteDoubleColAndRow(int x, int y, int value, Sudokufield& field){
    if((x==0) || (y==0) || (value==0)) return;
    int sqrtSize = std::sqrt(field.getSize());
    int startX = (x / sqrtSize) * sqrtSize;
    int startY = (y / sqrtSize) * sqrtSize;

// delete the values in Lines
    possiblitiesCol[y][value-1].clear();
    possiblitiesRow[x][value-1].clear();

// Delete all values at Position
    for(int i=0; i< field.getSize(); ++i){
        possiblitiesCol[y][i].erase(Pos(x, y, i+1));
        if(possiblitiesCol[y][i].size() == 1)
            onlyOnePossible.insert(*(possiblitiesCol[y][i].begin()));

        possiblitiesRow[x][i].erase(Pos(x, y, i+1));
        if(possiblitiesRow[x][i].size() == 1)
            onlyOnePossible.insert(*(possiblitiesRow[x][i].begin()));
   }

// Delete in other cols and Rows
    for(int i=0; i<field.getSize(); ++i){
        possiblitiesCol[i][value-1].erase(Pos(x, i, value));
        if(possiblitiesCol[i][value-1].size() == 1){
            onlyOnePossible.insert(*(possiblitiesCol[i][value-1].begin()));;
        }
        possiblitiesRow[i][value-1].erase(Pos(i, y, value));
        if(possiblitiesRow[i][value-1].size() == 1){
            onlyOnePossible.insert(*(possiblitiesRow[i][value-1].begin()));;
        }
    }
//*/

// Delete in square
    for(int posX = startX; posX < startX + sqrtSize; ++posX){
        for(int posY = startY; posY < startY +sqrtSize; ++posY){
             possiblitiesCol[posY][value-1].erase(Pos(posX, posY, value));
             if(possiblitiesCol[posY][value-1].size() == 1){
                 onlyOnePossible.insert(*(possiblitiesCol[posY][value-1].begin()));;
             }
             possiblitiesRow[posX][value-1].erase(Pos(posX, posY, value));
             if(possiblitiesRow[posX][value-1].size() == 1){
                 onlyOnePossible.insert(*(possiblitiesRow[posX][value-1].begin()));;
             }
        }
    }
//*/
}

void Preprocessor::deleteDoubleCell(int x, int y, int value, Sudokufield& field){
    possibleNumber[x][y].clear();
// Delete in Row and column
    for(int i=0; i<field.getSize(); ++i){

        possibleNumber[x][i].erase(Pos(x, i, value));
        if(possibleNumber[x][i].size() == 1) {
            onlyOnePossible.insert(*(possibleNumber[x][i].begin()));

        }
        possibleNumber[i][y].erase(Pos(i, y, value));
        if(possibleNumber[i][y].size() == 1)
            onlyOnePossible.insert(*(possibleNumber[i][y].begin()));
    }
// Delete in square in Possiblities
    int sqrtSize = std::sqrt(field.getSize());
    int startX = (x / sqrtSize) * sqrtSize;
    int startY = (y / sqrtSize) * sqrtSize;
    for(int posX = startX; posX < startX + sqrtSize; ++posX){
        for(int posY = startY; posY < startY +sqrtSize; ++posY){
            possibleNumber[posX][posY].erase(Pos(posX, posY, value));
            if(possibleNumber[posX][posY].size() == 1)
                onlyOnePossible.insert(*(possibleNumber[posX][posY].begin()));
        }
    }
}

void Preprocessor::deleteDoubleSquare(int x, int y, int value, Sudokufield& field){
    int sqrtSize = std::sqrt(field.getSize());
    int squareX = (x / sqrtSize) ;
    int squareY = (y / sqrtSize);
// delete Values in Square
    possiblitiesSquare[squareX][squareY][value-1].clear();

// Delete all values at Position
    for(int i=0; i< field.getSize(); ++i){
        possiblitiesSquare[squareX][squareY][i].erase(Pos(x, y, i+1));
        if(possiblitiesSquare[squareX][squareY][i].size() == 1)
            onlyOnePossible.insert(*(possiblitiesSquare[squareX][squareY][i].begin()));
    }
// delete in other squares
    for(int i=0; i< sqrtSize; ++i){
        for(int j=0; j<sqrtSize; ++j){
            possiblitiesSquare[i][squareY][value-1].erase(Pos(i*sqrtSize +j, y, value));
            if(possiblitiesSquare[i][squareY][value-1].size() == 1)
                onlyOnePossible.insert(*(possiblitiesSquare[i][squareY][value-1].begin()));

            possiblitiesSquare[squareX][i][value-1].erase(Pos(x, i*sqrtSize+j, value));
            if(possiblitiesSquare[squareX][i][value-1].size() == 1)
                onlyOnePossible.insert(*(possiblitiesSquare[squareX][i][value-1].begin()));
        }
    }

}

/**************************************************************************************************
 *                              calc start positions
 * ***********************************************************************************************/
/**
 * @brief Preprocessor::getRowPossiblities
 * @return a Datastructure that gives you all Possiblities for a row
 * example call:
 * std::vector<std::vector< std::set< Pos> > >&  test = Preprocessor::getRowPossiblities();
 * test[row][value].size(); // how many Positions for this value are possible in this row
 */
// find all possible Numbers in Row, Col and Square
void Preprocessor::calcPossibleNumbersInCols(Sudokufield& field){
// create structure
    possiblitiesCol.clear();
    for(int col=0; col<field.getSize(); ++col){
        std::vector<std::set<Pos> > buffercol;
        for(int v=0; v<field.getSize()+1; ++v){
            std::set<Pos> bufferV;
            buffercol.push_back(bufferV);
        }
        possiblitiesCol.push_back(buffercol);
    }

    for(int col=0; col < field.getSize(); ++col){
// find values
        bool values[field.getSize()];
        for(int i=0; i< field.getSize(); ++i){
            values[i] = false;
        }
        for(int row=0; row < field.getSize(); ++row){
            if(field(row, col) != 0){
                values[field(row, col) - 1] = true;
            }
        }
        for(int i=0; i<field.getSize(); ++i){
            if(!values[i]){
                for(int row=0; row< field.getSize(); ++row){
                    if(possibleNumber[row][col].find(Pos(row, col, i+1)) != possibleNumber[row][col].end()){
                        possiblitiesCol[col][i].insert(Pos(row, col, i+1));
                    }
                }
            }
        }
    }
    for(int i=0; i< field.getSize(); ++i)
        for(int j=0; j<field.getSize(); ++j){
            if(possiblitiesCol[i][j].size() == 1){
                onlyOnePossible.insert(*possiblitiesCol[i][j].begin());
            }
        }
}

// find all possible Numbers in Row, Col and Square
void Preprocessor::calcPossibleNumbersInRows(Sudokufield& field){
// create structure
    possiblitiesRow.clear();
    for(int row=0; row<field.getSize(); ++row){
        std::vector<std::set<Pos> > buffercol;
        for(int v=0; v<field.getSize()+1; ++v){
            std::set<Pos> bufferV;
            buffercol.push_back(bufferV);
        }
        possiblitiesRow.push_back(buffercol);
    }

    for(int row=0; row< field.getSize(); ++row){
// find values
        bool values[field.getSize()];
        for(int i=0; i< field.getSize(); ++i){
            values[i] = false;
        }
        for(int col=0; col< field.getSize(); ++col){
            if(field(row, col) != 0){
                values[field(row, col) - 1] = true;
            }
        }
// insert Values
        for(int i=0; i<field.getSize(); ++i){
            if(!values[i]){
                for(int col=0; col< field.getSize(); ++col){
                    if(possibleNumber[row][col].find(Pos(row, col, i+1)) != possibleNumber[row][col].end()){
                        possiblitiesRow[row][i].insert(Pos(row, col, i+1));
                    }
                }
            }
        }
    }
    for(int i=0; i< field.getSize(); ++i)
        for(int j=0; j<field.getSize(); ++j){
            if(possiblitiesCol[i][j].size() == 1){
                onlyOnePossible.insert(*possiblitiesRow[i][j].begin());
            }
        }
}

void Preprocessor::calcPossibleNumbersInSquare(Sudokufield& field){
// Create datastructure
    int sqrtSize = std::sqrt(field.getSize());
    possiblitiesSquare.clear();
    for(int col=0; col< sqrtSize; ++col){
        std::vector<std::vector<std::set<Pos> > > bufferRow;
        for(int row=0; row<sqrtSize; ++row){
            std::vector<std::set<Pos> > buffercol;
            for(int v=0; v<field.getSize()+1; ++v){
                std::set<Pos> bufferV;
                buffercol.push_back(bufferV);
            }
            bufferRow.push_back(buffercol);
        }
        possiblitiesSquare.push_back(bufferRow);
    }
//
    for(int rowSquare=0; rowSquare< sqrtSize; ++rowSquare){
        for(int colSquare=0; colSquare< sqrtSize; ++colSquare){
// find values
            bool values[field.getSize()];
            for(int i=0; i< field.getSize(); ++i){
                values[i] = false;
            }
            for(int row=0; row < sqrtSize; ++row)
                for(int col=0; col< sqrtSize; ++col){
                    if(field(rowSquare*sqrtSize + row, colSquare*sqrtSize + col) != 0){
                        values[field(rowSquare*sqrtSize + row, colSquare*sqrtSize + col) - 1] = true;
                    }
                }
// insert Values
            for(int i=0; i<field.getSize(); ++i){
                if(!values[i]){
                    for(int row=0; row < sqrtSize; ++row)
                        for(int col=0; col< sqrtSize; ++col){
                            if(possibleNumber[rowSquare*sqrtSize + row][colSquare*sqrtSize + col].find(Pos(rowSquare*sqrtSize + row, colSquare*sqrtSize + col,i+1)) != possibleNumber[rowSquare*sqrtSize + row][colSquare*sqrtSize + col].end()){
                                possiblitiesSquare[rowSquare][colSquare][i].insert(Pos(rowSquare*sqrtSize + row, colSquare*sqrtSize + col,i+1));
                            }
                        }
                }
            }
        }
    }
    for(int i=0; i< sqrtSize; ++i)
        for(int j=0; j< sqrtSize; ++j){
            for(int k=0; k<field.getSize(); ++k){
                if(possiblitiesCol[i][j].size() == 1){
                    onlyOnePossible.insert(*possiblitiesSquare[i][j][k].begin());
                }
            }
        }
}

/**************************************************************************************************
 *                                          var mapper
 * ***********************************************************************************************/
void Preprocessor::calcVarTransform(Sudokufield& test){
    varToNumber.clear();
    numberToVar.clear();

    numberToVar.push_back(Pos(0,0,0));
    for(int i=0; i<test.getSize(); ++i){
        std::vector<std::map<Pos, int> > buffer;
        for(int j=0; j<test.getSize(); ++j){
            std::map<Pos, int> t;
            for(Pos p: Preprocessor::getAllPossibleNumbers()[i][j]){
                t.insert(std::pair<Pos, int>(p, numberToVar.size()));
                numberToVar.push_back(p);
            }
            buffer.push_back(t);
        }
        varToNumber.push_back(buffer);
    }
}
