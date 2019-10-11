
<?php

include 'file.php';

Foo\foo();
echo Foo\FOO . "\n";
Foo\foo::staticmethod();

Foo\Bar\foo();
echo Foo\Bar\FOO . "\n";
Foo\Bar\foo::staticmethod();

Foo\Bar\subnamespace\foo();
echo Foo\Bar\subnamespace\FOO . "\n";
Foo\Bar\subnamespace\foo::staticmethod();

?>