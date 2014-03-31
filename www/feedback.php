<?php
/*
 * Copyright 2008, Torsten Curdt
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

function uniq()
{
    // Return the date and the 5 first chars of and MD5 (pseudo unique file name)
    return date('Y-m-d_H:i:s-') . substr(md5(getmypid().uniqid(rand())), 0, 5);
}

$submission_dir = './feedback/' .  '/' . uniq() . "_" . $_POST['name'] . '/';

if (!mkdir($submission_dir)) {
    // failed to create submission directory
    error_log("ERR 003 \n" . "failed to create submission directory");
    exit;
}

error_log("Start receveing files : " . count($_FILES));
error_log("Start receveing posts : " . count($_POST));
error_log("Start receveing gets : " . count($_GET));

foreach ($_POST as $key => $value) {

    if($key != "name")
        file_put_contents( $submission_dir . "/$key",  $value);
    
}

error_log("success");

?>
