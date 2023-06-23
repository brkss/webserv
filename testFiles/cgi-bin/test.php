#!/opt/homebrew/bin/php-cgi
<?php
    header("Content-Type: text/html");

/*

    echo "<html><body>\n";
    echo "<h1>Hello, World! " . $_POST["email"] ."</h1>\n";
    echo "</body></html>\n";
*/
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test php</title>
</head>
<body>
    
    <?php
        if(isset($_POST["send"])){
            echo "<b>your email is : ".$_POST["email"]." 🤔🤔</b>";
        }
    ?>
    <form method="POST" action="/test.php"> 
        <input type="text" name="email" />
        <input name="send" type="submit">send</input>
    </form>

</body>
</html>
