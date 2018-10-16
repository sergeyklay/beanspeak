--TEST--
Check for beanspeak presence
--SKIPIF--
<?php if (!extension_loaded("beanspeak")) print "skip"; ?>
--FILE--
<?php
echo "beanspeak extension is available";
?>
--EXPECT--
beanspeak extension is available
