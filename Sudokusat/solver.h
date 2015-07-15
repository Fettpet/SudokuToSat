#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include "sudokufield.h"
#include <vector>
#include <sstream>

using namespace std;

// encapsulates main algorithm and data structures
class Solver
{

public:
    Solver( string sudokufile );
    Solver( string sudokufile, string solutionfile );
    Solver( string workingDir, string sudokufile, string solutionfile, string satsolverCmd );
 
// main algorithm  
    int solve();
    Sudokufield& getSolution();
private:
// members
    string _solutionfile, _dimacsFile, _satsolverCmd, _tempDir, _workingDir;
	Sudokufield _field;
    std::stringstream _cnf;
	
	static const string _defaultSolutionfile;
	static const string _defaultssCmd;
	static const string _defaultDimacsFile;

//functions	
    void writeDimacsFile(int varCount, int clausCount);
	void readSolutionFile();
    void readSolutionFileMinisat();
    void readSolutionFileOther();
	vector<string> split( string &s, char delim);
    
};


#endif
