#include <iostream>
#include "mazeio.h"
#include <fstream>
#include "queue.h"
#include <cmath>

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// Add other prototypes here for any functions you wish to use


// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
    int rows, cols, result;
    char** mymaze=NULL;
    const char* invalid_char_message = "Error, invalid character.";
    const char* invalid_maze_message = "Invalid maze.";
    const char* no_path_message = "No path could be found!";

    // error message when there isn't a file input
    if(argc < 2){
        cout << "Please provide a maze input file" << endl;
        return 1;
    }

    // taking the argn[1] as the input file, and if it has error opening, print error message
    // take the first two input and print them out as row and column, then close the input file
    ifstream ifile(argv[1]);
    if(ifile.fail()){
        cout << "file input error" << endl;
        return 1;
    }
    ifile >> rows >> cols;
    cout << rows << " " << cols << endl;
    ifile.close();
    mymaze = read_maze(argv[1], &rows, &cols); // <---TASK: COMPLETE THIS FOR CHECKPOINT 1

    // For checkpoint 2 you should check the validity of the maze
    // You may do so anywhere you please and can abstract that
    // operation with a function or however you like.

    // loop through the input file and if there is any character that is not S,F,.,#, then print out invalid char message
    for(int i=0; i<(rows);i++){
      for(int j=0;j<(cols); j++){
        if(mymaze[i][j] == '.' || mymaze[i][j] == 'S' || mymaze[i][j] == '#' || mymaze[i][j] == 'F'){
            ;
        } else {
            cout << invalid_char_message << endl;
        }
      }
    } 


    //================================
    // When working on Checkpoint 4, you will need to call maze_search
    // and output the appropriate message or, if successful, print
    // the maze.  But for Checkpoint 1, we print the maze, regardless.
    
    // depending on the return of the function maze_search, change the message to print out accordingly
    result = maze_search(mymaze, rows, cols);
    if(result == 0){
      ;
    } else if(result ==-1){
        cout << invalid_maze_message << endl;
    } else if(result == -2){
        cout << no_path_message << endl;
    } else if(result == -3){
        cout << invalid_char_message << endl;
    }
    //================================
    // ADD CODE BELOW 
    // to delete all memory that read_maze allocated: CHECKPOINT 2
    for(int i=0; i< rows; i++){
      delete [] mymaze[i];
    }
    delete [] mymaze;

    return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 * NOTE: don't forget to deallocate memory in here too!
 *************************************************/
