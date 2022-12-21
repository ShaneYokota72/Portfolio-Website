// all the values needed from the HTML file
const input = document.getElementById('imageInput');
const img = document.getElementById("my-image");
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext("2d");

// setting the img in the HTML file to the inputed image
input.addEventListener('change', function (){
    const file = input.files[0];
    const reader = new FileReader();
    reader.onload = function(){
    img.src = reader.result;
    }
    reader.readAsDataURL(file);
});

// Apply some change to the file and put it into the canvas on the HTML file
img.addEventListener('load', function(){
    let width = img.clientWidth;
    let height = img.clientHeight;
    canvas.setAttribute('height', height);
    canvas.setAttribute('width', width);
    ctx.drawImage(img, 0, 0);

    // getting the RGB data output on terminal
    const imageData = ctx.getImageData(0, 0, width, height);
    const pixelData = imageData.data;
    for (let i = 0; i < pixelData.length; i += 4) {
        const red = pixelData[i];
        const green = pixelData[i + 1];
        const blue = pixelData[i + 2];
        console.log(`Red: ${red}, Green: ${green}, Blue: ${blue}`);
    }

    const mask = new Array(pixelData.length/4);
    imageparsing(pixelData, mask, width, height);
})

function imageparsing(pixelinfo, maskinfo, width_, height_){
    // make sure the array ratio is correct
    if(pixelinfo.length != (maskinfo.length)*4){
        return console.log("arraysize has wrong ratio");
    }
    //declare the RGB variables to add all the scanned row/column value
    let avgR = 0;
    let avgG = 0;
    let avgB = 0;

    // scan the top row for chromakey search/algorithm
    for(let i=0; i<width_*4; i+=4){
        avgR += pixelinfo[i];
        avgG += pixelinfo[i+1];
        avgB += pixelinfo[i+2];  
    }
    // scan the left col for chromakey search/algorithm
    for(let i=0; i<pixelinfo.length; i+=width_*4){
        avgR += pixelinfo[i];
        avgG += pixelinfo[i+1];
        avgB += pixelinfo[i+2];  
    }
    // scan the right col for chromakey search/algorithm
    for(let i=(width_-1)*4; i<pixelinfo.length; i+=width_*4){
        avgR += pixelinfo[i];
        avgG += pixelinfo[i+1];
        avgB += pixelinfo[i+2]; 
    }
    // scan the top row for chromakey search/algorithm
    for(let i=((height_-1)*width_*4); i<width_*4; i+=4){
        avgR += pixelinfo[i];
        avgG += pixelinfo[i+1];
        avgB += pixelinfo[i+2];  
    }

    // divide the avg values by the amout of pixel scanned to get the true average value
    avgR = avgR/(2*width_ + 2*height_);
    avgG = avgG/(2*width_ + 2*height_);
    avgB = avgB/(2*width_ + 2*height_);

    // get the avg difference of the RGB values in the image
    let dif = 0.0;
    for(let i=0; i<pixelinfo.length; i+=4){
        dif += Math.sqrt(Math.pow(avgR-pixelData[i], 2) + Math.pow(avgG-pixelData[i+1], 2) + Math.pow(avgB-pixelData[i+2], 2));
    }
    dif /= Math.pow((pixelinfo.length/4), 2);

    for(let i=0; i<pixelinfo.length/4; i+=4){
        let distance = 0;
        distance = Math.sqrt(Math.pow(avgR-pixelData[i], 2) + Math.pow(avgG-pixelData[i+1], 2) + Math.pow(avgB-pixelData[i+2], 2));

        if(dif*1.05 > distance){
            mask[(i+4)/4] = false;
        } else if(dif*1.05 <= distance) {
            mask[i(i+4)/4] = true;
        }
    }
}