// get image array from HTML
// use put the info into 2d array
// scan the image and make a Digit Blob
// sort the Digit Blobs
// Classiry the Digit 
class Location{
    row;
    col;
    constructor(){
        this.row = -1;
        this.col = -1;
    }
    constructor(r, c){
        this.row = r;
        this.col = c;
    }
    //Maybe I'll add a operator overloading of <
}
class DigitBlob{
    constructor(){
        img_ = null;
        digit_ = '!'; // dummy value
        bq0_ = 0;
        bq1_ = 0;
        bq2_ = 0;
        bq3_ = 0;
        bq4_ = 0;
        bqd_ = 0;
        euler_ = -2; // dummy value

        // ul_'s Location default constructor already initializes it to -1,-1
        emp = new Location();
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
    constructor(img/* 2D array */, upperleft/* Location Object */, height, width){
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
    classify(){

    }
    getClassification(){

    }

    //private
    #calc_bit_quads(){
        for(let i = ul_.row-1; i<ul_.row + h_; i++){
            for(let j=ul_.col-1; j < ul_.col + w_ ; j++){
                bqcheck = new Location(i,j);
                let b1 = img_[bqcheck.row][bqcheck.col];
                let b2 = img_[bqcheck.row][bqcheck.col+1];
                let b3 = img_[bqcheck.row+1][bqcheck.col];
                let b4 = img_[bqcheck.row+1][bqcheck.col+1];
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
    #calc_euler_number(){
        euler_ = -(bq1_ - bq3_ - 2*bqd_) / 4;
    }
    #calc_centers_of_mass(){
        totalblack = 0;
        vcentroid = 0;
        hcentroid = 0;

        for(let i=0; i<h_ - 1; i++){
            for(let j=0; j<w_ - 1; j++){
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
    #calc_symmetry(){

    }
    /* #calc_aspect_ratio(){

    } */
    /* #calc_thirds(){

    } */
    #calc_corners(){

    }
    /* #calc_vert_lines(){

    } */
    #morepixel(){

    }
    #VHline(){

    }

    //private variables
    #img_;
    #ul_;
    #h_;
    #w_;
    #digit_;
    #bq0_;
    #bq1_;
    #bq2_;
    #bq3_;
    #bq4_;
    #bqd_;
    euler_;
    #vsym;
    #hsym;
    #toptobot;
    #lefttoright;
    #totalblack;
    #vcentroid;
    #hcentroid;

    #Vline;
    #Hline;
    #halfVline;
    #halfHline;
    #topHline;
    #botHline;
    #righttopblack;
}
class NumImg{
    constructor(array){

    }
    findAndCreateDigitBlobs(){

    }

    // private variables
    #sortDigitBlobs(){

    }
    #h_;
    #w_;
    #Blobs;

}