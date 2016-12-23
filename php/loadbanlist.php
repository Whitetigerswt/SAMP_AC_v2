<?php

	require_once 'connect.php';
	$uConnect = new mysqli(mysql_host, mysql_user, mysql_pw, mysql_db);
	
	echo 
	'
		<table style="margin:auto; background-color:black; border-radius:5px;">
			<tr>
				<th>Name</th>
				<th>Reason</th>
				<th>Server Name</th>
				<th>Server IP</th>
				<th>Date and time</th>
			</tr>
	';
	$rowColor = 0;
	if($result = $uConnect->query("SELECT `Cheater`, `Reason`, `ServerName`, `ServerIP`, `DateTime` FROM `bans` ORDER BY `SQLid` DESC"))
	{
		if($result->num_rows > 0)
		{
			while($row = $result->fetch_assoc())
			{
				switch($rowColor)
				{
					case 0:
					{
						$rowColor = 1;
						echo
						'
							<tr style="margin:auto; text-align:center; background-color:#333333;">
								<td style="padding:1%;">'.$row['Cheater'].'</td>
								<td style="padding:1%;">'.$row['Reason'].'</td>
								<td style="padding:1%;">'.$row['ServerName'].'</td>
								<td style="padding:1%;">'.$row['ServerIP'].'</td>
								<td style="padding:1%;">'.$row['DateTime'].'</td>
							</tr>
						';
						break;
					}
					case 1:
					{
						$rowColor = 0;
						echo
						'
							<tr style="margin:auto; text-align:center; background-color:#222222;">
								<td style="padding:1%;">'.$row['Cheater'].'</td>
								<td style="padding:1%;">'.$row['Reason'].'</td>
								<td style="padding:1%;">'.$row['ServerName'].'</td>
								<td style="padding:1%;">'.$row['ServerIP'].'</td>
								<td style="padding:1%;">'.$row['DateTime'].'</td>
							</tr>
						';
						break;
					}
				}
			}
		}
	}
	mysqli_free_result($result);
	echo
	'
	</table>
	';
	mysqli_close($uConnect);

?>
