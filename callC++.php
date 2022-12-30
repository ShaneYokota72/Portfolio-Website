<form action="callC++.php" method="post">
<input type="file" name="img1"><br>
<input type="file" name="img2"><br>
<input type="submit" value="Submit">
</form>

<?php

// Get the command-line arguments from the user
$arg1 = $_POST["img1"];
$arg2 = $_POST["img2"];
echo "$arg1 <br>";
echo "$arg2 <br>";
// Execute the C++ program with the command-line arguments and capture its output
$output = shell_exec("./NumberScan $arg1 $arg2");

// Print the output to the web page
echo "<pre>";
echo $output;
echo "</pre>";

?>
