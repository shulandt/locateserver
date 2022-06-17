<?php

$outfilename = "backend_data/to_serv";
$content = "";

function writeServData()
{
	global $outfilename;
	global $content;
	$request = 0;
	if(!empty($_POST)) {
		if(isset($_POST['request']))
			$request = $_POST['request'];
		if($request == 1) {
   		    if(isset($_POST['coord']))
	            $content .= $_POST['coord'];
		    $content .= ",";
		    if(isset($_POST['radius']))
	            $content .= $_POST['radius'];
        }			
        if(file_put_contents($outfilename, $content) == false)
	        $info_msg = "error writing data";					
	}	
}

writeServData();

?>	