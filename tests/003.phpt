--TEST--
Should throw exception on invalid Beanspeak\Client constrictor params
--SKIPIF--
<?php include('skipif.inc'); ?>
--FILE--
<?php new Beanspeak\Client("foo"); ?>
--EXPECTF--
Fatal error: Uncaught TypeError: Argument 1 passed to Beanspeak\Client::__construct() must be of the type array or null, string given in %s:%d
Stack trace:
#0 %s(%d): Beanspeak\Client->__construct('foo')
#1 {main}
  thrown in %s on line %d
