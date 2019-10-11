<?php

$a = 1;

echo "not in func $a\n";

function Test() {
    echo "$a\n";    # Undefined variable: a
}

# Test();   # 调用失败

function Test_a() {
    global $a;
    echo "$a\n";
}
Test_a();

function test_static() {
    static $a = 0;
    echo "$a | ";
    $a++;
}

for ($i = 0; $i < 5; $i++) {
    test_static();
}

?>