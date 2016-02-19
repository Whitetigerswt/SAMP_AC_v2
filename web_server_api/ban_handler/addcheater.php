<?php
	require_once 'connect.php';
    $uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	// POST DATA: Cheater=%s&CheaterIP=%s&Hardware=%s&Reason=%s&ServerName=%s&Port=%d
	$Cheater = $uConnect->real_escape_string($_REQUEST['Cheater']);
	$CheaterIP = $uConnect->real_escape_string($_REQUEST['CheaterIP']);
	$Hardware = $uConnect->real_escape_string($_REQUEST['Hardware']);
	$Reason = $uConnect->real_escape_string($_REQUEST['Reason']);
	$ServerName = $uConnect->real_escape_string($_REQUEST['ServerName']);
	if(empty($Cheater) || empty($CheaterIP) || empty($Hardware) || empty($Reason) || empty($ServerName) || )
	{
		mysqli_close($uConnect);
		exit();
	}
	$Port = $_REQUEST['Port'];
	if(!is_numeric($Port))
	{
		mysqli_close($uConnect);
		exit();
	}
	$IP = $_SERVER['REMOTE_ADDR'];
	
	$IP = $IP . ":" . $Port;
	$today = date("F j, Y, g:i a");
	$uConnect->query("INSERT INTO `bans` (`Cheater` ,`CheaterIP` ,`Hardware` ,`Reason` ,`ServerIP` ,`ServerName`, `DateTime`) VALUES ('".$Cheater."',  '".$CheaterIP."',  '".$Hardware."',  '".$Reason."',  '".$IP."',  '".$ServerName."', '".$today."')");
	
	mysqli_close($uConnect);
?>