/*
mazeio.cpp
*/

#include <iostream>
#include <fstream>
#include "mazeio.h"

using namespace std;

/*************************************************
 * read_maze:
 * Read the maze from the given filename into a
 *  2D dynamically allocated array.
 *
 * Return the pointer to that array.
 * Return NULL (a special address) if there is a problem,
 * opening the file or reading in the integer sizes.
 *
 * The first argument is the filename of the maze input.
 *  You should use an ifstream to open and read from this
 *  file.
 *
 * We also pass in two pointers to integers. These are
 * output parameters (i.e. declared by the caller and
 * passed-by-reference for this function to fill in).
 * Fill the integers pointed to by these arguments
 * with the number of rows and columns
 * read (the first two input values).
 *
 *************************************************/
char** read_maze(char* filename, int* rows, int* cols)
{
    // take in the argv[1] as the file 
    ifstream ifile(filename);
    // get rid of the first two value as those are row and col
    int g1, g2;
    ifile >> g1 >> g2;
    // in case the file does not open, cout an error message and reuturn null
    if(ifile.fail()){
      cout << "input unable to open" << endl;
        return NULL;
    }
    // dynamically allocating 2D array
    char** R_maze = new char*[(*rows)];
    for(int i=0; i<(*rows); i++){
      R_maze[i] = new char[(*cols)];
    }
    // taking in the input form the file into the 2D array I just dymamically allocated
    for(int i=0; i<(*rows);i++){
        for(int j=0;j<(*cols); j++){
            char temp;
            ifile >> temp;
            R_maze[i][j] = temp;
        }
    }
    return R_maze;
}

/*************************************************
 * Print the maze contents to the screen in the
 * same format as the input (rows and columns, then
 * the maze character grid).
 *************************************************/
void print_maze(char** maze, int rows, int cols)
{
    // *** You complete **** CHECKPOINT 1
    // loop through the whole loop and print each value out
    // change the printing row every row
    cout << rows << " " << cols << endl;
    for(int i=0; i<(rows);i++){
        for(int j=0;j<(cols); j++){
            cout << maze[i][j];
        }
        cout << endl;
    }
}

