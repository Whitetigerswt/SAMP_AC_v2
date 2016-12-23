$.ajax({
  url:\'php/loadbanlist.php\',
  complete: function (response) {
	  $(\'#banlistshower\').html(response.responseText);
  },
  error: function () {
	  $(\'#banlistshower\').html(\'Bummer: there was an error!\');
  }
});