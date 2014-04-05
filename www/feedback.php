<?php

function uniq()
{
    // Return the date and the 5 first chars of and MD5 (pseudo unique file name)
    return date('Y-m-d_H-i-s-') . substr(md5(getmypid().uniqid(rand())), 0, 5);
}

$name = $_POST["name"];
$submission_dir = 'feedback/' . uniq() ."_".$name.'/';

if (!mkdir($submission_dir)) {
    // failed to create submission directory
    error_log("ERR 003 \n" . "failed to create submission directory");
    exit;
}

error_log("Start receveing files : " . count($_FILES));
error_log("Start receveing posts : " . count($_POST));
error_log("Start receveing gets : " . count($_GET));

foreach ($_POST as $key => $value) {
		error_log("key : ".$key);
    if($key != "name")
        file_put_contents( $submission_dir . "/$key",  $value);
    
}

$mailContent = "http://www.phonations.com/".$submission_dir;
mail("support@phonations.com", "Feedback report from ".$name, $mailContent);

error_log("success");

?>
