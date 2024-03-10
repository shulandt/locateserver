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
  'Cthrottle/'.
  'Cvoltage/'.
  'cpitch/'.
  'cdummy1/'.
  'cdummy2/'.
  'cdummy3/';
$array = unpack($pack_format, $bin_data);
//var_dump($array);
$time = $array["time"];
$lat_int = $array["lat"];
$lon_int = $array["lon"];
$alt_int = $array["alt"];
$speed_int = $array["speed"];
$course = $array["course"];
$throttle = $array["throttle"];
$voltage_int = $array["voltage"];
$pitch = $array["pitch"];

$lat = $lat_int / 1e7;
$lon = $lon_int / 1e7;
$alt = $alt_int / 10.;
$speed = $speed_int / 10.;
$voltage = $voltage_int / 10.;

$time_string = gmdate("H:i:s", $time);

// tranfer file to web
//$fd = fopen("../locate/backend_data/from_client".$serial, 'w') or die("file open error");
$client_num = "31";
if($serial == "218422")
  $client_num = "01";
else if($serial == "218343")
  $client_num = "02";
else if($serial == "218326")
  $client_num = "03";
else if($serial == "218315")
  $client_num = "04";
else if($serial == "218307")
  $client_num = "05";
else if($serial == "218314")
  $client_num = "06";
else if($serial == "218301")
  $client_num = "07";
else if($serial == "218298")
  $client_num = "08";
else if($serial == "218305")
  $client_num = "09";
else if($serial == "218319")
  $client_num = "10";
else if($serial == "218299")
  $client_num = "11";
else if($serial == "218329")
  $client_num = "12";
else if($serial == "218330")
  $client_num = "13";
else if($serial == "218384")
  $client_num = "14";
else if($serial == "218318")
  $client_num = "15";
else if($serial == "218331")
  $client_num = "16";
else if($serial == "218320")
  $client_num = "17";
else if($serial == "218349")
  $client_num = "18";
else if($serial == "218324")
  $client_num = "19";
else if($serial == "218363")
  $client_num = "20";
else if($serial == "218357")
  $client_num = "21";
else if($serial == "218328")
  $client_num = "22";

$fd = fopen("../locate/backend_data/from_client".$client_num, 'w') or die("file open error");
fprintf($fd, "%s,%09.6f,%010.6f,%.0f,%.1f,%d,%d,%d,%.1f,%s", $imei, $lat, $lon, $alt, $speed, $course, $pitch, $throttle, $voltage, $time_string);
fclose($fd);

// NMEA log
$nmea_file = fopen("../locate/backend_data/".$imei.".nmea", 'a');

$time_string = gmdate("His", $time);
$date_string = gmdate("dmy", $time);

$ns = 'N';
$ew = 'E';
if($lat < 0.)
  $ns = 'S';
if($lon < 0.)
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
$nmea_string = sprintf("\$GPRMC,%s,%s,%011.6f,%s,%012.6f,%s,%05.1f,%03d.0,%s,,,%s*",
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
