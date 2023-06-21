#!/opt/homebrew/bin/php-cgi
<?php
	session_save_path('/tmp/php');	
	session_start();
	session_unset(); //clear out userid
	session_destroy();  //delete session file
	header_remove("Set-Cookie");
	header('Location: public_page.html');
	echo "redirecting...\n";
	die();
?>
