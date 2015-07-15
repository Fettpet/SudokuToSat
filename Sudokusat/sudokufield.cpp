#include "sudokufield.h"
#include <cassert>
#include <set>
#include <cstdlib>
#include <map>
/**************************************************************************************************
 *                                              Construktors
 *************************************************************************************************/
/**
 * @brief Sudokufield::Sudokufield
 * @param n: Defines the size of the field
*/
Sudokufield::Sudokufield(int n):
    size(n)
{
	std::map<std::string, int> varToNumber;
	std::vector<std::string> numberToVar;
	
    for(int i=0; i<size; ++i){
        std::vector<short> buffer;
        for(int j=0; j<size; ++j){
            buffer.push_back(0);
        }
        field.push_back(buffer);
    }
}
/**
 * @brief Sudokufield::Sudokufield
 * @param fileName: The File where the sudoku is
 */
Sudokufield::Sudokufield(std::string fileName):
    size(-1)
{
	std::map<std::string, int> varToNumber;
	std::vector<std::string> numberToVar;
	
// open file and test exists
    std::fstream data;
    data.open((fileName.c_str()), std::ios::in);
    if(!data.is_open()){
        std::cerr << "cant open file " << fileName.c_str() << std::endl;
        std::exit(0);
    }
    while(!data.eof()){
// read line
        std::string line;
        std::getline(data, line);
// write into Sudoku
        if(line.find("puzzle") != std::string::npos){
            line = line.substr(line.find(":")+1, line.find("x"));
            size = std::atoi(line.c_str());
        } else if(line.find("|") != std::string::npos){
// input line into sudoku
            std::string number;
            std::vector<short> buffer;
            for(unsigned int i=0; i< line.size(); ++i){
                if((line[i] == ' ') && (number.size() != 0)){
                    buffer.push_back(std::atoi(number.c_str()));
                    number.clear();
                } else if((line[i] >= '0') && (line[i] <= '9')){
                    number.push_back(line[i]);
                } else if(line[i] == '_'){
                    number.push_back('0');
                }
            }
            field.push_back(buffer);
        }
    }
    size = field.size();
}

/**************************************************************************************************
                                            operator
**************************************************************************************************/

//Getter
/**
This funciton is the function to read one cell in the Sudokufield
 * @brief Sudokufield::operator ()
 * @param x: The X Position
 * @param y: The Y Position
 * @return the value of the field
 */
short Sudokufield::operator()(short x, short y) const{
#ifdef DEBUG

    if((x > size) || (x < 0)){
        std::cerr << "Randbereich überschritten wegen x";
        assert((x > size) || (x < 0));
    }
    if((y > size) || (y < 0)){
        std::cerr << "Randbereich überschritten wegen y";
        assert((y > size) || (y < 0));
    }
#endif
    return field[x][y];
}
//Setter
/**
This funciton is the function to write one cell in the Sudokufield
 * @brief Sudokufield::operator ()
 * @param x: The X Position
 * @param y: The Y Position
 * @return the value of the field
 */
short& Sudokufield::operator()(short x, short y){
#ifdef DEBUG

    if((x > size) || (x < 0)){
        std::cerr << "Randbereich überschritten x" << std::endl;
        assert((x > size) || (x < 0));
    }
    if((y > size) || (y < 0)){
        std::cerr << "Randbereich überschritten y" << std::endl;
        assert((y > size) || (y < 0));
    }
#endif
    return field[x][y];
}
/**************************************************************************************************
 *                                  Getter
 *************************************************************************************************/
unsigned short Sudokufield::getSize()const{
    return size;
}

std::map<std::string, int>& Sudokufield::getVarToNumber()
{
	return varToNumber;
}

std::vector<std::string>& Sudokufield::getNumberToVar()
{
	return numberToVar;
}


/**************************************************************************************************
 *                                  Setter
 *************************************************************************************************/
void Sudokufield::setVarToNumber(const std::map<std::string, int>& varToNumber )
{
	this->varToNumber = varToNumber;
}
    
void Sudokufield::setNumberToVar(const std::vector<std::string>& numberToVar )
{
	this->numberToVar = numberToVar;
}
/**************************************************************************************************
 *                                  Debug functions
 *************************************************************************************************/
/**
Test the field. If it is complete filled (nowhere is a zero and a number higher then the size)
it returns true, in other cases false
 * @brief Sudokufield::isCompleteFilledIn
 * @return
 */
bool Sudokufield::isCompleteFilledIn() const{
    for(int x=0; x<size; ++x){
        for(int y=0; y<size; ++y){
            if(this->operator ()(x,y) == 0) return false;
            if(this->operator ()(x,y) > size) return false;
        }
    }
    return true;
}

/**
  test the sudokufield if it is filled in the legal way
 * @brief Sudokufield::isRight
 * @return
 */
