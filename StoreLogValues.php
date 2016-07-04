<?php

$db_server   = "localhost";
$db_database = "env_logger";
$db_user     = "db_user";
$db_passwd   = "db_passwd";

$hash_key = "123123";  // set shared secret

$timestamp = time();

$temp     = ""; // Values should be -200 ... +1000 (divide by 10!)
$hum      = ""; // Values should be 0 ... 1000 (divide by 10!)
$sensorID = ""; // ID should be 0...99

// Verify range and format of temperature and huminity
//
// Regexp for Temp ([0..9]+.[0..9]+)
// Regexp for Hum  ([0..9]+.[0..9]+)
// Regexp for Sensor ([0..9]+)

// store values to Int (store as tens of a degree 20,5C -> 205)
// store values to Int (store as tens of percentage 65,3% -> 653)

// Authenticate check via hash over values and hash_key
//
//ToDo (Use SHA-256? Check for Arduino-Code first!

// Store into Database
//
// Store sessorID, timestamp, temp, hum, 

// Check for errors and create a mail in case of issues
//


echo "OK"

?>