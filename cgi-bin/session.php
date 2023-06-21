<?php
	session_save_path('/tmp/php');	
	if (isset($_COOKIE['PHPSESSID'])) {
	    echo "PHPSESSID cookie is present!";
	} else {
		    echo "PHPSESSID cookie is not present.";
	}
	session_start();
	if ( !isset($_SESSION['id'])) {
		session_destroy();
		header_remove("Set-Cookie");
		header('Location: public_page.html');
		echo "Session Unset\n";
	}
?>
