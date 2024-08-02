/* P1:
 * File: WildcardMatch.cpp
 * -----------------------
 * This program tests the wildcardMatch method, which
 * matches a filename against a pattern containing the wildcard
 * characters * and ?.
 */

#include <iostream>
#include <string>
// #include "CSC3002OJActive/assignment4/WildcardMatch.h" //for OJ test
// #include "WildcardMatch.h" // for local test
using namespace std;

/*
 * Method: wildcardMatch
 * Usage: if (wildcardMatch(filename, pattern)) . . .
 * --------------------------------------------------
 * This method checks to see whether filename matches the pattern,
 * which consists of three types of characters:
 *
 * 1. The character ?, which matches any single character
 * 2. The character *, which matches any string of characters
 * 3. Any other character, which matches only that character
 */

bool wildcardMatch(string filename, string pattern) {
   unsigned ptr = 0;
   for(unsigned i = 0; i < pattern.size(); i++){
      switch (pattern[i]){
         case '?':
            if(ptr >= filename.size())
               return false;
            ptr++;
            break;
         case '*':
            if(i == pattern.size()-1)
               return true;
            while(ptr < filename.size()){
               if(wildcardMatch(filename.substr(ptr), pattern.substr(i+1)))
                  return true;
               ptr++;
            }
            return false;
         default:
            if(ptr >= filename.size() || filename[ptr] != pattern[i])
               return false;
            ptr++;
            break;
      }
   }
   return ptr == filename.size();
}


/* DO NOT modify the main() part */

int main() {
   string in_pair, filename, pattern;
   bool matchResult;
   while(getline(cin,in_pair)){
      int sp = in_pair.find(' ');
      filename = in_pair.substr(0, sp);
      pattern = in_pair.substr(sp+1, in_pair.size());
      matchResult = wildcardMatch(filename, pattern);
      cout << boolalpha  << matchResult << endl;
    }
   return 0;
}

