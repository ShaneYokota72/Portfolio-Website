#include <stdio.h>
# include <iostream>

using namespace std; 
struct Location {
    int row;
    int col;
    // Default constructor
    Location() {
        row = -1;
        col = -1;
    }
    // Initializing constructor
    Location(int r, int c) {
        row = r;
        col = c;
    }



};


int main(){
    Location test;
    cout << "row: " << test.row << " col: " << test.col << endl;
}