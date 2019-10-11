<?php

class SimpleClass 
{
    public $var = 'a default value';

    public function displayVar() {
        echo $this->var;
    }
}

$instance = new SimpleClass();

$assigned = $instance;
$reference = &$instance;

$instance->var = '$assigned will have this value';
$instance = NULL;   # 通过reference和instance访问都为NULL 但实际对象依然存在 目前只能通过assigned访问到

var_dump($instance);
var_dump($reference);
var_dump($assigned);


class Foo 
{
    public $bar;

    public function __construct() {
        echo "Foo construct\n";
        $this->bar = function() {
            return 2019;
        };
    }

    public function __destruct() {
        echo "Foo destruct\n";
    }
}

$obj = new Foo();
$func = $obj->bar;
echo $func(), PHP_EOL;

class Test
{
    private $foo;

    public function __construct($foo) {
        $this->foo = $foo;
    }

    private function bar() {
        echo "Accessed this private method\n";
    }

    public function baz(Test $other) {
        $other->foo = 'hello';
        var_dump($other->foo);

        $other->bar();  # 访问私有
    }
}

$test = new Test('test');
$test->baz(new Test('ohter'));


# 接口
# 声明接口
interface iTemplate
{
    public function setVariable($name, $var);
    public function getHtml($template);
}

# 实现接口
class Template implements iTemplate 
{
    private $vars = array();

    public function setVariable($name, $var) {
        $this->vars[$name] = $var;
    }

    public function getHtml($template) {
        foreach ($this->vars as $name => $value) {
            $template = str_replace('{' . $name . '}', $value, $template);
        }
        return $template;
    }
}

?>