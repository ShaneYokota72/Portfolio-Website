<?php

// Check that the form was submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Get the form data
  $email = $_POST['email'];
  $firstName = $_POST['first-name'];
  $lastName = $_POST['last-name'];
  $message = $_POST['message'];

  // Set the recipient email address
  $to = "shaneyok@usc.edu";

  // Set the email subject
  $subject = "New Message from $firstName $lastName";

  // Build the email content
  $emailContent = "Email: $email\n";
  $emailContent .= "First Name: $firstName\n";
  $emailContent .= "Last Name: $lastName\n";
  $emailContent .= "Message:\n$message\n";

  // Send the email
  if (mail($to, $subject, $emailContent)) {
    // Set a 200 (okay) response code.
    http_response_code(200);
    echo "Thank you for your message. I'll be in touch soon.";
  } else {
    // Set a 500 (internal server error) response code.
    http_response_code(500);
    echo "Oops! Something went wrong and we couldn't send your message.";
  }
}

?>
