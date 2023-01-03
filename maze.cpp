#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <sstream>
#include <string>
#include <emscripten/emscripten.h>


using namespace std;

// Class and struct of Location and Queue

// e.g. position [3][5] in an array would have row 3, col 5
struct Location {
   int row;
   int col;
};

// a class that contains a series of Locations
class Queue {
public:
   
   // constructor. maxlen must be as large as the total number
   // of Locations that will ever be entered into this Queue.
   Queue(int maxlen){
        head = 0;
        tail = 0;
        contents = new Location[maxlen];
    }
   
   // destructor. releases resources. C++ will call it automatically.
   ~Queue(){
   }
   
   // insert a new Location at the end/back of our list   
   void add_to_back(Location loc){
        contents[tail] = loc;
        tail++;
    }

   // return and "remove" the oldest Location not already extracted
   Location remove_from_front(){
        int temp = head;
        head++;
        return contents[temp];
    }


   // is this Queue empty? (did we extract everything added?)
   bool is_empty(){
        return head == tail;
    }

   // member variables of an Queue, for implementation:

   // get the tail value
   int get_tail(){
        return tail;
    }
   // get the head value
   int get_head(){
        return head;
    }
   // get the contents value(address)
   Location* get_contents(){
        return contents;
    }


private:
   Location* contents; // ptr to dynamically-allocated array
   int tail; // how many Locations were added so far?
             // (index of next free item at the end)
   int head; // how many Locations were extracted so far?
             // (index of the first occupied location)
};

// Prototyle of read/print maze functions
// fill in rows and cols
char** read_maze(char* filename, int* rows, int* cols){
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

// print maze to cout
void print_maze(char** maze, int rows, int cols){
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

// Prototype for maze_search, which you will fill in below.
string maze_search(char** maze, int rows, int cols){
    // *** You complete **** CHECKPOINT 4
    Location startloc;
    Location finishloc;
    bool s = false,f = false;
    int s_amount = 0, f_amount = 0;
    // 1) find S and F, and return invalid if not found. If something that is not S,F,#,. is found, return -3, which will ultimately print out the invalid char message
    for(int i=0; i<(rows);i++){
      for(int j=0;j<(cols); j++){ 

        if(maze[i][j] != 'S' && maze[i][j] != 'F' && maze[i][j] != '.' && maze[i][j] != '#'){
          return "-3";
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
            if(s != true || f != true || s_amount != 1 || f_amount !=1){
              return "-1";
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
            return "-2";
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
    string result = "";
    // print out the predecessor array, which should be the maze + the route changed to '*'
    for(int i=0; i<(rows);i++){
        for(int j=0;j<(cols); j++){
            cout << predecessor[i][j];
            result += predecessor[i][j];
        }
        cout << endl;
        result+='\n';
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

    //return 0; // DELETE this stub, it's just for Checkpoint 1 to compile.
    
    return result;
}

int main(){
    cout << "Program will start when you type the maze and click the button" << endl;
    return 0;
}


#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif


// main function to read, solve maze, and print result
EXTERN EMSCRIPTEN_KEEPALIVE char* runmaze(const char* filename, const int r, const int c) {
    cout << "Program RunMaze Started";
    // a string will be the argv
    // e.g. 
    //////////
    // 4 4  //
    // ...s //
    // .##. //
    // .### //
    // ..F. //
    //////////
    // will be inputed as "4 4 ...s .##. .### ..F."
    int rows, cols;
    string result;
    char** mymaze=NULL;
    const char* invalid_char_message = "Error, invalid character.";
    const char* invalid_maze_message = "Invalid maze.";
    const char* no_path_message = "No path could be found!";

    // error message when there isn't a file input
    /* if(argc < 2){
        cout << "Please provide a maze input file" << endl;
        return 1;
    } */


    // taking the argn[1] as the input file, and if it has error opening, print error message
    // take the first two input and print them out as row and column, then close the input file
    string temp = filename;
    // cout << "temp: " << temp << endl;
    stringstream ss(temp);
    // char charrows;
    // char charcols;
    // ss >> charrows >> charcols;
    rows = r;
    cols = c; 

    if(temp.length() == rows*cols) {
        mymaze = new char*[(rows)];
        for(int i=0; i<(rows); i++){
            mymaze[i] = new char[(cols)];
        }

        for(int i=0; i<(rows); i++){
            for(int j=0;j<(cols); j++){
                char temp;
                ss >> temp;
                mymaze[i][j] = temp;
            }
        }
    } else {
        cout << "temp len: " << temp.length() << endl;
        cout << "row: " << rows << " cols: " << cols << " both x: " << rows*cols << endl;
        cout << "The maze does not follow the format. Please try again" << endl;/* 
        char* mazeerror = "Error, invalid character.";
        return mazeerror; */
    }
    


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
    if(result == "0"){
      ;
    } else if(result == "-1"){
        cout << invalid_maze_message << endl;
    } else if(result == "-2"){
        cout << no_path_message << endl;
    } else if(result == "-3"){
        cout << invalid_char_message << endl;
    }
    //================================
    // ADD CODE BELOW 
    // to delete all memory that read_maze allocated: CHECKPOINT 2
    for(int i=0; i< rows; i++){
      delete [] mymaze[i];
    }
    delete [] mymaze;

    /* char* arr[result.length() + 1]; 
	strcpy(arr, result.c_str()); 
    arr[result.length()] = '\0';


    string str = "std::string to char*";
  */
    char* rescast = const_cast<char*>(result.c_str());

    return rescast;
}


