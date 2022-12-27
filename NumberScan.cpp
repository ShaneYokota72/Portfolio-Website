// 1. load image
    // a. original does NumImg(filaname) -> open file with readGSBMP -> dynamically allocate to store the info to 2D array
    //    I will be doing it a little bit different. NumImg(array of the pixel info from JS) -> dynamically allocate a 2D array so that the form is the same as BMP file in C++

// 2. Find and Create Digit Blobs
// 3. Sort the Digit Blobs
// 4. Classify the Digits
// 5. Since the result is in strings, change them to BigInt
// 6. Calculate the answers with BigInt

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <string>

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
    bool operator<(const Location& other) const{
        // compare the topleft coordinate
        Location temp = *this;
        if(temp.col < other.col){
            return true;
        } else if(temp.col > other.col){
            return false;
        } else if(temp.col == other.col){
            return temp.row < other.row;
        }
        return false;
    }
};

class DigitBlob {
public:
    DigitBlob();
    DigitBlob(uint8_t** img, Location upperleft, int height, int width);

    ~DigitBlob();

    void classify();

    char getClassification() const;

    void printClassificationResults() const;

    const Location& getUpperLeft() const;
    int getHeight() const;
    int getWidth() const;

    bool operator<(const DigitBlob& other);

private:
    void calc_bit_quads();
    void calc_euler_number();
    void calc_centers_of_mass();
    void calc_symmetry();
    void calc_aspect_ratio();
    void calc_thirds();
    void calc_corners();
    void calc_vert_lines();
    void morepixel();
    void VHline();

    uint8_t** img_;
    Location ul_;
    int h_;
    int w_;
    char digit_;
    int bq0_,bq1_,bq2_,bq3_,bq4_,bqd_;
    int euler_;
    double vsym;
    double hsym;
    double toptobot;
    double lefttoright;
    double totalblack;
    double vcentroid;
    double hcentroid;

    bool Vline;
    bool Hline;
    bool halfVline;
    bool halfHline;
    bool topHline;
    bool botHline;
    bool righttopblack;
};

DigitBlob::DigitBlob(){
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2; // dummy value

    // ul_'s Location default constructor already initializes it to -1,-1
    Location emp;
    ul_ = emp;
    // Initilaize h_ and w_ and any other data members
    h_ = w_ = 0;
    Vline = false;
    Hline = false;
    halfHline = false;
    halfVline = false;
    botHline = false;
    topHline = false;
    righttopblack = false;
}

DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width){
    img_ = img;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2; // dummy value

    // Initilaize ul_, h_ and w_ and any other data members
    ul_ = upperleft;
    h_ = height;
    w_ = width;
    Vline = false;
    Hline = false;
    halfHline = false;
    halfVline = false;
    botHline = false;
    topHline = false;
    righttopblack = false;

}

DigitBlob::~DigitBlob(){
    //nothing
}

void DigitBlob::classify(){
    calc_bit_quads();
    calc_euler_number();

    calc_centers_of_mass();
    calc_symmetry();
    morepixel();
    VHline();
    calc_corners();

    // 0 1 2 3 4 5 6 7 8 9 
    // Euler = -1 => 1 2 3 5 7
    // Euler = 0 => 0 4 6 9
    // Euler = 1 => 8
    

    if(euler_ == 1){
        digit_ = '8';
    } else if(euler_ == 0){
        if(vsym > 0.8 && hsym > 0.8){
            digit_ = '0';
        } else if(lefttoright > 1.18){// 1.3 or 1.5
            // 4 or 9
            if(Vline && Hline){
                digit_ = '4';
            } else {
                digit_ = '9';
            }
        } else if(lefttoright < 1){
            // 6
            digit_ = '6';
        }
    } else if(euler_ == -1){
        // 1 2 3 5 7 
        // botHline => 1 or 2
        // topHline => 5 or 7
        // hlafHline=> 1
        // neither => 3

        if (Vline){
            digit_ = '1';
        }  else if((!Vline && !Hline && !halfVline) || hsym>0.75){
            digit_ = '3';
        } else if(botHline){
            if(hsym > 0.65 && vsym > 0.7){
                digit_ = '1';
            } else if(halfHline &&righttopblack){
                digit_ = '1';
            } else {
                digit_ = '2';
            }
        } else if (topHline){
            // 5 or 7
            if(hcentroid < 0.4){
                digit_ = '7';
            } else if (halfVline){
                digit_ = '5';
            }
        } else if(halfVline){
            digit_ = '5';
        }
    }   
    //printClassificationResults();
}
class NumImg {
public:
    NumImg(const int* pixeldata, int imgwidth, int imgheight);
    ~NumImg();
    size_t findAndCreateDigitBlobs();
    string classify(bool withDebug);
    void printBoundingBoxes() const;
    void drawBoundingBoxesAndSave(const char* filename);
    const DigitBlob& getDigitBlob(size_t i) const;
    size_t numDigitBlobs() const;
private:
    void sortDigitBlobs();
    DigitBlob createDigitBlob(bool** explored, int pr, int pc);
    /// Pointer to the 2D dynamically allocated array of pixels
    uint8_t** img_;
    /// Height and width of input image (not always 256x256)
    int h_;
    int w_;
    /// Vector to store the DigitBlobs we find in the image
    std::vector<DigitBlob> blobs_;
    bool** visited;
};



