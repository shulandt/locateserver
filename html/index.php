<!doctype html>
<html>
<head>
	<title>Track it ...</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link href="/images/favicon.ico" type="image/x-icon" rel="icon">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">	
	<link href="css/materialize.css" type="text/css" rel="stylesheet" media="screen,projection"/>
	<link href="css/style.css" type="text/css" rel="stylesheet" media="screen,projection"/>
	<link href="css/leaflet.css" type="text/css" rel="stylesheet" media="screen,projection"/>
	<style>
		body {
			padding: 0;
			margin:  0;
		}
		html, body, #map {
			min-height: 500px;
			width: 100%;
			border: 1px solid black;
		}
        .custom-popup .leaflet-popup-tip,
        .custom-popup .leaflet-popup-content-wrapper {
            font-size: 14px;
        }		
	</style>
</head>
<body>
  <nav class="light-blue lighten-1" role="navigation">
    <div class="nav-wrapper container"><a id="logo-container" href="#" class="brand-logo">Logo</a>
      <ul class="right hide-on-med-and-down">
        <li><a href="#">Navbar Link</a></li>
      </ul>

      <ul id="nav-mobile" class="sidenav">
        <li><a href="#">Navbar Link</a></li>
      </ul>
      <a href="#" data-target="nav-mobile" class="sidenav-trigger"><i class="material-icons">menu</i></a>
    </div>
  </nav>
  <div class="section no-pad-bot" id="index-banner">
    <div class="container">
      <h1 class="header center orange-text">Track it</h1>
	  <div class="row center">
		  <div id="clients_content">Active clients: 0</div>
      </div>	  
	  <div class="row center">
		  <div id="client00_content">client 00 disconnected</div>
      </div>
	  <div class="row center">
		  <div id="client01_content">client 01 disconnected</div>
      </div>	  
	  <div class="row center">
		  <div id="map"></div>
	  </div>
    </div>
  </div>
  <footer class="page-footer orange">
      <div class="container">
      Made by <a class="orange-text text-lighten-3" href="http:\\google.com">Anonymous</a>
      </div>
  </footer>

  <!--  Scripts-->
  <script src="js/leaflet.js"></script>
  <script src="js/jquery-3.6.0.min.js"></script>
  <script src="js/materialize.js"></script>
  <script src="js/init.js"></script>
	
  <script>
    const zeroPad = (num, places) => String(num).padStart(places, '0');
    const max_clients = 2;
	  
	var map = L.map('map');
	  
	L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
	  attribution: '',
	  tileSize: 512,
	  maxZoom: 20,
	  zoomOffset: -1,
	  id: 'mapbox/streets-v11',
	  accessToken: 'pk.eyJ1Ijoic2h1bGFuZHQiLCJhIjoiY2ptYWx3Nmx0MDNkNTNrb2EyN2NweTF1aCJ9.pc506oWrm9MaPGo8a5esSg'
	}).addTo(map);
	  
	var marker_icon = L.icon({
	  						   iconUrl: 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABmJLR0QAAAAAAAD5Q7t/AAAACW9GRnMAAADgAAAAAADMbq1MAAAACXBIWXMAAAsTAAALEwEAmpwYAAAI3UlEQVRYw8VXa4hc5Rl+5py5X3d257YzyW4ct27WzZooTXeNNsWYRqJQUEMoqIXSIghVEGNv0FBFan9U1P6otJW2tChGE9ofpRaqWGlNmpobySYxmmSzJnuZmZ377VzmnNPnO7MTbExsYwsOvMzMuXzv873v87zv+wGf8cdxFc9laLc4nK5JTzQ+4U8NZX2xTNLhcKBdXMi1FmbPKqX8Mauj7edz79DmaNb/A8C4Q5K3h7Pj90bHbhzz+f2S0+WGMFmWIABYdKObFoxOB0q7ZRaPv3uycurwHsswXuX7xz8tgChtW+S6tY/Gxj8/5vUHEAkFEesLIRoOIez3IejzEoCEtt5BXdFQabawVGug1mgRSBuLh985WTq671mus5tWvhoAqySXZ0fq5i0PRDLD4UR/FJnkANIDUURDAXjcHkhOJyRJhkMSUeguZZoGqi0V88UKZvMl5IpllM+frX34l1d/Z6rKT/jQuUsdyZdzLvsCT6zcdM/X40PDvpGVGUxkV+KaTBL9kQj8gSC83LnX54fP57N/C0AeD83rRTjoRyoaQSIahtPjRscd9PiGx26qnDkRM3X1CNevfBKAqORyf3/ll7d/M54elNfQ8fiqFYhH++FjCjxeH1xc1E1zuckBRsFJk10ucsLF/y5e71qYABORsP1s04LkXjk6UTpx0GkZnX/Qj3I5AE7afemNX/leNJX2jAxlMDa8AuEwFxG5poNoPIZ4Mo5QXwShSAgB3gva3yGEaH4aCQu9YxCUE26+Ewl6oRkmKi3V4Y5l1pTfO3Sefo7SzJ7T3mc0MjLxSDidCSRiA1iVSsLr9/MJNyzubHw0i2ubVTjnZiCSLvJuXSSR+GPBIcuopTI4NZhCLVeAQZW4PF5kkzHkSlVoyopA//jkI6Xj+/f21NED4BJSS9wwtSYYDCNJAN6AH7rshEZ5DQ+msaaUg/upJ1D969tdeTsu4a8AwHT0b9mC8W//AIf6+rGwWIDDsOAiN1Ik8FK5isH1GxmFA9sp0af4lt4DkApnr7/bzTwHgkEEAgGoDhdMywmfx4+J4UHg8aeR+/0fMPjDnQiMjMDS9UvoLKN++Ajyzz6HeCyO7I+fw+n5PNAxoXVAwnoRIkGbdT/6RtfdXT5x8EW+db4HYENiYnJChNwfDMBi2FTJiZZmIju+Cum/v4XKyy8h9p3HMf3QDky3TBYdww4Ck2GnQpDyho1bsTKfR2XPbsS3fRWRgWHMnJuDbDmYDheJ7LOVkrrp1gkC2MDXdtkAHE73pNMt2O2FTNMkFx0A6cE4NqT70Hnyt9DIav99D2B1IoTkFQpKPwJwfeth5F56GYnXXsYXn/0pLuRKKJUrMETWZFZQStbpZB1xuSdNXesC8LK2C7I4iY6sgWJJJGEcdw8GEN35XSy+thsWQ3xm2zZq27O8549/FhkSva1AJCf3698gMzSMe7d/Da++b2B2rgmLRcu5LF1PNDnRzp/vkjCQHs4KLbuElhkJiSjXUv+j7/wR088/33Un6vz77//XXU6t13F6xw6MpQdx3dRWzMwXWD25PoktUR1B+hQAJPGwP5FJiiIiMTQuWh91HjdVNA4dRIP3h555Bl9oNuGfnITG/8YVTNwL3n47pkwTyYcftkueXKuSyAw764PMCEiSwy7fvkQ6eVGGoqOJixLDLLtkgiBSEU4u1HMooqAvO5GvsGsbRKsFjUQ0GGZ1+ZpMp6JzGjZpLVuytvUAtPILOevasSwbm11MBBCJgDoEQS5i+rHHbOspv/MJoc/v24c3Uyn7t7kMwH7P6m7Csky7abXy87mLAJoL587yRtZ+wnTYOXIQtU6U6kemCusqJxylB0DMDHRqsnaYbN0W+SR8XgSglHLHTMPY3OENw6KZAjvz1N9nL9ixhw6LEmXHs+u99PFOzr8GF9cqFXAtboJMp5ZNdkxV1WHwmq5p6GgqTYNaKhy7CIB63K+rvLmMUOFwMdvUMXr/NzBcrGLmxV9C4/XU2rVIPPgQXAMDsAvFcg+wCyE7YGd2Bu/t3AmFIPrI/uEHH8SZiSmcZjFydDToqgJVaUNXFJjd0e1iL9g7f+DtY6GBeyY03uxwmnl3sQ7v1BjW3bkV1q5dqJVKyK7IoLP1Tsz2JbiLbgxsPjFgom+NzJ+B64UXUCOAGKMnb7kLB/wJXDh1DB06Vqkkrd3CwtG9Yvd7P9qO20xDIjY++SW3KJesem4WJYP4rksNIN4oQptfQPq++5FfvR7Hywby1TaWGgqW6goKtTYKVRVRpmi134HGiRNI3XEHKlO34ch8CeViEfVyETVao1zC3N9e/zmR/0kEvwfA5IWCrmsbI0OfS7i4kJcANFZExJJIT92MxF13IXfrFhzX3My1AZfUDZ9sy4wLsDfUHTLiN65DfPNmVG/ZjLcbJs7PfohWtYLqUgF1RvHD/W9NK0sLT4jCeelAUm7nL2iBzLWb3KGIW+RUDBSFto4FbxiVwWtwtq5DJYGEY8smqmV/s7Xyp4FaW8WppoElpuhooYbZcxeg1KuoFvOoMQpLs2ebi/9880m++EZvIPkoAHFhpnR6ui8yesN62elxOJhcqhFtsrhSb9nsFtcss+tYMFuYuC6YrWsKWvUGzlPixUIe7WqZO8/TllDOL5ozf37lF5Tjz0TpudJMqHBmO1l87+hAcGhk3KLexKxvdHRukFLSNVtKtqldSQlmK6022q0mWo06Giy9rUqJpF2yw14tFlCen+ucef2Vlwy1/XQv9J80FVdMTTlSmD7gkfoSq3kS8uh0pFIdQiEqWayQzTyAoNVscMCo0yqoM8+1cpmhXkKFVuOuxe/c2Q9q59547Vd0/qPLjeX/8WASX3fro0NTm8Y8nJLEKC4GCqc9EbNc22cBllZGSERFSEzovM00qM0a5g7vO8kh9FMdTP79aCbL2/uvX39vat2GMY84monOKRqXTZzudCTS01FVpqJhFo7xaPbB0T3M9/90NLvs4VRMMt6B5EQwfU02wDYuKlFj8UKuwdqu8nBqXuXh9DP//AvNXR2uGqmotgAAAABJRU5ErkJggg==',
							   iconAnchor: [12, 12]
							});
	
    var custom_popup_options =
        {
          offset: [4, 0],
          className : 'custom-popup'
        }
	
	let markerArray = [];
	let markerPopupArray = [];
	let targetArray = [];
	let targetPopupArray = [];
	let clientPresentArray = [];
	var targetSelect = -1;
	var activeClients = 0;
	
	var time_limit = 0;
	var boom = 0;

    map.on("click", function(e) {
	  if(targetSelect >= 0) {  
		targetArray[targetSelect].setLatLng(e.latlng);
		targetArray[targetSelect].setStyle({color: '#ff0000', fillcolor: '#ff0000'});
		putData(targetSelect, 1);
		targetSelect = -1;
      }		  
	});
	  
	map_init();
	
	function map_init()
	{
		map.setView(L.latLng(0., 0.), 12);
		var i;
		for(i = 0; i < max_clients; i++) {
			clientPresentArray[i] = 0;
			markerPopupArray[i] = L.popup(custom_popup_options);
			markerPopupArray[i].setContent('no data');
			markerArray[i] = L.marker([0., 0.], {icon: marker_icon, zIndexOffset: 10000}).bindPopup(markerPopupArray[i]);
			targetPopupArray[i] = L.popup(custom_popup_options);
			targetArray[i] = L.circle([0., 0.], {radius: 1}).bindPopup(targetPopupArray[i]);
			putData(i, 0);
		}			
	}
	
	function set_marker(i, lat, lon, sat, dist, time, battery)
	{
	  var latLng = L.latLng(lat, lon);
	  
	  if(clientPresentArray[i] == 0) {
		  clientPresentArray[i] = 1;
		  activeClients++;
		  $('#clients_content').html('Active clients: ' + activeClients);
		  markerArray[i].addTo(map);
		  targetArray[i].addTo(map);
	  }	  
      markerPopupArray[i].setContent('Lat: <b>' + lat + '</b>' + 
		                             '<br>Lon: <b>' + lon + '</b>' +
						             '<br>Sat: <b>' + sat + '</b>' +
							         '<br>Distance: <b>' + dist + 'm</b>' +
							         '<br>Time to: <b>' + time + 's</b>' +
							         '<br>Battery: <b>' + battery + '%</b>' +
									 '<br><button name="buttonTarget" style="width: 100%" onClick="targetView(' + i + ')">Target view</button>' +
							         '<br><button name="buttonBoom" style="width: 100%" onClick="clientBoom(' + i + ')">Boom!</button>');  
   	  markerArray[i].setLatLng(latLng);
	}
	  
	function set_target(i, lat, lon, radius)
	{
	  var latLng = L.latLng(lat, lon);
	  targetArray[i].setLatLng(latLng).setRadius(radius);
      targetArray[i].setStyle({color: '#3388ff', fillcolor: '#3388ff'});
  
      var popupContent = 
	                     'Lat: <b>' + lat + '</b>' + 
		                 '<br>Lon: <b>' + lon + '</b>' +
						 '<br>Radius: <b>' +
                         '<input name="nameRadius" id="nameRadius" ' +
                         'oninput="javascript: if (this.value.length > this.maxLength) this.value = this.value.slice(0, this.maxLength);" ' +
                         'type = "number" maxlength = "4" min="1" max="9999" ' +
						 'style="font-size: 14px; width: 40px; height: 18px; border: 1px solid black; border-radius: 2px;" ' + 
						 'value=' + radius + '> m </b>' +
						 '<button name="buttonSetRadius" style="float: right" onClick="target_circle_set_radius(' + i + ')">Set</button>' +
						 '<br>Time Limit: <b>' +
                         '<input name="nameTime" id="nameTime" ' +
                         'oninput="javascript: if (this.value.length > this.maxLength) this.value = this.value.slice(0, this.maxLength);" ' +
                         'type = "number" maxlength = "4" min="1" max="9999" ' +
						 'style="font-size: 14px; width: 40px; height: 18px; border: 1px solid black; border-radius: 2px;" ' + 
						 'value=' + time_limit + '> s </b>' +
						 '<button name="buttonSetTime" onClick="target_circle_set_time(' + i + ')">Set</button>' +
						 '<br><button name="buttonMove" style="width: 100%" onClick="target_circle_clk(' + i + ')">Move</button>';
    	  
      targetPopupArray[i].setContent(popupContent);	  
	}
	  
	var intervalId = setInterval(function() { getClientsData();}, 1000);
		
	function getClientsData()
    {
		var i;
		for(i = 0; i < max_clients; i++) {
			getClientData(i);
		}	
    }		
	
	function getClientData(client_num)
	{
	  $.ajax({
		type: "POST",
		data: {num: zeroPad(client_num, 2)},	
		url: 'get_coord.php',
		cache: false,
		success: function(data){
		  if(data.trim() == 'empty') {
			  $('#client' + zeroPad(client_num, 2) + '_content').html('client ' + zeroPad(client_num, 2) + ' disconnected');
			  if(clientPresentArray[client_num] != 0) {
				clientPresentArray[client_num] = 0;
				if(activeClients > 0)
                  activeClients--;
                $('#clients_content').html('Active clients: ' + activeClients);			  
			    markerArray[client_num].removeFrom(map);
				targetArray[client_num].removeFrom(map);
			  }	
		  }
          else {
            $('#client' + zeroPad(client_num, 2) + '_content').html('<a href="javascript:client_content_func(' + client_num + ');">client ' + zeroPad(client_num, 2) + ' connected</a>');			
		    const words = data.split(',');
		    set_marker(client_num, words[0], words[1], words[2], words[3], words[4], words[5]);
			if(words.length > 6)
		      set_target(client_num, words[6], words[7], words[8]);
		    if(words.length > 9)
			  time_limit = words[9].trim();
		  }	
		}
	  });
	}
	
	function putData(client_num, req)
	{
	  var latLng = targetArray[client_num].getLatLng();	
	  var radius = targetArray[client_num].getRadius();  
	  $.ajax({
		type: "POST",  
		url: 'put_coord.php',
		data: {
		  request: req,
          num: zeroPad(client_num, 2),		  
		  coord:  latLng.lat + ',' + latLng.lng,
		  radius: radius,
		  time: time_limit,
		  boom: boom
		},
		cache: false,
		success: true	
	  });		
	}	

    function client_content_func(client_num)
	{
	  map.setView(markerArray[client_num].getLatLng());
	}

    function target_circle_clk(client_num)
	{
	  targetSelect = client_num;	
      targetArray[client_num].setStyle({color: '#aa0000', fillcolor: '#aa0000'});
	  targetPopupArray[client_num].close();
  	}	

    function target_circle_set_radius(client_num)
    {
      var radius = document.getElementById('nameRadius').value;		
	  targetArray[client_num].setRadius(radius);
      targetArray[client_num].setStyle({color: '#ff0000', fillcolor: '#ff0000'});	  
	  putData(client_num, 1);
    }

    function target_circle_set_time(client_num)
    {
      time_limit = document.getElementById('nameTime').value;		
	  putData(client_num, 1);
    }
	
	function clientBoom(client_num)
	{
	  boom = 1;
      putData(client_num, 1);
      boom = 0;	  
	}	

	function targetView(client_num)
	{
      map.setView(targetArray[client_num].getLatLng());
	  targetArray[client_num].setStyle({color: '#00ff00', fillcolor: '#00ff00'});
	  markerPopupArray[client_num].close();
	}
	
	</script>
  </body>
</html>