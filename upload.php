<?php
  // Check if the form has been submitted
  if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Check if the form has been completed correctly
    if (isset($_FILES['image1']) && isset($_FILES['image2'])) {
      // Check if the uploaded files are BMP images
      if ($_FILES['image1']['type'] === 'image/bmp' && $_FILES['image2']['type'] === 'image/bmp') {
        // Set the target directory for the uploaded images
        /* $target_dir = 'images/'; */
        
        // Generate a unique filename for each image
        $image1_filename = uniqid().'.bmp';
        $image2_filename = uniqid().'.bmp';
        
        // Set the target path for each image
        $image1_target_path = /* $target_dir. */$image1_filename;
        $image2_target_path = /* $target_dir. */$image2_filename;
        
        // Try to move the uploaded images to the target directory
        if (move_uploaded_file($_FILES['image1']['tmp_name'], $image1_target_path) && move_uploaded_file($_FILES['image2']['tmp_name'], $image2_target_path)) {
          // The images were uploaded successfully
          echo 'The images were uploaded successfully.';
        } else {
          // There was an error uploading the images
          echo 'There was an error uploading the images.';
        }
      } else {
        // The uploaded files are not BMP images
        echo 'Error: Only BMP images are allowed.';
      }
    } 
  }
?>


<form action="upload.php" method="post" enctype="multipart/form-data">
  <label for="image1">Select BMP image 1:</label>
  <input type="file" name="image1" id="image1" accept=".bmp">
  <br>
  <label for="image2">Select BMP image 2:</label>
  <input type="file" name="image2" id="image2" accept=".bmp">
  <br>
  <input type="submit" value="Upload Images">
</form>

