<?php

if (isset($_POST['submit'])) {
  // Check if files were uploaded
  if (isset($_FILES['image1']) && isset($_FILES['image2'])) {
    $image1 = $_FILES['image1'];
    $image2 = $_FILES['image2'];

    // Check if the files are images
    if (exif_imagetype($image1['tmp_name']) && exif_imagetype($image2['tmp_name'])) {
      // Generate a unique file name for the images
      $image1_filename = uniqid('image1_') . '.' . pathinfo($image1['name'], PATHINFO_EXTENSION);
      $image2_filename = uniqid('image2_') . '.' . pathinfo($image2['name'], PATHINFO_EXTENSION);

      // Save the images to the server
      move_uploaded_file($image1['tmp_name'], 'uploaded_images/' . $image1_filename);
      move_uploaded_file($image2['tmp_name'], 'uploaded_images/' . $image2_filename);
    } else {
      echo 'The uploaded files are not images.';
    }
  } else {
    echo 'No files were uploaded.';
  }
}

?>

<!-- HTML form for file uploads -->
<form method="post" enctype="multipart/form-data">
  <label for="image1">Image 1:</label>
  <input type="file" name="image1" id="image1"><br>
  <label for="image2">Image 2:</label>
  <input type="file" name="image2" id="image2"><br>
  <input type="submit" name="submit" value="Submit">
</form>
