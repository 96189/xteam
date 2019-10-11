<?php

class Foo
{
    static function bar()
    {
        echo "bar\n";
    }
    function baz()
    {
        echo "baz\n";
    }
}

$func = array("Foo", "bar");
$func();

$func = array(new Foo, "baz"); # bar
$func();


?>