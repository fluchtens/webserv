<?php
$serverSoftware = $_SERVER['SERVER_SOFTWARE'];
$serverIP = $_SERVER['SERVER_ADDR'];
$serverPort = $_SERVER['SERVER_PORT'];

$htmlContent = "<!DOCTYPE html>\n";
$htmlContent .= "<html lang=\"en\">\n";
$htmlContent .= "<head>\n";
$htmlContent .= "\t<meta charset=\"UTF-8\">\n";
$htmlContent .= "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
$htmlContent .= "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
$htmlContent .= "\t<title>Server information</title>\n";
$htmlContent .= "</head>\n";
$htmlContent .= "<body>\n";
$htmlContent .= "\t<h1>Server information:</h1>\n";
$htmlContent .= "\t<p>Web server: $serverSoftware</p>\n";
$htmlContent .= "\t<p>IP address: $serverIP</p>\n";
$htmlContent .= "\t<p>Port: $serverPort</p>\n";
$htmlContent .= "</body>\n";
$htmlContent .= "</html>";

echo $htmlContent;
?>