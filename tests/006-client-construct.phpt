--TEST--
Should throw expected exception on host absence
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client('?foo=bar'); ?>
--EXPECTF--
Fatal error: Uncaught Beanspeak\InvalidArgumentException: Invalid Client DSN scheme: missed host part. in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct('?foo=bar')
#1 {main}
  thrown in %s on line %d
