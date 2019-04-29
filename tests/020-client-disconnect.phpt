--TEST--
Test Beanspeak\Client::disconnect
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php
$client = new Beanspeak\Client();
var_dump($client->disconnect());
?>
--EXPECT--
bool(false)
