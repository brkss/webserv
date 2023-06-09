#!/opt/homebrew/bin/php-cgi
<?php

header("Content-Type: text/html");

$requestBody = file_get_contents('php://stdin');

//$requestBody = "testing";

$postData = file_get_contents('php://input');
$parsedData = [];
parse_str($postData, $parsedData);
$parsedData = json_decode($postData, true);

echo $parsedData;

echo "\n";

echo "<html><body>\n";
echo "<h1>Hello, World! " . $_POST["email"] ."</h1>\n";
echo "<h2>".$_SERVER['CONTENT_LENGTH']."</h2>";
echo "</body></html>\n";

?>