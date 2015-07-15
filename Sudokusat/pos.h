#ifndef POS_H
#define POS_H
#include <iostream>
#include <cmath>
#include <string>
class Pos
{
public:
    Pos();
    Pos(int x, int y);
    Pos(int x, int y, int value);
    Pos(const std::string& t);
    Pos(const Pos& old);
// getter
    int getX()const;
    int getY()const;
    int getValue()const;
// Setter
    void setX(const int x);
    void setY(const int y);
    void setValue(const int value);
// Distanz berechnung
    static float calcDist(const int x1, const int y1, const int x2, const int y2);
    static float calcDist(const Pos& p1, const Pos& p2);
    float calcDist(const Pos& p1)const;
    float calcDist(const int x, const int y)const;
// Operatoren
    Pos& operator=(const Pos& old);
    bool operator==(const Pos& other)const;
    bool operator<(const Pos& other)const;
// tostring
    std::string toString()const;
private:
    int x, y, value;
};

std::ostream& operator<<(std::ostream& out, const Pos& p);

#endif // POS_H
