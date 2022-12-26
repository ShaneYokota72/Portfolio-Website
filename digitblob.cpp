#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location& other) const
{
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

// TO DO - Complete this function
DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

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

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

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

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob()
{
    // Add code if it is necessary
    /*for(int i=0; i<h_; i++){
      delete [] img_[i];
    }
    delete [] img_;*/
}

// TO DO - Complete this function
void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_centers_of_mass();
    calc_symmetry();
    morepixel();
    VHline();
    calc_corners();
    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number but you are free to
    //    change our structure

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
            if(Vline && Hline/* vsym >= 0.65 */){
                digit_ = '4';
            } else /* if(!Vline && !Hline) */{
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

// Complete - Do not alter
char DigitBlob::getClassification() const
{
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const
{
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

// Complete - Do not alter
const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const
{
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const
{
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number()
{
    euler_ = -(bq1_ - bq3_ - 2*bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads()
{
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

// Add more private helper function implementations below

// claculate the horizontal and vertical symmetry of the image
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

// check the right top corner, if it is black or not
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