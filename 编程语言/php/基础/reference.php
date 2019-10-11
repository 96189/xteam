<?php

$bar = 'hello';
$baz = 'world';

# 引用不是指针
function foo(&$var) {
    $var = & $GLOBALS["baz"];
    echo '$var' . ':' . $var . "\n";
}

echo $bar . "\n";
foo($bar);
echo $bar . "\n";

# 引用传递
function change(&$var) {
    $var = $GLOBALS["baz"];
}

echo $bar . "\n";
change($bar);
echo $bar . "\n";

# 返回引用
class foo {
    public $value = 42;

    public function &getValue() {
        return $this->value;
    }
}

$obj = new foo;
$myValue = &$obj->getValue();
echo $obj->value . "\n";
$obj->value = 2;
echo $obj->value . "\n";

# 取消引用
$a = 1;
$b = &$a;
echo 'a:' . $a . "\n";
echo 'b:' . $b . "\n";
unset($a);
echo 'a:' . $a . "\n";
echo 'b:' . $b . "\n";
?>