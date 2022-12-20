const input = document.getElementById('imageInput');
    const img = document.getElementById("my-image");
    input.addEventListener('change', function (){
      const file = input.files[0];
      const reader = new FileReader();
      
      reader.onload = function(){
        img.src = reader.result;
      }

      reader.readAsDataURL(file);
    });
    img.addEventListener('load', function(){
      const canvas = document.getElementById('canvas');
      const ctx = canvas.getContext("2d");
      var width = img.clientWidth;
      var height = img.clientHeight;
      canvas.setAttribute('height', height);
      canvas.setAttribute('width', width);
      ctx.drawImage(img, 0, 0);
    })