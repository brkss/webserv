#!/Users/adriouic/.brew/bin/php-cgi
<?php 
	session_save_path('/tmp/');	
	session_start();
	$_SESSION["id"] = 9876123551;
	header("Location: private_page.php");
?>
