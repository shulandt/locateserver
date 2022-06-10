<?php

$ctrlname = "backend_data/servinfo";

function readServInfo()
{
	global $ctrlname;
	if(file_exists($ctrlname)){
	    $content = file_get_contents($ctrlname);
	    echo $content;
	}
}

readServInfo();

?>	