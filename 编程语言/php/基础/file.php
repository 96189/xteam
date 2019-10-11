<?php
namespace Foo 
{
    const FOO = 3;
    function foo() {}
    class foo 
    {
        static function staticmethod() {}
    }   
}

namespace Foo\Bar {
    const FOO = 2;
    function foo() {}
    class foo 
    {
        static function staticmethod() {}
    }
}

namespace Foo\Bar\subnamespace {
    const FOO = 1;
    function foo() {}
    class foo 
    {
        static function staticmethod() {}
    }  
}

?>