#include <stdexcept>
#include "temp_path.h"

//
// Return temporary path.
// The returned path can be safely used for creation of temporary file.
//
std::string GetTempPath()
{
    char dir[] = "/tmp/aaXXXXXXX";
    char *p = mkdtemp( dir );
    if( !p )
    {
        throw std::runtime_error( "Function 'mkdtemp failed" );
    }

    std::string path( dir );
    path += "/";
    path += "a.dat";

    return path;
}
