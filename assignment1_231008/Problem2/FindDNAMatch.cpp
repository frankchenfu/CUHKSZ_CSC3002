/* Q2: 
 * File: FindDNAMatch.cpp
 * ----------------------
 * This file solves the DNA matching exercise from the text.
 */
// header file for OJ system
// #include <iostream>
// #include <string>

// using namespace std;

#include <iostream>
#include <string>
#include <stdexcept>
//#include "CSC3002OJActive/assignment1/FindDNAMatch.h" //for OJ
#include "FindDNAMatch.h" //for local test
using namespace std;


/*
 * Function: findDNAMatch
 * Usage: int pos = findDNAMatch(s1, s2);
 *        int pos = findDNAMatch(s1, s2, start);
 * ---------------------------------------------
 * Returns the first index position at which strand s1 would bind to
 * the strand s2, or -1 if no such position exists.  If the start
 * parameter is supplied, the search begins at that index position.
 */

int findDNAMatch(string s1, string s2, int start) {
   try{
      s1 = matchingStrand(s1);
   }
   catch(invalid_argument &e){
      return -1;
   }
   for(int i = start; i < s2.length() - s1.length() + 1; i++)
      if(s1 == s2.substr(i, s1.length()))
         return i;
   return -1;
}

/*
 * Function: matchingStrand
 * Usage: string match = matchingStrand(strand);
 * ---------------------------------------------
 * Returns a string in which each base has been replaced by its
 * counterpart base (C <-> G and A <-> T).  Any other characters
 * are replaced by an X.
 */

string matchingStrand(string strand) {
   for(int i = 0; i < strand.length(); i++) {
      switch(strand[i]) {
         case 'A': strand[i] = 'T'; break;
         case 'C': strand[i] = 'G'; break;
         case 'G': strand[i] = 'C'; break;
         case 'T': strand[i] = 'A'; break;
         default: throw invalid_argument("Invalid DNA strand");
      }
   }
   return strand;
}

/*
 * Function: findAllMatches
 * Usage: findAllMatches(s1, s2);
 * ------------------------------
 * Finds all positions at which s1 can bind to s2.
 */

void findAllMatches(string s1, string s2) {
   bool found = false;
   int pos = 0;
   while((pos = findDNAMatch(s1, s2, pos)) != -1) {
      found = true;
      cout << s1 << " matches " << s2 << " at position " << pos << endl;
      pos++;
   }
   if(!found)
      cout << s1 << " has no matches in " << s2 << endl;
}

// DO NOT modify the main() function
/*
 * sample output:
 * input:  TTGCC TAACGGTACGTC
 * output: TTGCC matches TAACGGTACGTC at position 1
*/
int main(int argc, char* argv[]) {
    string s1, s2;
    cin >> s1 >> s2; 
    findAllMatches(s1, s2);
    return 0;
}