NumImg::NumImg(const int* pixeldata, int imgwidth, int imgheight){
    h_ = imgheight;
    w_ = imgwidth;
    
    // dynamically allocating an array
    img_ = new int[imgheight];
    for(int i=0; i<imgheight; i++){
        img_[i] = new int[imgwidth];
    }

    // dynamically allocating visited array and setting the values to false as default
    visited = new bool[imgheight];
    for(int i=0; i<imgheight; i++){
        visited[i] = new bool[imgwidth];
    }

    // inputting the data getting from JS and putting it into 2d array
    // taking the R value as measurement

    // at the same time, setting visited array to false
    for(int i=0; i<imgheight; i++){
        for(int j=0; j<imgwidth; i++){
            img_[i][j] = pixeldata[((i*imgwidth)+j)*4];
            visited[i][j] = false;
        }
    }

    // Convert to Black and White using a fixed threshold
    for(int i=0; i<imgheight; i++){
        for(int j=0; j<imgwidth; i++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }

}

NumImg::~NumImg(){
    // memory deallocation
    for(int i=0; i<h_; i++){
      delete [] img_[i];
      delete [] visited[i];
    }
    delete [] img_;
    delete [] visited;
}

size_t NumImg::findAndCreateDigitBlobs(){
    // initializing a returning value(about of digit blob found)
    size_t result = 0;

    //run through the image and check for a black pixel, then run BFS
    for (int i=0; i<h_; i++){
      for(int j=0; j<w_; j++){
        if((img_[i][j] == 0) && (visited[i][j] == false)){
          // do the BFS search
          DigitBlob goingtoadd;
          goingtoadd = createDigitBlob(visited, i, j);
          blobs_.push_back(goingtoadd);
          sortDigitBlobs();
          result++;
        }
      }
    }

}

string NumImg::classify(bool withDebug);

void NumImg::printBoundingBoxes() const;

void NumImg::drawBoundingBoxesAndSave(const char* filename);

const NumImg::DigitBlob& getDigitBlob(size_t i) const;

size_t NumImg::numDigitBlobs() const;

int main(int* lhsimg, int* rhsimg, int command)
{
  int debug = command;

  // ===================
  // TO DO: Fill in the arguments to the constructors below
  NumImg img1(lhsimg);
  NumImg img2(rhsimg);
  // ===================
  // TO DO: call findAndCreateDigitBlobs on each img 
  img1.findAndCreateDigitBlobs();
  img2.findAndCreateDigitBlobs();
  // ===================
  // Complete - Do not alter
  if(debug == 1) {
    img1.printBoundingBoxes();
  }
  else if(debug == 2) {
    img2.printBoundingBoxes();
  }
  else if(debug == 3) {
    if(argc < 5){
      cout << "Please provide an extra argument of the filename to save the image." << endl;
    }
    else {
      img1.drawBoundingBoxesAndSave(argv[4]);
    }
  }
  else if(debug == 4) {
    if(argc < 5){
      cout << "Please provide an extra argument of the filename to save the image." << endl;
    }
    else {
      img2.drawBoundingBoxesAndSave(argv[4]);
    }
  }
  else if(debug == 5 || debug == 7) {
    // pass true if debug is 5 and false otherwise (i.e. 7)
    string num = img1.classify( debug == 5 );
    cout << "Image 1 digit string: " << num << endl;
  }
  else if(debug == 6 || debug == 8) {
    // pass true if debug is 6 and false otherwise (i.e. 8)
    string num = img2.classify( debug == 6 );
    cout << "Image 2 digit string: " << num << endl;
  }
  else {
    // Complete - Do not alter - Default full implementation
    string str1 = img1.classify(false);
    string str2 = img2.classify(false);
    
    BigInt num1(str1);
    BigInt num2(str2);
    BigInt sum = num1 + num2;
    cout << num1.to_string() << " + " << num2.to_string() 
          << " = " << sum.to_string() << endl;
    if(num1 < num2){
      BigInt diff = num2 - num1;
      cout << num2.to_string() << " - " << num1.to_string() 
            << " = " << diff.to_string() << endl;
    }
    else {
      BigInt diff = num1 - num2;
      cout << num1.to_string() << " - " << num2.to_string() 
            << " = " << diff.to_string() << endl;
    }
  }

  return 0;
}

