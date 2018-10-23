--TEST--
Check for Beanspeak\ExceptionInterface parent interface
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php
use Beanspeak\ExceptionInterface;
class MyImpl extends \Exception implements ExceptionInterface {}
$ex = new MyImpl();
var_dump($ex instanceof \Throwable);
?>
--EXPECT--
bool(true)
