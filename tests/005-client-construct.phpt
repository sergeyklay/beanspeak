--TEST--
Should throw expected exception for invalid scheme
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client('ftp://127.0.0.1'); ?>
--EXPECTF--
Fatal error: Uncaught Beanspeak\Exception\InvalidArgumentException: Invalid DSN scheme. Supported schemes are either 'tcp' or 'unix' (disabled), got 'ftp'. in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct('ftp://127.0.0.1')
#1 {main}
  thrown in %s on line %d
