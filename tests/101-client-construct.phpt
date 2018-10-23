--TEST--
Should create a Beanspeak\Client instance with default params
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php
$cl = new Beanspeak\Client();
$reflect = new ReflectionObject($cl);
foreach ($reflect->getProperties() as $prop) {
    echo implode(' ', Reflection::getModifierNames($prop->getModifiers())) . " \${$prop->getName()} = ";
    echo $prop->setAccessible(true);
    echo $prop->getValue($cl) == null ? 'NULL' : $prop->getValue($cl);
    echo ";\n";
}
?>
--EXPECT--
private $socket = NULL;
private $host = 127.0.0.1;
private $port = 11300;
private $timeout = 60;
private $persistent = 1;
protected $usedTube = default;
protected $watchedTubes = NULL;
