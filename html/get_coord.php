<?php

$ctrlname = "backend_data/from_client";
$content = "empty";

function readServInfo()
{
	global $ctrlname;
	if(!empty($_POST)) {
		if(isset($_POST['num'])) {
			$num = $_POST['num'];
			$ctrlname .= $num;
   	        if(file_exists($ctrlname))
	            $content = file_get_contents($ctrlname);
        }
    }
    echo $content;	
}

readServInfo();

?>	