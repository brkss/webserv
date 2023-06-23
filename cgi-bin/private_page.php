#!/Users/adriouic/.brew/bin/php-cgi
<?php 
	require_once("session.php");
?> 
<!DOCTYPE html>
<html>
<head>
	<title>session-test-private</title>
</head>
<body>
	<header>
		<h1>Private Page</h1>
	</header>
	<main>
		<p> You can only see this page if you have a valid PHP session on our web server </p>
		<p> Destroy php-session <a href="logout.php">Logout</a></p>
	</main> 
</body>
<html>
