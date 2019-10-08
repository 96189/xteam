
#include "leaf.h"
#include "composite.h"

int main(int argc, char* argv[])
{
    BinaryFile xx("xx.mp3"); 
    BinaryFile yy("yy.mp3"); 
    BinaryFile zz("zz.mp3"); 
    Folder music("music");
    music.addFile(&xx);
    music.addFile(&yy);
    music.addFile(&zz);
    music.showFileName();

    AbstractFile *readme = new AsciiFile("readme");
    Folder *cpp = new Folder("cpp");
    Folder *php = new Folder("php");
    Folder *code = new Folder("code");
    code->addFile(php);
    code->addFile(cpp);
    code->addFile(readme);
    code->showFileName();
    return 0;
}