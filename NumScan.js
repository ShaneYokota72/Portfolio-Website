// get image array from HTML
// use put the info into 2d array
// scan the image and make a Digit Blob
// sort the Digit Blobs
// Classiry the Digit 
function transformArray(array, width) {
    const matrix = new Array(array.length / width).fill().map(() => new Array(width));
    let row = 0;
    let col = 0;
    for (let i = 0; i < array.length; i++) {
        matrix[row][col] = array[i];
        col++;
        if (col === width) {
            col = 0;
            row++;
        }
    }
    return matrix;
}
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
    constructor(img/* 2D array */, upperleft/* Location Object */, height, width){
        this.#img_ = img;
        this.#digit_ = '!'; // dummy value
        this.#bq0_ = 0;
        this.#bq1_ = 0;
        this.#bq2_ = 0;
        this.#bq3_ = 0;
        this.#bq4_ = 0;
        this.#bqd_ = 0;
        this.#euler_ = -2; // dummy value

        // Initilaize ul_, h_ and w_ and any other data members
        this.#ul_ = upperleft;
        this.#h_ = height;
        this.#w_ = width;
        this.#Vline = false;
        this.#Hline = false;
        this.#halfHline = false;
        this.#halfVline = false;
        this.#botHline = false;
        this.#topHline = false;
        this.#righttopblack = false;
    }
    classify(){
        this.#calc_bit_quads();
        this.#calc_euler_number();
        this.#calc_centers_of_mass();
        this.#calc_symmetry();
        this.#morepixel();
        this.#VHline();
        this.#calc_corners();

        if(this.#euler_ == 1){
            this.#digit_ = '8';
        } else if(this.#euler_ == 0){
            if(this.#vsym > 0.8 && this.#hsym > 0.8){
                this.#digit_ = '0';
            } else if(this.#lefttoright > 1.18){// 1.3 or 1.5
                // 4 or 9
                if(this.#Vline && this.#Hline){
                    this.#digit_ = '4';
                } else {
                    this.#digit_ = '9';
                }
            } else if(this.#lefttoright < 1){
                // 6
                this.#digit_ = '6';
            }
        } else if(this.#euler_ == -1){
            // 1 2 3 5 7 
            // botHline => 1 or 2
            // topHline => 5 or 7
            // hlafHline=> 1
            // neither => 3
    
            if (this.#Vline){
                this.#digit_ = '1';
            }  else if((!this.#Vline && !this.#Hline && !this.#halfVline) || this.#hsym>0.75){
                this.#digit_ = '3';
            } else if(this.#botHline){
                if(this.#hsym > 0.65 && this.#vsym > 0.7){
                    this.#digit_ = '1';
                } else if(this.#halfHline &&this.#righttopblack){
                    this.#digit_ = '1';
                } else {
                    this.#digit_ = '2';
                }
            } else if (this.#topHline){
                // 5 or 7
                if(this.#hcentroid < 0.4){
                    this.#digit_ = '7';
                } else if (this.#halfVline){
                    this.#digit_ = '5';
                }
            } else if(this.#halfVline){
                this.#digit_ = '5';
            }
        }   

    }
    getClassification(){
        return this.#digit_;
    }

    //private
    #calc_bit_quads(){
        for(let i = this.#ul_.row-1; i<this.#ul_.row + this.#h_; i++){
            for(let j= this.#ul_.col-1; j < this.#ul_.col + this.#w_ ; j++){
                bqcheck = new Location(i,j);
                let b1 = img_[bqcheck.row][bqcheck.col];
                let b2 = img_[bqcheck.row][bqcheck.col+1];
                let b3 = img_[bqcheck.row+1][bqcheck.col];
                let b4 = img_[bqcheck.row+1][bqcheck.col+1];
                // check which bq it is and add 1 to the matching bq
                if(this.#b1+this.#b2+this.#b3+this.#b4 == 255*4){
                    //0 bloc is balck(all white)
                    this.#bq0_++;
                } else if(this.#b1+this.#b2+this.#b3+this.#b4 == 255*3){
                    //1 bloc is balck
                    this.#bq1_++;
                } else if(this.#b1+this.#b2+this.#b3+this.#b4 == 255*2){
                    //2 block is black
                    if(b1 == b4){
                        //bqd
                        this.#bqd_++;
                    } else {
                        //bq2
                        this.#bq2_++;
                    }
                } else if(this.#b1+this.#b2+this.#b3+this.#b4 == 255*1){
                    //3 block is black
                    this.#bq3_++;
                } else if(this.#b1+this.#b2+this.#b3+this.#b4 == 0){
                    //4 block is black
                    this.#bq4_++;
                }
            }
        }
    }
    #calc_euler_number(){
        this.#euler_ = -(this.#bq1_ - this.#bq3_ - 2*this.#bqd_) / 4;
    }
    #calc_centers_of_mass(){
        this.#totalblack = 0;
        this.#vcentroid = 0;
        this.#hcentroid = 0;

        for(let i=0; i<this.#h_ - 1; i++){
            for(let j=0; j<this.#w_ - 1; j++){
                if(img_[this.#ul_.row + i][this.#ul_.col + j] == 0){
                    this.#totalblack++;
                    this.#vcentroid+=j;
                    this.#hcentroid+=i;
                }
            }  
        }
        this.#vcentroid= this.#vcentroid/(this.#totalblack*(this.#w_-1)); 
        this.#hcentroid= this.#hcentroid/(this.#totalblack*(this.#h_-1)); 
    }
    #calc_symmetry(){
        let vmatch=0;
        for(let i = 0; i<this.#h_; i++){
            for(let j=0; j < this.#w_*0.45 ; j++){
                if (img_[(this.#ul_).row + i][(this.#ul_.col) + j] == img_[(this.#ul_).row + i][(this.#ul_).col + this.#w_ -1 -j]){
                    vmatch++;
                }
            }
        }
        this.#vsym = this.#vmatch/(this.#h_ * this.#w_ / 2);


        //horizontal symmetry (top to bottom)
        let hmatch = 0;
        for(let i = 0; i<this.#h_*0.45; i++){
            for(let j=0; j < this.#w_ ; j++){
                if (img_[(this.#ul_).row + i][(this.#ul_.col) + j] == img_[(this.#ul_).row + this.#h_ - 1 - i][(this.#ul_).col + j]){
                    hmatch++;
                }
            }
        }
        this.#hsym = this.#hmatch/(this.#h_ * this.#w_ / 2);
    }
    /* #calc_aspect_ratio(){

    } */
    /* #calc_thirds(){

    } */
    #calc_corners(){
        this.#righttopblack = (img_[(this.#ul_).row][(this.#ul_).col + this.#w_ -1] == 0 );
    }
    /* #calc_vert_lines(){

    } */
    #morepixel(){
        //virtical symmetry (left to right)
        let leftblack = 0;
        let rightblack = 0;
        for(let i=(this.#ul_).row; i<(this.#ul_).row+this.#h_-1; i++){
            for(let j=(this.#ul_).col; j <(this.#ul_).col+this.#w_-1; j++){
                if(img_[i][j] == 0){
                    if(j< ((this.#ul_).col + ((this.#w_-1)/2) - this.#w_*0.1)){
                        leftblack++;
                    } else if(j>=((this.#ul_).col + ((this.#w_-1)/2) + this.#w_*0.1)){
                        rightblack++;
                    }
                }
            }
        }
        //cout << "LtoR = " << rightblack/leftblack << endl;
        lefttoright = rightblack/leftblack;

        //horizontal symmetry (top to bottom)
        let topblack = 0;
        let botblack = 0;
        for(let i=(this.#ul_).row; i<(this.#ul_).row+this.#h_-1; i++){
            for(let j=(this.#ul_).col; j <(this.#ul_).col+this.#w_-1; j++){
                if(img_[i][j] == 0){
                    if(i<((this.#ul_).row + ((this.#h_-1)/2) - this.#h_*0.1)){
                        topblack++;
                    } else if(i>=((this.#ul_).row + ((this.#h_-1)/2) + this.#h_*0.1)){
                        botblack++;
                    }
                }
            }
        }
        //cout << "TtoB = " << botblack/topblack << endl;
        this.#toptobot = this.#botblack/this.#topblack;
    }
    #VHline(){
        for(let i=(this.#ul_).row;i<(this.#ul_).row + this.#h_ -1; i++){
            let Htest = 0;
            for(let j=(this.#ul_).col;j<(this.#ul_).col + this.#w_ -1; j++){
                if(this.#Hline){
                    break;
                }
                if(img_[i][j] == 0){
                    Htest++;
                }
            }
            if(Htest >= this.#w_*0.801){
                //cout << "Htest at " << i << " " << Htest << ":" << w_*0.85 << endl;
                this.#Hline = true;
                if(i < ((this.#ul_).row + (this.#h_*0.1))){
                    this.#topHline = true;
                } else if(i > ((this.#ul_).row + (this.#h_*0.8))){
                    this.#botHline = true;
                }
            }
        }
    
        for(let j=(this.#ul_).col;j<(this.#ul_).col + this.#w_ -1; j++){
            let Vtest = 0;
            for(let i=(this.#ul_).row;i<(this.#ul_).row + this.#h_ -1; i++){
                if(this.#Vline){
                    break;
                }
                if(img_[i][j] == 0){
                    Vtest++;
                }
            }
            if(Vtest >= this.#h_*0.85){
                //cout << "Vtest at " << i << " " << Vtest << ":" << w_*0.85 << endl;
                this.#Vline = true;
            }
        }
    
    
        for(let j=(this.#ul_).col;j<(this.#ul_).col + this.#w_ -1; j++){
            let halfVtest = 0;
            let starthalfV = false;
            for(let i=(this.#ul_).row;i<(this.#ul_).row + this.#h_ -1; i++){
                if(this.#halfHline){
                    break;
                } else if(img_[i][j] == 0){
                    starthalfV = true;
                    halfVtest++;
                } else if(starthalfV && img_[i][j] == 255){
                    break;
                }
            }
            if(halfVtest >= this.#h_*0.45 && halfVtest <= this.#h_*0.6){
                //cout << "Vtest at " << j << " " << Vtest << ":" << h_*0.85 << endl;
                this.#halfVline = true;
            } 
        }
    
        for(let i=(this.#ul_).row;i<(this.#ul_).row + this.#h_ -1; i++){
            let halfHtest = 0;
            let starthalfH = false;
            for(let j=(this.#ul_).col;j<(this.#ul_).col + this.#w_ -1; j++){
                if(this.#halfHline){
                    break;
                } else if(img_[i][j] == 0){
                    starthalfH = true;
                    halfHtest++;
                } else if(starthalfH && img_[i][j] == 255){
                    break;
                }
            }
            if(halfHtest >= this.#w_*0.45 && halfHtest <= this.#w_*0.6){
                //cout << "Htest at " << i << " " << Htest << ":" << w_*0.85 << endl;
                this.#halfHline = true;
            }
        }
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
    constructor(array, height, width){
        // instead of filename, I will be just taking an 2D array
        this.#h_ = height;
        this.#w_ = width;

        // inputting the data getting from JS and putting it into 2d array
        // taking the R value as measurement
        const matrix = transformArray(array, 3);
        img_ = matrix;

        // at the same time, setting visited array to false
        for(let i=0; i<imgheight; i++){
            for(let j=0; j<imgwidth; j++){
                img_[i][j] = pixeldata[((i*imgwidth)+j)*4];
                visited[i][j] = false;
            }
        }

        // Convert to Black and White using a fixed threshold
        for(let i=0; i<imgheight; i++){
            for(let j=0; j<imgwidth; j++){
                if(img_[i][j] > 150){
                    img_[i][j] = 255;
                }
                else {
                    img_[i][j] = 0;
                }
            }
        }
    }
    findAndCreateDigitBlobs(){
// 
    }
    classify(DebugNum){

    }
    // private variables
    #sortDigitBlobs(){

    }
    #createDigitBlob(explored, pr, pc){

    }
    #img_;
    #h_;
    #w_;
    #Blobs;
    #visited;
}