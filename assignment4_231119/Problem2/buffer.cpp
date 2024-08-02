/* P3:
 * File: p3beffer.cpp
 * -------------------
 * This file implements EditorBuffer
 */
#include <iostream>
//#include "CSC3002OJActive/assignment4/buffer.h"
//#include "CSC3002OJActive/assignment4/SimpleTextEditor.h"
#include "buffer.h"
#include "SimpleTextEditor.h"
using namespace std;

/*
 * Implementation notes: EditorBuffer constructor
 * ----------------------------------------------
 * This function initializes an empty editor buffer, represented
 * as a doubly linked list.  In this implementation, the ends of
 * the linked list are joined to form a ring, with the dummy cell
 * at both the beginning and the end.  This representation makes
 * it possible to implement the moveCursorToEnd method in constant
 * time, and reduces the number of special cases in the code.
 */

EditorBuffer::EditorBuffer() {
   start = cursor = new Cell;
   start->next = start;
   start->prev = start;
}

/* TODO PART:
 * Implementation notes: EditorBuffer destructor
 * ---------------------------------------------
 * The destructor must delete every cell in the buffer.  Note
 * that the loop structure is not exactly the standard idiom for
 * processing every cell within a linked list, because it is not
 * legal to delete a cell and later look at its next field.
 */

EditorBuffer::~EditorBuffer() {
   Cell *ptr = nullptr, *i = start->next;
   while (i != start) {
      ptr = i;
      i = i->next;
      delete ptr;
   }
   delete start;
}

/* TODO PART:
 * Implementation notes: cursor movement
 * -------------------------------------
 * In a doubly linked list, each of these operations runs in
 * constant time.
 */

void EditorBuffer::moveCursorForward() {
   if (cursor != start) {
      cursor = cursor -> next;
   }
}

void EditorBuffer::moveCursorBackward() {
   if (cursor -> prev != start) {
      cursor = cursor -> prev;
   }
}

void EditorBuffer::moveCursorToStart() {
   cursor = start -> next;
}

void EditorBuffer::moveCursorToEnd() {
   cursor = start;
}

/* TODO PART:
 * Implementation notes: insertCharacter, deleteCharacter
 * ------------------------------------------------------
 * This code is much like that used for the traditional linked
 * list except that more pointers need to be updated.
 */

void EditorBuffer::insertCharacter(char ch) {
   Cell *ptr = new Cell;
   ptr -> ch = ch;
   ptr -> prev = cursor -> prev;
   ptr -> next = cursor;
   ptr -> prev -> next = ptr;
   cursor -> prev = ptr;
}

void EditorBuffer::deleteCharacter() {
   if (cursor != start) {
      Cell *ptr = cursor;
      cursor -> prev -> next = cursor -> next;
      cursor -> next -> prev = cursor -> prev;
      cursor = cursor -> next;
      delete ptr;
   }
}

/* TODO PART:
 * Implementation notes: getText and getCursor
 * -------------------------------------------
 * The getText method uses the standard linked-list pattern to loop
 * through the cells in the linked list.  The getCursor method counts
 * the characters in the list until it reaches the cursor.
 */

string EditorBuffer::getText() const {
   string str;
   for (Cell *ptr = start -> next; ptr != start; ptr = ptr -> next) {
      str += ptr -> ch;
   }
   return str;
}

int EditorBuffer::getCursor() const {
   int tot = 0;
   for (Cell *ptr = start -> next; ptr != cursor; ptr = ptr -> next) {
      tot++;
   }
   return tot;
}

/* DO NOT modify the main() part */
int main(){
    string cmd;
    EditorBuffer buffer;
    while(getline(cin,cmd)){
        executeCommand(buffer, cmd);
    }        
    return 0;
}