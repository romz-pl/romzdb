#include <cstdlib>
#include <cassert>
#include "temp_path.h"

//
// Return temporary path.
// The returned path can be safely used for creation of temporary file.
//
std::string GetTempPath()
{
    char dir[] = "/tmp/romzdb_XXXXXXX";
    char *p = mkdtemp( dir );
    assert( p );
    std::string path( dir );
    path += "/a.dat";
    return path;
}

