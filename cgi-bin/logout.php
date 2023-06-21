<?php
session_unset(); //clear out userid
session_destroy();  //delete session file
header('Location: public.php');

?>
