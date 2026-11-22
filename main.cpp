using namespace std;
#include "file_manager.h"

int main()
{
    FileManager fm;
    fm.newFolder("home");
    fm.openFolder("home");
    fm.remove("/home");
}
