<?php
$message = 'hello';
echo $message . "\n";

$example = function () {
    var_dump($message); # 闭包使用变量需要从父作用域继承
};
$example();

# 从父作用域继承变量(值) 
$example = function() use ($message) {
    var_dump($message);
};
$example();

$message = 'world';
$example();

$message = 'hello';

# 从父作用域继承变量(引用)
$example = function() use (&$message) {
    var_dump($message);
};
$example();

$message = 'world';
$example();

$example = function ($arg) use ($message) {
    var_dump($arg . ' ' . $message);
};
$example("hello");

?>