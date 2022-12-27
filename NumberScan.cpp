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
    // dynamically allocating an array
    img_ = new int[imgheight];
    for(int i=0; i<imgheight; i++){
        img_[i] = new int[imgwidth];
    }

    for(int i=0; i<imgheight; i++){
        for(int j=0; j<imgwidth; i++){
            img_[i][j] = pixeldata[i-1*j]
        }
    }
}

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

