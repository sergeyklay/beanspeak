--TEST--
Get a properties list of the Beanspeak\Client
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php
$reflect = new ReflectionClass(Beanspeak\Client::class);
foreach ($reflect->getProperties() as $prop) {
    echo implode(' ', Reflection::getModifierNames($prop->getModifiers())) . " \${$prop->getName()};\n";
}
?>
--EXPECT--
private $socket;
private $host;
private $port;
private $timeout;
private $persistent;
private $usedTube;
private $watchedTubes;
