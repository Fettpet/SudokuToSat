#include "solver.h"
#include <fstream>
#include <sstream>
#include "sudoku2cnf.h"
#include <algorithm>

using namespace std;


const string Solver::_defaultSolutionfile = "ausg2.txt" ;

const string Solver::_defaultssCmd = "glucose-3.0/core/glucose" ;
const string Solver::_defaultDimacsFile = "cnf.dimacs" ;




Solver::Solver( string sudokufile ) :
    _solutionfile( _defaultSolutionfile ), _dimacsFile( _defaultDimacsFile ),  _satsolverCmd( _defaultssCmd ), _tempDir("/tmp/"),
    _field( sudokufile )
{
	solve();
}

Solver::Solver( string sudokufile, string solutionfile ) : 
    _solutionfile( solutionfile ), _dimacsFile( _defaultDimacsFile ),  _satsolverCmd( _defaultssCmd ), _tempDir("/tmp/"),
     _field( sudokufile )
{
	solve();
}

Solver::Solver(string workingDir,  string sudokufile, string solutionfile, string tmpDir ) :
    _solutionfile( solutionfile ), _dimacsFile( _defaultDimacsFile ),  _satsolverCmd( _defaultssCmd ), _tempDir(tmpDir),
     _field( sudokufile )
{
    _workingDir = workingDir;
    if(_tempDir[_tempDir.size() -1] != '/'){
        _tempDir.push_back('/');
    }
	solve();
}


int Solver::solve()
{
	
	Sudoku2CNF s2cnf;
    if(!Preprocessor::doIt( _field )){;
        if(_field.isSatifable()) {
            s2cnf.sudoku2Logic( _field, _cnf );

            writeDimacsFile(Preprocessor::getVarCount(), s2cnf.getClauselCount());

            std::stringstream str;
            str << _workingDir << _satsolverCmd << " " << _tempDir << _dimacsFile << " " << _tempDir << "solution > " << _tempDir << "ausg.txt";
            std::cout << str.str().c_str() << std::endl;
            system( str.str().c_str());
            readSolutionFile();
        }
    }
    std::ofstream data(_solutionfile);
    data << _field;
    if(!_field.isRight()){
        data  << "s UNSATISFIABLE" << std::endl;
        std::cout << "s UNSATISFIABLE";
    }
    data.close();
    return 0;
}

Sudokufield& Solver::getSolution(){
    return _field;
}

void Solver::writeDimacsFile(int varCount, int clausCount)
{
    std::stringstream str;
    str << _tempDir << _dimacsFile;
	ofstream fileOut;
    fileOut.open ( str.str().c_str());
    fileOut << "p cnf " << varCount << " " << clausCount << std::endl;
    fileOut << _cnf.str();
	fileOut.close();
    _cnf.str("");
    _cnf.clear();
}

void Solver::readSolutionFile()
{
    if((_satsolverCmd.find("minisat") != std::string::npos) || (_satsolverCmd.find("glucose") != std::string::npos)){
        readSolutionFileMinisat();
    } else{
        readSolutionFileOther();
    }
}

void Solver::readSolutionFileMinisat(){
    string line;
    std::stringstream str;
    str << _tempDir << "solution";
    ifstream fileIn ( str.str().c_str());
    if (fileIn.is_open())
    {
        getline( fileIn, line );

        if( line == "UNSAT" )
        {
            cout << line << endl;
        }
        else if( line == "SAT" )
        {
            cout << "> read solution.. ";

            getline( fileIn, line );
            vector<string> solution = split( line, ' ');
            for( string s : solution )
            {
                if( s == "0")
                {
                    cout << "done" << endl;
                    break;
                }

                // skip negativ vars
                if( s[0] == '-' ) continue;

                int varAsInt = stoi( s );
                Pos varAsString = Preprocessor::getPosForInt(varAsInt);

                _field( varAsString.getX(), varAsString.getY()) = varAsString.getValue();

            }
        }

        fileIn.close();
    }

    else cerr << "Solver: Unable to open file" << endl;
}

void Solver::readSolutionFileOther(){
    string line;
    ifstream fileIn ( "ausg.txt" );
    while(!fileIn.eof()){

        getline( fileIn, line );
        if(line.empty()) continue;
        if(line[0] == 'c') continue;
        if(line[0] == 's') {
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            if(line.find("unsat") != std::string::npos){
                cout << line << endl;
                break;
            }
        }
        if(line[0] == 'v'){

            line = line.substr(2);
            vector<string> solution = split( line, ' ');
            for( string s : solution )
            {
                if(s.empty()) continue;
                if( s == "0")
                {
                    break;
                }

                // skip negativ vars
                if( s[0] == '-' ) continue;

                int varAsInt = stoi( s );
                Pos varAsString = Preprocessor::getPosForInt(varAsInt);

                _field( varAsString.getX(), varAsString.getY()) = varAsString.getValue();

            }
        }
    }
}

vector<string> Solver::split( std::string &s, char delim) {
    vector<string> elems;
    
    stringstream ss(s);
    string item;
    while ( getline(ss, item, delim) ) {
        elems.push_back(item);
    }
    return elems;
}