int maze_search(char** maze, int rows, int cols)
{
    // *** You complete **** CHECKPOINT 4
    Location startloc;
    Location finishloc;
    bool s = false,f = false;
    int s_amount = 0, f_amount = 0;
    // 1) find S and F, and return invalid if not found. If something that is not S,F,#,. is found, return -3, which will ultimately print out the invalid char message
    for(int i=0; i<(rows);i++){
      for(int j=0;j<(cols); j++){ 

        if(maze[i][j] != 'S' && maze[i][j] != 'F' && maze[i][j] != '.' && maze[i][j] != '#'){
          return -3;
        } else if(maze[i][j] == 'S'){
            startloc.row = i;
            startloc.col = j;
            s = true;
            s_amount++;
        } else if(maze[i][j] == 'F'){
            finishloc.row = i;
            finishloc.col = j;
            f = true;
            f_amount++;
        } 
        if((i == (rows-1)) && (j == (cols-1))){
            if(s == true && f == true && s_amount == 1 && f_amount ==1){
              
                ;
            } else {
              return -1;
            }
        }
      }
    }

    // 2) construct a Dynamically Allocated Memory queue array
    Queue Q1(rows*cols);

    // 3) make a new Dynamically allocated memory array for visited and predecessor
    char** predecessor = new char*[(rows)];
    for(int i=0; i<(rows); i++){
        predecessor[i] = new char[(cols)];
    }
    bool** visited = new bool*[(rows)];
    for(int i=0; i<(rows); i++){
        visited[i] = new bool[(cols)];
    }
    
    // copy maze to predecessor
    for(int i=0; i<rows; i++){
        for(int j=0;j<cols; j++){
            predecessor[i][j] = maze[i][j];
        }
    }
    // change all the bool value in the array false except for the start location
    for(int i=0; i<rows; i++){
        for(int j=0;j<cols; j++){
            visited[i][j] = false;
        }
    }
    visited[startloc.row][startloc.col] = true;

    // 4) breadth-first search(BFS) algorithm
    Location Begin, End;
    Begin = startloc;
    End = finishloc;
    Q1.add_to_back(Begin);
    bool nosolution = true;
    //run while Q1 is not empty
    while(!Q1.is_empty()){
        // the current location will always change to the Location that is first in the waiting queue
        Location current = Q1.remove_from_front();
        // check if North block has been visited, if it is '#', if it is the finish location. If it is none of them, add the North block to the queue.
        // if it is the finish location, break out of the while loop
        if(current.row > 0){
            Location N;
            N.row = current.row - 1;
            N.col = current.col;
            if(maze[N.row][N.col] == '.' && visited[N.row][N.col] == false){
                Q1.add_to_back(N);
                visited[N.row][N.col] = true;
            } else if(N.row == finishloc.row && N.col == finishloc.col){
                nosolution = false;
                break;
            }
        }
        // check if South block has been visited, if it is '#', if it is the finish location. If it is none of them, add the South block to the queue.
        // if it is the finish location, break out of the while loop
        if(current.row < rows-1){
            Location S;
            S.row = current.row + 1;
            S.col = current.col;
            if(maze[S.row][S.col] == '.' && visited[S.row][S.col] == false){
                Q1.add_to_back(S);
                visited[S.row][S.col] = true;
            } else if(S.row == finishloc.row && S.col == finishloc.col){
                nosolution = false;
                break;
            }
        }
        // check if East block has been visited, if it is '#', if it is the finish location. If it is none of them, add the East block to the queue.
        // if it is the finish location, break out of the while loop
        if(current.col < cols-1){
            Location E;
            E.row = current.row;
            E.col = current.col + 1;
            if(maze[E.row][E.col] == '.' && visited[E.row][E.col] == false){
                Q1.add_to_back(E);
                visited[E.row][E.col] = true;
            } else if(E.row == finishloc.row && E.col == finishloc.col){
                nosolution = false;
                break;
            }
        }
        // check if West block has been visited, if it is '#', if it is the finish location. If it is none of them, add the West block to the queue.
        // if it is the finish location, break out of the while loop
        if(current.col > 0){
            Location W;
            W.row = current.row;
            W.col = current.col - 1;
            if(maze[W.row][W.col] == '.' && visited[W.row][W.col] == false){
                Q1.add_to_back(W);
                visited[W.row][W.col] = true;
            } else if(W.row == finishloc.row && W.col == finishloc.col){
                nosolution = false;
                break;
            }
        }
        // if the queue is empty and the solution has not been found, return -2, which will ultimately print no path message
        if(Q1.is_empty() && nosolution){
            return -2;
        } 
    }

    // At this point, we know the solution has been found
    Location temp;
    temp = End;
    // trace back the steps and change the route to '*'
    for(int i=Q1.get_head()-1; i>=0; i--){
        if(abs((temp.row)-(Q1.get_contents()[i].row)+(temp.col)-(Q1.get_contents()[i].col)) == 1){
            if(temp.row == End.row && temp.col == End.col){
                ;
            } else {
                predecessor[temp.row][temp.col] = '*';
            }
            temp.row = Q1.get_contents()[i].row;
            temp.col = Q1.get_contents()[i].col;
        }
    }
    // print out the predecessor array, which should be the maze + the route changed to '*'
    for(int i=0; i<(rows);i++){
        for(int j=0;j<(cols); j++){
            cout << predecessor[i][j];
        }
        cout << endl;
    }

    // delete all the dynamically allocated memories
    for(int i=0; i<(rows); i++){
        delete [] predecessor[i];
    }
    delete [] predecessor;

    for(int i=0; i<(rows); i++){
        delete [] visited[i];
    }
    delete [] visited;

    delete [] Q1.get_contents();

    return 0; // DELETE this stub, it's just for Checkpoint 1 to compile.
}
