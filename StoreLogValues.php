<?php
include "StoreLogValues_Config.php"

$timestamp = time();

$temp     = ""; // Values should be -200 ... +1000 (divide by 10!)
$hum      = ""; // Values should be 0 ... 1000 (divide by 10!)
$sensorID = ""; // ID should be 0...99
$vcc      = ""; // Raw value of ADC, unit is 1/1024 Volt, int

if(isset($_GET["temp"])) { $temp = $_GET["temp"]; } else { $temp = "--"; }
if(isset($_GET["hum"] )) { $hum  = $_GET["hum"];  } else { $hum  = "--"; }
if(isset($_GET["vcc"] )) { $vcc  = $_GET["vcc"];  } else { $vcc  = "--"; }

// Verify range and format of temperature, huminity and vcc
if(preg_match("/^[0-9]{1,3}.[0-9]{1,2}+$/", $temp) != 1) { echo "Invalid Temp Value!"; exit(); }
if(preg_match("/^[0-9]{1,3}.[0-9]{1,2}+$/", $hum ) != 1) { echo "Invalid Hum Value!" ; exit(); }
if(preg_match("/^[0-9]{1,5}$/"            , $vcc ) != 1) { echo "Invalid Vcc Value!" ; exit(); }

// Regexp for Sensor ([0..9]+) - Not yet implemented

// a simple log for now
$logfile = fopen("/home/stefan/www/webroot/temp.heinrichsen.net/temp_log.txt","a");
fwrite($logfile, date("Y-m-d H-i-s") . ";$timestamp;$temp;$hum;$vcc;\n");
fclose($logfile);

// Check battery value and send mail in case of empty/weak battery
if($vcc<3000) {
	mail($mail, "Battery low on Temp-Logger", "Hi Admin,\n\nit seems the battery on your TempLogger device is weak. Current voltage is " . ($vcc/1024/3) . ". This should be considered as emtpy battery.\n\nBest regards,\n   Your receiving script.\n");
}

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
