<?php 

//$infile = file_get_contents('../locate/backend_data/test.txt');
//$infile_data = array();
//parse_str($infile, $infile_data);

//$imei = $infile_data["imei"];
//$serial = $infile_data["serial"];
//$data = $infile_data["data"];
$imei = $_POST["imei"];
$serial = $_POST["serial"];
$data = $_POST["data"];

$bin_data = hex2bin($data);
$pack_format =
  'Ltime/'.
  'llat/'.
  'llon/'.
  'salt/'.
  'Sspeed/'.
  'Scourse/'.
  'Cthrottle/';
$array = unpack($pack_format, $bin_data);
//var_dump($array);
$time = $array["time"];
$lat_int = $array["lat"];
$lon_int = $array["lon"];
$alt_int = $array["alt"];
$speed_int = $array["speed"];
$course_int = $array["course"];
$throttle = $array["throttle"];

$lat = $lat_int / 1e7;
$lon = $lon_int / 1e7;
$alt = $alt_int / 10.;
$speed = $speed_int / 10.;
$course = $course_int / 10.;

// tranfer file to web
//$fd = fopen("../locate/backend_data/from_client".$serial, 'w') or die("file open error");
$fd = fopen("../locate/backend_data/from_client00", 'w') or die("file open error");
fprintf($fd, "%s,%09.6f,%010.6f,0,0,0,0", $imei, $lat, $lon);
fclose($fd);

// NMEA log
$nmea_file = fopen("../locate/backend_data/".$imei.".nmea", 'a');

$time_string = gmdate("His", $time);
$date_string = gmdate("dmy", $time);

$ns = 'N';
$ew = 'E';
if(lat < 0.)
  $ns = 'S';
if(lon < 0.)
  $ew = 'W';

$lat_nmea = abs(floor($lat) + ($lat - floor($lat)) * 0.6) * 100.;
$lon_nmea = abs(floor($lon) + ($lon - floor($lon)) * 0.6) * 100.;

$nmea_string = sprintf("\$GPGGA,%s,%011.6f,%s,%012.6f,%s,%d,%d,%03.1f,%.1f,M,,M,,*",
                       $time_string, $lat_nmea, $ns, $lon_nmea, $ew, 1, 12, 1.0, $alt);
       
$csum = 0;
$i = 1;
while($nmea_string[$i] != '*') {      
  $csum ^= ord($nmea_string[$i]);
  $i++;
}  
fprintf($nmea_file, "%s%02X\r\n", $nmea_string, $csum);

$rmc_status = 'A';
$nmea_string = sprintf("\$GPRMC,%s,%s,%011.6f,%s,%012.6f,%s,%05.1f,%05.1f,%s,,,%s*",
                       $time_string, $rmc_status, $lat_nmea, $ns, $lon_nmea, $ew, $speed * 3.6 / 1.852, $course,  
                       $date_string, $rmc_status);                    
$csum = 0;
$i = 1;
while($nmea_string[$i] != '*') {      
  $csum ^= ord($nmea_string[$i]);
  $i++;
}          
fprintf($nmea_file, "%s%02X\r\n", $nmea_string, $csum);

fclose($nmea_file);

?>