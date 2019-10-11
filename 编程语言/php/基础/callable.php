<?php 

function my_callback_function() {
    echo "global hello world\n";
}

class MyClass {
    static function myCallbackMethod() {
        echo "MyClass Hello World\n";
    }
    function myObjMethod() {
        echo "MyClass obj method\n";
    }
}

call_user_func('my_callback_function');

call_user_func(array('MyClass', 'myCallbackMethod'));
call_user_func(array('MyClass', 'myObjMethod'));

$obj = new MyClass();
call_user_func(array($obj, 'myCallbackMethod'));
call_user_func(array($obj, 'myObjMethod'));

call_user_func('MyClass::myCallbackMethod');
call_user_func('MyClass::myObjMethod');

class A {
    public static function who() {
        echo "A\n";
    }
}

class B extends A {
    public static function who() {
        echo "B\n";
    }
}

call_user_func(array('B', 'who'));
call_user_func(array('B', 'parent::who'));


class C {
    public function __invoke($name) {
        echo 'Hello ' . $name . "\n";
    }
}

$c = new C();
call_user_func($c, 'FUNC-PHP!');

?>