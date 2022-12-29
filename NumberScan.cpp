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
#include <iomanip>
#include <algorithm> 
#include <emscripten/emscripten.h>

using namespace std;

uint8_t** readGSBMP(const char* filename, int& height, int& width)
{
    height = width = 0;
    uint8 type[2];
    //int headersize = 0;

    //int i,j;

    BITMAPFILEHEADER bfh;

    /* file pointer */
    FILE *file;

    /* read input bmp into the data matrix */
    if (!(file=fopen(filename,"rb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        return NULL;
    }

    fread(type, sizeof(unsigned char), 0x2, file);
    if(type[0] != 'B' and type[1] != 'M') {
        cout << "Not a BMP file" << endl;
        return NULL;
    }
    fread(&bfh.bfSize,sizeof(bfh.bfSize),1,file);
    fread(&bfh.bfReserved1,sizeof(bfh.bfReserved1),1,file);
    fread(&bfh.bfReserved2,sizeof(bfh.bfReserved2),1,file);
    fread(&bfh.bfOffBits,sizeof(bfh.bfOffBits),1,file);
    fread(&bfh.biSize,sizeof(bfh.biSize),1,file);
    fread(&bfh.biWidth,sizeof(bfh.biWidth),1,file);
    fread(&bfh.biHeight,sizeof(bfh.biHeight),1,file);

    fread(&bfh.biPlanes,sizeof(bfh.biPlanes),1,file);
    fread(&bfh.biBitCount,sizeof(bfh.biBitCount), 1, file);
    fread(&bfh.biCompression,sizeof(bfh.biCompression),1,file);
    fread(&bfh.biSizeImage,sizeof(bfh.biSizeImage),1,file);

    fread(&bfh.biXPelsPerMeter,sizeof(bfh.biXPelsPerMeter),1, file);
    fread(&bfh.biYPelsPerMeter,sizeof(bfh.biYPelsPerMeter),1, file);
    fread(&bfh.biClrUsed,sizeof(bfh.biClrUsed), 1, file);
    fread(&bfh.biClrImportant, sizeof(uint32),1,file);

    height = bfh.biHeight;
    width = bfh.biWidth;
    //cout << "Read header h,w = " << height << "," << width << endl;
    unsigned char **result=0;
    result = new unsigned char*[bfh.biHeight];
    for(unsigned i=0; i<bfh.biHeight; i++) {
        result[i] = new unsigned char[bfh.biWidth];
    }

   //fseek(file, 10, SEEK_SET);
   //fread(&headersize, sizeof(uint32), 1, file);
   //cout << "Header size is " << bfh.bfOffBits << endl;

    fseek(file, bfh.bfOffBits, SEEK_SET);

    for(unsigned i=0; i<bfh.biHeight; i++) {
        for(unsigned j=0; j<bfh.biWidth; j++) {
            fread( &result[height-1-i][j], sizeof(unsigned char),1,file);
        }
    }
    fclose(file);

    return result;
}

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

char DigitBlob::getClassification() const{
    return digit_;
}

void DigitBlob::printClassificationResults() const{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data
    cout << "vsym: " << vsym << " hsym: " << hsym << endl;
    cout << "top to bot ratio: " << toptobot << " left to right ratio: " << lefttoright << endl; 
    cout << "vcentroid " << vcentroid << " hcentroid " << hcentroid << endl;
    cout << "Vline: " << boolalpha << Vline << " Hline: " << Hline << endl;
    cout << "halfVline: " << boolalpha << halfVline << " halfHline: " << halfHline << endl;
    cout << "tophline: " << boolalpha << topHline << " bothline: " << botHline << endl;
    cout << "right top is black?" << righttopblack << endl;
    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

const Location& DigitBlob::getUpperLeft() const {
    return ul_;
}

int DigitBlob::getHeight() const {
    return h_;
}

int DigitBlob::getWidth() const {
    return w_;
}

bool DigitBlob::operator<(const DigitBlob& other) {
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

void DigitBlob::calc_euler_number() {
    euler_ = -(bq1_ - bq3_ - 2*bqd_) / 4;
}

void DigitBlob::calc_bit_quads() {
    for(int i = ul_.row-1; i<ul_.row + h_; i++){
        for(int j=ul_.col-1; j < ul_.col + w_ ; j++){
            Location bqcheck(i, j);
            int b1 = img_[bqcheck.row][bqcheck.col];
            int b2 = img_[bqcheck.row][bqcheck.col+1];
            int b3 = img_[bqcheck.row+1][bqcheck.col];
            int b4 = img_[bqcheck.row+1][bqcheck.col+1];
            // check which bq it is and add 1 to the matching bq
            if(b1+b2+b3+b4 == 255*4){
                //0 bloc is balck(all white)
                bq0_++;
            } else if(b1+b2+b3+b4 == 255*3){
                //1 bloc is balck
                bq1_++;
            } else if(b1+b2+b3+b4 == 255*2){
                //2 block is black
                if(b1 == b4){
                    //bqd
                    bqd_++;
                } else {
                    //bq2
                    bq2_++;
                }
            } else if(b1+b2+b3+b4 == 255*1){
                //3 block is black
                bq3_++;
            } else if(b1+b2+b3+b4 == 0){
                //4 block is black
                bq4_++;
            }
        }
    }

}

void DigitBlob::calc_symmetry(){
    //virtical symmetry (left to right)
    double vmatch=0;
    for(int i = 0; i<h_; i++){
        for(int j=0; j < w_*0.45 ; j++){
            if (img_[ul_.row + i][ul_.col + j] == img_[ul_.row + i][ul_.col + w_ -1 -j]){
                vmatch++;
            }
        }
    }
    vsym = vmatch/(h_ * w_ / 2);


    //horizontal symmetry (top to bottom)
    double hmatch = 0;
    for(int i = 0; i<h_*0.45; i++){
        for(int j=0; j < w_ ; j++){
            if (img_[ul_.row + i][ul_.col + j] == img_[ul_.row + h_ - 1 - i][ul_.col + j]){
                hmatch++;
            }
        }
    }
    hsym = hmatch/(h_ * w_ / 2);
}

// calculate the ratio of the pixels from left to right and top to bottom
void DigitBlob::morepixel(){
    //virtical symmetry (left to right)
    double leftblack = 0;
    double rightblack = 0;
    for(int i=ul_.row; i<ul_.row+h_-1; i++){
        for(int j=ul_.col; j <ul_.col+w_-1; j++){
            if(img_[i][j] == 0){
                if(j< (ul_.col + ((w_-1)/2) - w_*0.1)){
                    leftblack++;
                } else if(j>=(ul_.col + ((w_-1)/2) + w_*0.1)){
                    rightblack++;
                }
            }
        }
    }
    //cout << "LtoR = " << rightblack/leftblack << endl;
    lefttoright = rightblack/leftblack;

    //horizontal symmetry (top to bottom)
    double topblack = 0;
    double botblack = 0;
    for(int i=ul_.row; i<ul_.row+h_-1; i++){
        for(int j=ul_.col; j <ul_.col+w_-1; j++){
            if(img_[i][j] == 0){
                if(i<(ul_.row + ((h_-1)/2) - h_*0.1)){
                    topblack++;
                } else if(i>=(ul_.row + ((h_-1)/2) + h_*0.1)){
                    botblack++;
                }
            }
        }
    }
    //cout << "TtoB = " << botblack/topblack << endl;
    toptobot = botblack/topblack;
}

// calculate the vertical centroid and horizontal centroid
void DigitBlob::calc_centers_of_mass(){
    totalblack = 0;
    vcentroid = 0;
    hcentroid = 0;

    for(int i=0; i<h_ - 1; i++){
        for(int j=0; j<w_ - 1; j++){
            if(img_[ul_.row + i][ul_.col + j] == 0){
                totalblack++;
                vcentroid+=j;
                hcentroid+=i;
            }
        }  
    }
    vcentroid= vcentroid/(totalblack*(w_-1)); 
    hcentroid= hcentroid/(totalblack*(h_-1)); 
}

// check if they have a horizontal line or vertical lien
void DigitBlob::VHline(){
    for(int i=ul_.row;i<ul_.row + h_ -1; i++){
        int Htest = 0;
        for(int j=ul_.col;j<ul_.col + w_ -1; j++){
            if(Hline){
                break;
            }
            if(img_[i][j] == 0){
                Htest++;
            }
        }
        if(Htest >= w_*0.801){
            //cout << "Htest at " << i << " " << Htest << ":" << w_*0.85 << endl;
            Hline = true;
            if(i < (ul_.row + (h_*0.1))){
                topHline = true;
            } else if(i > (ul_.row + (h_*0.8))){
                botHline = true;
            }
        }
    }

    for(int j=ul_.col;j<ul_.col + w_ -1; j++){
        int Vtest = 0;
        for(int i=ul_.row;i<ul_.row + h_ -1; i++){
            if(Vline){
                break;
            }
            if(img_[i][j] == 0){
                Vtest++;
            }
        }
        if(Vtest >= h_*0.85){
            //cout << "Vtest at " << i << " " << Vtest << ":" << w_*0.85 << endl;
            Vline = true;
        }
    }


    for(int j=ul_.col;j<ul_.col + w_ -1; j++){
        int halfVtest = 0;
        bool starthalfV = false;
        for(int i=ul_.row;i<ul_.row + h_ -1; i++){
            if(halfHline){
                break;
            } else if(img_[i][j] == 0){
                starthalfV = true;
                halfVtest++;
            } else if(starthalfV && img_[i][j] == 255){
                break;
            }
        }
        if(halfVtest >= h_*0.45 && halfVtest <= h_*0.6){
            //cout << "Vtest at " << j << " " << Vtest << ":" << h_*0.85 << endl;
            halfVline = true;
        } 
    }

    for(int i=ul_.row;i<ul_.row + h_ -1; i++){
        int halfHtest = 0;
        bool starthalfH = false;
        for(int j=ul_.col;j<ul_.col + w_ -1; j++){
            if(halfHline){
                break;
            } else if(img_[i][j] == 0){
                starthalfH = true;
                halfHtest++;
            } else if(starthalfH && img_[i][j] == 255){
                break;
            }
        }
        if(halfHtest >= w_*0.45 && halfHtest <= w_*0.6){
            //cout << "Htest at " << i << " " << Htest << ":" << w_*0.85 << endl;
            halfHline = true;
        }
    }
}

void DigitBlob::calc_corners(){
    //double black = 0;
    /*for(int i=ul_.row +h_ -1 -(0.025*h_); i<ul_.row +h_ -1; i++){
        for(int j=ul_.col +w_ -1 -(0.025*w_); j<ul_.col +w_ -1; j++){
            if(img_[i][j] == 0){
                black++;
            }
        }
    }
    if(black/((0.025*h_)*(0.025*w_)) > 0.9){
        righttopblack = true;
    }*/

    righttopblack = (img_[ul_.row][ul_.col + w_ -1] == 0 );

}


class NumImg {
public:
    NumImg(const int* pixeldata, int imgwidth, int imgheight);
    ~NumImg();
    size_t findAndCreateDigitBlobs();
    string classify(bool withDebug);
    void printBoundingBoxes() const;
    /* void drawBoundingBoxesAndSave(const char* filename); */
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



NumImg::NumImg(const char* bmp_filename){
    /* h_ = imgheight;
    w_ = imgwidth;
    
    // dynamically allocating an array
    img_ = new uint8_t*[imgheight];
    for(int i=0; i<imgheight; i++){
        img_[i] = new uint8_t[imgwidth];
    }

    // dynamically allocating visited array and setting the values to false as default
    visited = new bool*[imgheight];
    for(int i=0; i<imgheight; i++){
        visited[i] = new bool[imgwidth];
    }

    // inputting the data getting from JS and putting it into 2d array
    // taking the R value as measurement

    // at the same time, setting visited array to false
    for(int i=0; i<imgheight; i++){
        for(int j=0; j<imgwidth; j++){
            img_[i][j] = pixeldata[((i*imgwidth)+j)*4];
            visited[i][j] = false;
        }
    }

    // Convert to Black and White using a fixed threshold
    for(int i=0; i<imgheight; i++){
        for(int j=0; j<imgwidth; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    } */

    img_ = readGSBMP(bmp_filename, h_, w_);

    //dynamically allocating the visited array;
    visited = new bool*[h_];
    for(int i=0; i<h_; i++){
        visited[i] = new bool[w_];
    }

    for(int i=0; i<h_; i++){
        for(int j=0; j<w_; j++){
            visited[i][j] = false;
        }
    }


    // Leave this check
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
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
          result++;
        }
      }
    }
    sortDigitBlobs();
    return result;
}

DigitBlob NumImg::createDigitBlob(bool** explored, int pr, int pc) {
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    //for efficient for loop in the BFS algorithm
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here
    
    //declare max left,right,top,bottom
    int maxleft = pc;
    int maxright = pc;
    int maxtop = pr;
    int maxbottom = pr;
    //BFS search algorithm
    deque<Location> loc;
    visited[pr][pc] = true;
    Location start(pr,pc);
    loc.push_back(start);
    while(loc.size() != 0){
        Location temp = loc.at(0);
        for(int i=0; i<8; i++){
            if((img_[temp.row+neighbor_row[i]][temp.col+neighbor_col[i]] == 0) && (visited[temp.row+neighbor_row[i]][temp.col+neighbor_col[i]] == false)){
                visited[temp.row+neighbor_row[i]][temp.col+neighbor_col[i]] = true;
                Location addthis(temp.row+neighbor_row[i], temp.col+neighbor_col[i]);
                loc.push_back(addthis);
                if(temp.row+neighbor_row[i]>maxbottom){
                    maxbottom = temp.row+neighbor_row[i];
                }
                if(temp.row+neighbor_row[i]<maxtop){
                    maxtop = temp.row+neighbor_row[i];
                }
                if(temp.col+neighbor_col[i]>maxright){
                    maxright = temp.col+neighbor_col[i];
                }
                if(temp.col+neighbor_col[i]<maxleft){
                    maxleft = temp.col+neighbor_col[i];
                }
            }
        }
        loc.pop_front();
    }

    // with the maxbot,top,right,left info, create the upperleft location
    Location upperleft(maxtop, maxleft);
    int height, width;
    height = maxbottom - maxtop + 1;
    width = maxright - maxleft + 1;

    //make DigitBlob to return
    DigitBlob tempblob(img_, upperleft, height, width);

    return tempblob;
}

string NumImg::classify(bool withDebug){
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}

void NumImg::printBoundingBoxes() const {
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
    }
}

/* void NumImg::drawBoundingBoxesAndSave(const char* filename); */

const DigitBlob& NumImg::getDigitBlob(size_t i) const{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}

size_t NumImg::numDigitBlobs() const {
    return blobs_.size();
}

void NumImg::sortDigitBlobs(){
    // sort(blobs_.begin(), blobs_.end());
    for(unsigned int i=0; i<blobs_.size(); i++){
        int min = ((blobs_.at(i)).getUpperLeft()).row;
        int minidx = i;
        for(unsigned int j=i; j<blobs_.size(); j++){
            if(((blobs_.at(j)).getUpperLeft()).row <= min){
                min = ((blobs_.at(j)).getUpperLeft()).row;
                minidx = j;
            }
        }
        // i and min index swap;
        swap(blobs_.at(i), blobs_.at(minidx));
    }
}

int main(int argc, char* argv[]){
    return 0;
}

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE int NumScan(const char* lhsimg, const char* rhsimg){
    //int debug = command;

    // ===================
    // TO DO: Fill in the arguments to the constructors below
    NumImg img1(lhsimg);
    NumImg img2(rhsimg);
    // ===================
    // TO DO: call findAndCreateDigitBlobs on each img 
    img1.findAndCreateDigitBlobs();
    img2.findAndCreateDigitBlobs();
    // ===================
    string str1 = img1.classify(false);
    string str2 = img2.classify(false);
    cout << "str1: " << str1 << " str2: " << str2 << endl;

    /* BigInt num1(str1);
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
    } */
    /* } */ 

  return 0;
}