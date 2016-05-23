--TEST--
Check for xxtea presence
--SKIPIF--
<?php if (!extension_loaded("xxtea")) print "skip"; ?>
--FILE--
<?php 
//echo "xxtea extension is available";
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TESTING for further information on
  writing regression tests
*/
$content = 'Meitu Vs IDreamsky, Meitu WIN, Meitu WIN!';
$key = "caomima";
$a = xxtea_encode($content, $key);
$b = xxtea_decode($a, $key);

if ($b == $content) {
	echo "success!";
} else {
	echo "fail!";
}
?>
--EXPECT--
success!
