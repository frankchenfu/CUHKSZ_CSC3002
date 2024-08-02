/*
 * File: RemoveComments.cpp
 * ------------------------
 * Prints out a file after removing comments.
 */
// header file for local test
#include <iostream>
#include <iomanip>
#include <fstream>
//#include "CSC3002OJActive/assignment1/RemoveComments.h" //for OJ
#include "RemoveComments.h" //for local test
using namespace std;


/*
 * Function: removeComments
 * Usage: removeComments(is, os);
 * ------------------------------
 * Copies characters from the input stream is to the output stream os,
 * removing any comments it finds.  This program eliminates both the
 * /* and // comment forms but does not check to see if those characters
 * are embedded within strings.
 */

void removeComments(istream & is, ostream & os) {
    char ch; bool flag = false;
    while (is.get(ch)) {
        if (flag) {
            if (ch == '*' && is.peek() == '/') {
                is.get(ch);
                flag = false;
            }
        }
        else {
            if (ch == '/' && is.peek() == '*') {
                is.get(ch);
                flag = true;
            }
            else if (ch == '/' && is.peek() == '/'){
                while (is.get(ch) && ch != '\n');
                os.put('\n');
            }
            else
                os.put(ch);
        }
    }
}


void TestRemoveComments(){
    removeComments(cin, cout);
}

// DO NOT modify the main() function
/*
 * sample output format:
 * input:  test file // this is a comment
           test file /* this is also a comment*/
/* output: test file
           test file
 */
int main(int argc, char* argv[]) {
    TestRemoveComments();
    return 0;
}

