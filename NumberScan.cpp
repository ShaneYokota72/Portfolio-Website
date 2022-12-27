// 1. load image
    // a. original does NumImg(filaname) -> open file with readGSBMP -> dynamically allocate to store the info to 2D array
    //    I will be doing it a little bit different. NumImg(array of the pixel info from JS) -> dynamically allocate a 2D array so that the form is the same as BMP file in C++
    
// 2. Find and Create Digit Blobs
// 3. Sort the Digit Blobs
// 4. Classify the Digits
// 5. Since the result is in strings, change them to BigInt
// 6. Calculate the answers with BigInt

int main(int* img1, int* img2, int command)
{
  int debug = command;

  // ===================
  // TO DO: Fill in the arguments to the constructors below
  NumImg img1(argv[1]);
  NumImg img2(argv[2]);
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

