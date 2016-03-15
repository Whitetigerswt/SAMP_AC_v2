<?php

	require_once 'connect.php';
    	$uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	$Hardware = $uConnect->real_escape_string($_REQUEST['Hardware']);
	$IP = $uConnect->real_escape_string($_REQUEST['IP']);
	if(empty($IP) || empty($Hardware))
	{
		mysqli_close($uConnect);
		exit();
	}
	if($result = $uConnect->query("SELECT `SQLid` FROM `bans` WHERE `Hardware` = '". $Hardware ."' OR `CheaterIP` = '". $IP ."'"))
	{
		if($result->num_rows > 0)
		{
			http_response_code(508);
		}
	}
	mysqli_close($uConnect);

?>
