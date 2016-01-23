<?php

	require_once 'connect.php';
    $uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	if(isset($_GET['Hardware'])) 
	{
  		$Hardware = $uConnect->real_escape_string($_GET['Hardware']);
	}
	else 
	{
  		$Hardware = $uConnect->real_escape_string($_POST['Hardware']);
	}
	if(isset($_GET['IP'])) 
	{
  		$IP = $uConnect->real_escape_string($_GET['IP']);
	}
	else 
	{
  		$IP = $uConnect->real_escape_string($_POST['IP']);
	}
	if($result = $uConnect->query("SELECT `SQLid` FROM `bans` WHERE `Hardware` = '". $Hardware ."' OR `CheaterIP` = '". $IP ."'"))
	{
		if($result->num_rows > 0)
		{
			http_response_code(508);
		}
	}
	var_dump(http_response_code());
	mysqli_close($uConnect);

?>