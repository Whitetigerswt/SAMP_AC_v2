<?php

	// post data: 'cheatname=&cheatdescription=&cheatsource=';
	$cheatname = filter_input(INPUT_POST | INPUT_GET, 'cheatname', FILTER_SANITIZE_SPECIAL_CHARS);
	$cheatdescription = filter_input(INPUT_POST | INPUT_GET, 'cheatdescription', FILTER_SANITIZE_SPECIAL_CHARS);
	$cheatsource = filter_input(INPUT_POST | INPUT_GET, 'cheatsource', FILTER_VALIDATE_URL);
	
	require_once 'connect.php';
	$uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	$cheatname = $uConnect->real_escape_string($cheatname);
	$cheatdescription = $uConnect->real_escape_string($cheatdescription);
	$cheatsource = $uConnect->real_escape_string($cheatsource);
	
	$uConnect->query("INSERT INTO `cheat_reports` (`CheatName`, `CheatDescription`, `CheatSource`) VALUES ('".$cheatname."', '".$cheatdescription."', '".$cheatsource."')");
	
	mysqli_close($uConnect);
?>