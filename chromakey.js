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
    // for (let i = 0; i < pixelData.length; i += 4) {
    //     const red = pixelData[i];
    //     const green = pixelData[i + 1];
    //     const blue = pixelData[i + 2];
    //     console.log(`Red: ${red}, Green: ${green}, Blue: ${blue}`);
    // }

    //imageparsing(pixelData, mask, width, height);
    //declare the RGB variables to add all the scanned row/column value
    let avgR = 0;
    let avgG = 0;
    let avgB = 0;

    // scan the top row for chromakey search/algorithm
    for(let i=0; i<width*4; i+=4){
        avgR += pixelData[i];
        avgG += pixelData[i+1];
        avgB += pixelData[i+2]; 
    }
    // scan the left col for chromakey search/algorithm
    for(let i=0; i<pixelData.length; i+=width*4){
        avgR += pixelData[i];
        avgG += pixelData[i+1];
        avgB += pixelData[i+2]; 
    }
    // scan the right col for chromakey search/algorithm
    for(let i=(width-1)*4; i<pixelData.length; i+=width*4){
        avgR += pixelData[i];
        avgG += pixelData[i+1];
        avgB += pixelData[i+2];
    }
    // scan the bot row for chromakey search/algorithm
    for(let i=((height-1)*width*4); i<pixelData.length; i+=4){
        avgR += pixelData[i];
        avgG += pixelData[i+1];
        avgB += pixelData[i+2];
    }

    // divide the avg values by the amout of pixel scanned to get the true average value
    avgR = avgR/(2*width + 2*height);
    avgG = avgG/(2*width + 2*height);
    avgB = avgB/(2*width + 2*height);
    // get the avg difference of the RGB values in the image
    let dif = 0.0;
    for(let i=0; i<pixelData.length; i+=4){
        dif += Math.sqrt(Math.pow(avgR-pixelData[i], 2) + Math.pow(avgG-pixelData[i+1], 2) + Math.pow(avgB-pixelData[i+2], 2));
    }
    dif /= width*height;

    for(let i=0; i<pixelData.length; i+=4){
        let distance = Math.sqrt(Math.pow(avgR-pixelData[i], 2) + Math.pow(avgG-pixelData[i+1], 2) + Math.pow(avgB-pixelData[i+2], 2));
        //console.log(distance + "vs" + dif);
        if(dif*1.05 > distance){
            pixelData[i] = 255;
            pixelData[i + 1] = 255;
            pixelData[i + 2] = 255;
        }
    }

    //replace(pixelData, mask);
    ctx.putImageData(imageData, 0, 0);
})
