const input = document.getElementById('imageInput');
const img = document.getElementById("my-image");

input.addEventListener('load', function (){
    img.setAttribute('src', input.result);
});