bool Sudokufield::isRight() const{
    if(!isCompleteFilledIn()) return false;

    // test the rows and the col
    for(int i=0; i<size; ++i){
        std::set<short> testRows;
        std::set<short> testCols;
        for(int j=0; j<size; ++j){
            testCols.insert(this->operator ()(i,j));
            testRows.insert(this->operator ()(j,i));
        }
        if(testCols.size() != size) return false;
        if(testRows.size() != size) return false;
    }
    // the the squares in the sudoku
    int countSquares = std::sqrt(size);
    for(int i=0; i<countSquares; ++i){
        for(int j=0; j<countSquares; ++j){
            std::set<short> test;
            for(int x=0; x<countSquares; ++x){
                for(int y=0; y<countSquares; ++y){
                    test.insert(this->operator ()(i*countSquares + x, j*countSquares + y));
                }
            }
            if(test.size() != size) return false;
        }
    }
    return true;
}

/**
  the a single cell if it is not set (equal to zero)
 * @brief Sudokufield::isEmpty
 * @param x
 * @param y
 * @return true, the cell is set, false in other cases
 */
bool Sudokufield::isEmpty(short x, short y) const{
    if(this->operator ()(x,y) == 0) return false;
    return true;
}



/**
  give back a vector with all Positions, where the fields are not set
 * @brief Sudokufield::getAllEmptyFields
 * @return
 */
std::vector<Pos> Sudokufield::getAllEmptyFields() const{
    std::vector<Pos> result;
    for(int x=0; x<size; ++x){
        for(int y=0; y<size; ++y){
            if(this->operator ()(x,y) == 0) result.push_back(Pos(x,y));
            if(this->operator ()(x,y) > size) result.push_back(Pos(x,y));
        }
    }
    return result;
}

bool Sudokufield::isSatifable() const{
// for lines

    for(int x=0; x<size; ++x){
        bool found[size+1];
        for(int y=0; y<=size; ++y){
            found[y] = false;
        }
        for(int y=0; y<size; ++y){
            if(this->operator()(x,y) != 0) {
                if(found[this->operator()(x,y)]){
                    return false;
                } else {
                    found[this->operator()(x,y)] = true;
                }
            }
        }
    }

// for rows
    for(int y=0; y<size; ++y){
        bool found[size+1];
        for(int x=0; x<=size; ++x){
            found[x] = false;
        }
        for(int x=0; x<size; ++x){
            if(this->operator()(x,y) != 0) {
                if(found[this->operator()(x,y)]){
                    return false;
                } else {
                    found[this->operator()(x,y)] = true;
                }
            }
        }
    }

// for square
    int sqrtSize = std::sqrt(size);
    for(int xf=0; xf < sqrtSize; ++xf){
        for(int yf=0; yf < sqrtSize; ++yf){
            bool found[size+1];
            for(int x=0; x<=size; ++x){
                found[x] = false;
            }
            for(int x=0; x<sqrtSize; ++x){
                for(int y=0; y<sqrtSize; ++y){
                    if(this->operator()(xf * sqrtSize +x, yf*sqrtSize +y) != 0) {
                        if(found[this->operator()(xf * sqrtSize +x, yf*sqrtSize +y)]){
                            return false;
                        } else {
                            found[this->operator()(xf * sqrtSize +x, yf*sqrtSize +y)] = true;
                        }
                    }
                }
            }
        }
    }
  //  */
    return true;
}
/**************************************************************************************************
 *                                  output
 *************************************************************************************************/
/**
    overload the standard output
*/
std::ostream& operator <<(std::ostream& out, const Sudokufield& f){
// Precalculation
     int countNumber(0);
    if(f.getSize() == 100){
        countNumber = 3;
    } else {
        countNumber = std::ceil(std::log10(f.getSize()));
    }
    int countPlus = std::floor(std::sqrt(f.getSize()))* (countNumber+1)+2;
    int countLine = (countNumber +1)* f.getSize() + std::sqrt(f.getSize())*2;
    out << "experiment: generator (Time: 0.008052 s)" << std::endl;
    out << "number of tasks: 1" << std::endl;
    out << "task: 1" << std::endl;
    out << "puzzle size: " << std::sqrt(f.getSize()) << "x" << std::sqrt(f.getSize()) << std::endl;
//the field
    for(int y=0; y<f.getSize(); ++y){
        if(y % (int)std::sqrt(f.getSize()) == 0){
            // line between number rows
            for(int i=0; i< countLine; ++i){
                if(i % countPlus == 0){
                    out << "+";
                } else {
                    out << "-";
                }
            }
            out << "+" <<std::endl;
        }
        out << "| ";
        for(int x=0; x<f.getSize(); ++x){
            if((x % (int)std::sqrt(f.getSize()) == 0) && (x>0)){
                out << "| ";
            }
            int s;
            if(f(y,x) ==0){
                s=1;
            }else if(f(y,x) ==1){
                s=1;
            } else{
                s =std::ceil(std::log10(f(y,x)+1)) ;
            }
            for(int i=s; i< countNumber; ++i){
                out << " ";
            }
            out << f(y,x);


            out << " ";
        }
        out << "|" <<std::endl;

    }
// The last Line
    // line between number rows
    for(int i=0; i< countLine; ++i){
        if(i % countPlus == 0){
            out << "+";
        } else {
            out << "-";
        }
    }
    out << "+" <<std::endl;
//    out << std::endl;
    return out;
}
