<form action="callC++.php" method="post">
<input type="text" name="img1"><br>
<input type="text" name="img2"><br>
<input type="submit">
</form>

<?php

// Get the command-line arguments from the user
$arg1 = $_GET["img1"];
$arg2 = $_GET["img2"];
echo $arg1;
echo $arg2;
// Execute the C++ program with the command-line arguments and capture its output
$output = shell_exec("./NumberScan");

// Print the output to the web page
echo "<pre>";
echo $output;
echo "</pre>";

?>
