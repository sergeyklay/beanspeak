--TEST--
Should throw expected exception on corrupted DSN
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client('tcp://'); ?>
--EXPECTF--
Fatal error: Uncaught Beanspeak\InvalidArgumentException: The beanstalkd connection DSN is invalid: 'tcp://'. in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct('tcp://')
#1 {main}
  thrown in %s on line %d
