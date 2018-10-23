--TEST--
Should throw expected exception for disabled protocol
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client('unix://var'); ?>
--EXPECTF--
Fatal error: Uncaught Beanspeak\Exception\InvalidArgumentException: Protocol 'unix' currently disabled in Beanspeak Client. in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct('unix://var')
#1 {main}
  thrown in %s on line %d
