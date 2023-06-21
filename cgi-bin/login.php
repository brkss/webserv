#!/opt/homebrew/bin/php-cgi
<?php 
	session_save_path('/tmp/php');	
	session_start();
	$_SESSION["id"] = 9876123551;
	header("Location: private_page.php");
?>
