<?php
	require_once 'connect.php';
    $uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	// POST DATA: Cheater=%s&CheaterIP=%s&Hardware=%s&Reason=%s&ServerName=%s&Port=%d
	if(isset($_GET['Cheater'])) 
	{
  		$Cheater = $uConnect->real_escape_string($_GET['Cheater']);
	}
	else 
	{
  		$Cheater = $uConnect->real_escape_string($_POST['Cheater']);
	}
	if(isset($_GET['CheaterIP'])) 
	{
  		$CheaterIP = $uConnect->real_escape_string($_GET['CheaterIP']);
	}
	else 
	{
  		$CheaterIP = $uConnect->real_escape_string($_POST['CheaterIP']);
	}
	if(isset($_GET['Hardware'])) 
	{
  		$Hardware = $uConnect->real_escape_string($_GET['Hardware']);
	}
	else 
	{
  		$Hardware = $uConnect->real_escape_string($_POST['Hardware']);
	}
	if(isset($_GET['Reason'])) 
	{
  		$Reason = $uConnect->real_escape_string($_GET['Reason']);
	}
	else 
	{
  		$Reason = $uConnect->real_escape_string($_POST['Reason']);
	}
	if(isset($_GET['ServerName'])) 
	{
  		$ServerName = $uConnect->real_escape_string($_GET['ServerName']);
	}
	else 
	{
  		$ServerName = $uConnect->real_escape_string($_POST['ServerName']);
	}
	if(isset($_GET['Port'])) 
	{
  		$Port = $_GET['Port'];
	}
	else 
	{
  		$Port = $_POST['Port'];
	}
	$IP = $_SERVER['REMOTE_ADDR'];
	$IP = $IP . ":" . $Port;
	
	$today = date("F j, Y, g:i a");
	
	$uConnect->query("INSERT INTO `bans` (`Cheater` ,`CheaterIP` ,`Hardware` ,`Reason` ,`ServerIP` ,`ServerName`, `DateTime`) VALUES ('".$Cheater."',  '".$CheaterIP."',  '".$Hardware."',  '".$Reason."',  '".$IP."',  '".$ServerName."', '".$today."')");;
	
	mysqli_close($uConnect);
?>