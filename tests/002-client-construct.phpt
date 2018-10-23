--TEST--
Should throw expected exception on invalid Beanspeak\Client constrictor params
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client([]); ?>
--EXPECTF--
Fatal error: Uncaught TypeError: Argument 1 passed to Beanspeak\Client::__construct() must be of the type string or null, array given in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct(Array)
#1 {main}
  thrown in %s on line %d
