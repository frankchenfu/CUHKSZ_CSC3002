
/* P1:
 * File: reversequeue.cpp
 * ----------------------
 * This file tests the ReverseQueue function.
 */

#include <iostream>
//#include "CSC3002OJActive/assignment2/reversequeue.h" // For OJ test
#include "reversequeue.h" // For local test
#include <stack>
#include <queue>
using namespace std;

/* TODO PART:
 * Function: reverseQueue
 * Usage: reverseQueue(queue);
 * ---------------------------
 * Reverses the order of the elements in a queue.  This
 * implementation does so by using a stack to hold the
 * values as they are being reversed.
 */

void reverseQueue(queue<string> &q) {
	stack<string> s;
	while(!q.empty()){
		s.push(q.front());
		q.pop();
	}
	while(!s.empty()){
		q.push(s.top());
		s.pop();
	}
}

/* TODO PART:
 * Function: listQueue
 * Usage: listQueue(queue);
 * ------------------------
 * Lists the contents of the queue on the standard output stream.
 */

void listQueue(queue<string> &q) {
	cout<<"The queue contains: ";
	queue<string> tmp=q;
	while(!q.empty()){
		cout<<q.front()<<" ";
		q.pop();
	}
	cout<<endl;
	q=tmp;
}

/* DO NOT modify this main() part*/

int main() {
	string str;
	queue<string> line;

	while(cin>>str){
		line.push(str);
	}
	listQueue(line);
	reverseQueue(line);
	listQueue(line);
	return 0;
}
