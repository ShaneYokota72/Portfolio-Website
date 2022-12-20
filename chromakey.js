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
    var width = img.clientWidth;
    var height = img.clientHeight;
    canvas.setAttribute('height', height);
    canvas.setAttribute('width', width);
    ctx.drawImage(img, 0, 0);
})