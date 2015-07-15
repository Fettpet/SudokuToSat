TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
SOURCES += main.cpp \
    sudokufield.cpp \
    pos.cpp \
    preprocessor.cpp \
    Parser.cpp \
    sudoku2cnf.cpp \
    solver.cpp \

HEADERS += \
    sudokufield.h \
    pos.h \
    preprocessor.h \
    parser.h \
    Parser.h \
    sudoku2cnf.h \
    solver.h \

OTHER_FILES += \
    install.sh \
    Sudoku.pro.user \
    Makefile \
    glucose-3.0/core/Makefile \
    glucose-3.0/mtl/config.mk \
    glucose-3.0/mtl/template.mk \
    glucose-3.0/simp/Makefile \
    glucose-3.0/utils/Makefile

