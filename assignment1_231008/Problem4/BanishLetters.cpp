/*
 * File: BanishLetters.cpp
 * -----------------------
 * This program removes all instances of a set of letters from
 * an input file.
 */

#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <cctype>
//#include "CSC3002OJActive/assignment1/lib.h"
using namespace std;

int banishLetters() {
    string banish; getline(cin, banish);
    bool ban[26]; memset(ban, 0, sizeof(ban));
    for(auto ch:banish)
        if(isalpha(ch))
            ban[tolower(ch)-'a']=true;
    stringstream ss; ss<<cin.rdbuf();
    string s=ss.str();
    for(auto ch:s)
        if(!isalpha(ch)||!ban[tolower(ch)-'a'])
            cout<<ch;
    return 0;
}


/* DO NOT modify this main() part */
/*
 sample output:
 input: S
        this is a testing 1
        this is a testing 2
 output:
        thi i a teting 1
        thi i a teting 2
*/
int main(int argc, char* argv[]) {
    banishLetters();
    return 0;
}
