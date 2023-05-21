#ifndef MTOKEN_H
#define MTOKEN_H

#include <iostream>
#include <string>
using namespace std;

struct MToken
{
    string type;
    string value;
    int line;
};

std::ostream& operator<<(std::ostream& os, const MToken& token);

#endif