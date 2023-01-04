<form action="callC++.php" method="post" enctype="multipart/form-data">
    <label for="file_input1">Select a file to upload:</label><br>
    <input type="file" id="file_input1" name="file_input1"><br><br>
    <label for="file_input2">Select another file to upload:</label><br>
    <input type="file" id="file_input2" name="file_input2"><br><br>
    <input type="submit" value="Submit">
</form> 


<?php
    // Check if the first file has been uploaded
    if (isset($_FILES["file_input1"]) && $_FILES["file_input1"]["error"] == UPLOAD_ERR_OK) {
      // Get the uploaded file data for the first file
      $uploaded_file1 = $_FILES["file_input1"];
    
      // Generate a unique file name for the first uploaded file
      $file_name1 = uniqid() . "." . pathinfo($uploaded_file1["name"], PATHINFO_EXTENSION);
    
      // Move the first uploaded file to a specified location
      move_uploaded_file($uploaded_file1["tmp_name"], "uploads/$file_name1");
    
      // Check if the second file has been uploaded
      if (isset($_FILES["file_input2"]) && $_FILES["file_input2"]["error"] == UPLOAD_ERR_OK) {
        // Get the uploaded file data for the second file
        $uploaded_file2 = $_FILES["file_input2"];
    
        // Generate a unique file name for the second uploaded file
        $file_name2 = uniqid() . "." . pathinfo($uploaded_file2["name"], PATHINFO_EXTENSION);
    
        // Move the second uploaded file to a specified location
        move_uploaded_file($uploaded_file2["tmp_name"], "uploads/$file_name2");
    
        // Execute the C++ program with the two files as command-line arguments and capture its output
        $output = shell_exec("./NumberScan uploads/$file_name1 uploads/$file_name2");
    
        // Print the output to the web page
        echo "<pre>";
        echo $output;
        echo "</pre>";
    
        // Delete the second file from the server
        unlink("uploads/$file_name2");
      }
      // Delete the first file from the server
      unlink("uploads/$file_name1");
    }
?>
