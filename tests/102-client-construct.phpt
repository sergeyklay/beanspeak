--TEST--
Should create a Beanspeak\Client instance with provided params
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php
$cl = new Beanspeak\Client('128.7.6.5');
$reflect = new ReflectionClass(Beanspeak\Client::class);
$prop = $reflect->getProperty('host');
$prop->setAccessible(true);
var_dump($prop->getValue($cl));

$cl = new Beanspeak\Client('192.169.0.1:299');
$reflect = new ReflectionClass(Beanspeak\Client::class);

$host = $reflect->getProperty('host');
$host->setAccessible(true);
var_dump($host->getValue($cl));

$port = $reflect->getProperty('port');
$port->setAccessible(true);
var_dump($port->getValue($cl));
?>
--EXPECT--
string(9) "128.7.6.5"
string(11) "192.169.0.1"
int(299)
