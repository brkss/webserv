#!/opt/homebrew/bin/php-cgi
<?php

header("Content-Type: text/html");

echo "<html><body>\n";
echo "<h1>Hello, World! " + $_POST["email"] +"</h1>\n";
echo "</body></html>\n";

?>