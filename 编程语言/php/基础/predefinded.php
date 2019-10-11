<?php

echo $_SERVER['PHP_SELF'] . "\n";
$vars = $_SERVER['argv'];
foreach ($vars as $val) {
    echo $val . "\n";
}
echo $_SERVER['argc'] . "\n";
echo $_SERVER['GATEWAY_INTERFACE'] . "\n";
echo $_SERVER['SERVER_ADDR'] . "\n";
echo $_SERVER['SERVER_NAME'] . "\n";
echo $_SERVER['SERVER_SOFTWARE'] . "\n";
echo $_SERVER['SERVER_PROTOCOL'] . "\n";
echo $_SERVER['REQUEST_METHOD'] . "\n";
?>