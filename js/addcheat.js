function ADDCHEAT_ShowHelp(id)
{
	switch(id) 
	{
		case 0:
			document.getElementById("addcheat_help_0").style.display = "block";
			break;
		case 1:
			document.getElementById("addcheat_help_1").style.display = "block";
			break;
		case 2:
			document.getElementById("addcheat_help_2").style.display = "block";
			break;
		case 3:
			document.getElementById("addcheat_help_3").style.display = "block";
			break;
	}
}

function ADDCHEAT_ReallyHideHelp(id)
{
	switch(id) 
	{
		case 0:
			document.getElementById("addcheat_help_0").style.display = "none";
			break;
		case 1:
			document.getElementById("addcheat_help_1").style.display = "none";
			break;
		case 2:
			document.getElementById("addcheat_help_2").style.display = "none";
			break;
		case 3:
			document.getElementById("addcheat_help_3").style.display = "none";
			break;
	}
}

function ADDCHEAT_HideHelp(id)
{
	setTimeout(ADDCHEAT_ReallyHideHelp, 10000, id);
}

function ADDCHEAT_HideError()
{
	document.getElementById("addcheat_form_error").style.display = "none";
	document.getElementById("addcheat_form_error").innerHTML = "n/a";
}

function ADDCHEAT_ShowError(error, inputid)
{
	document.getElementById("addcheat_form_error").innerHTML = error;
	document.getElementById("addcheat_form_error").style.display = "block";
	
	if(inputid != -1)
	{
		var old_color = inputid.style.backgroundColor;
		inputid.addEventListener("click", 
			function()
			{
				inputid.style.backgroundColor = old_color;
			}
		);
		inputid.style.backgroundColor = "#E36868";
	}
	
	setTimeout(ADDCHEAT_HideError, 30000, inputid, old_color);
}


// ADDCHEAT form validation and submission
$(document).ready(function()
{
	$(function() 
	{
		$( ".addcheat_form" ).submit(function( event ) 
		{
			event.preventDefault();
			var cheatname = document.getElementById("addcheat_input_cheatname");
			var cheatdescription = document.getElementById("addcheat_input_cheatdescription");
			var md5checksum = document.getElementById("addcheat_input_md5checksum");
			var cheatsource = document.getElementById("addcheat_input_cheatsource");
			
			// Validation
			if(cheatname.value.length <= 0 || cheatdescription.value.length <= 0 || md5checksum.value.length <= 0 || cheatsource.value.length <= 0)
			{
				ADDCHEAT_ShowError("You must fill in the whole form!", -1);
				return false;
			}
			if(cheatdescription.value.length < 70)
			{
				ADDCHEAT_ShowError("You have written a very short cheat description. Please make sure it's atleast 70 characters. Yours is only " + cheatdescription.value.length + " character(s).", cheatdescription);
				return false;
			}
			if(/[a-f|0-9]{32}/i.test(md5checksum.value) == false)
			{
				ADDCHEAT_ShowError("Invalid MD5 hash. Use some online tool to get a valid MD5 hash of a file!", md5checksum);
				return false;
			}
			
			// Submission
			var dataString = 'cheatname='+ cheatname.value + '&cheatdescription=' + cheatdescription.value + '&md5checksum=' + md5checksum.value.toLowerCase() + '&cheatsource=' + cheatsource.value;
			$.ajax(
			{
				type: "POST",
				url: "php/process_addcheat.php",
				data: dataString,
				success: function() 
				{
				  $('#addcheat_form_container').html("<div class='addcheat_form_success'></div>");
				  $('.addcheat_form_success').html("Successfully reported cheat!");
				},
				error: function()
				{
					ADDCHEAT_ShowError("Something went wrong. Please try again later!", -1);
				}
			});
		});
	});
});