#include "unixfile.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <stdexcept>
#include <cstring>
#include <vector>

//
//
//
UnixFile::UnixFile( const std::string& path, Mode mode )
    : m_fd( m_badFd )
{
    if( mode == Mode::Open )
    {
        Open( path );
        return;
    }

    assert( mode == Mode::Create );
    Create( path );
}

//
//
//
UnixFile::~UnixFile()
{
    Close();
}

//
// Opens file
//
void UnixFile::Open( const std::string& path )
{
    assert( m_fd == m_badFd );

    const int oflag = O_RDWR;
    m_fd = open( path.c_str(), oflag );
    if( m_fd < 0 )
    {
        const std::string msg = std::string( "UnixFile::Open. Cannot open file: ") + path;
        throw std::runtime_error( msg );
    }
}

//
// Creates file
//
void UnixFile::Create( const std::string& path )
{
    assert( m_fd == m_badFd );

    const int oflag = O_CREAT | O_EXCL | O_RDWR;
    const int mask = 0600;    // r/w privileges to owner only
    m_fd = open( path.c_str(), oflag, mask );
    if( m_fd < 0 )
    {
        const std::string msg = std::string( "UnixFile::Create. Cannot create file ") + path;
        throw std::runtime_error( msg );
    }
}

//
// Closes file
//
void UnixFile::Close( )
{
    if( m_fd != m_badFd )
    {
        close( m_fd );
        m_fd = m_badFd;
    }
}

//
// Writes "data" of length "nbyte" to file.
// The data are offset "offset" from the begin.
//
void UnixFile::Write( const void* data, size_t nbyte, off_t offset ) const
{
    assert( m_fd != m_badFd );

    ssize_t rcw = pwrite( m_fd, data, nbyte, offset );
    if( rcw < 0 || rcw != static_cast< ssize_t >( nbyte ) )
    {
        throw std::runtime_error( "UnixFile::Write" );
    }
}

//
// Reads data from the file.
// If the data are read outside the file, it throws the exeption.
//
void UnixFile::Read( void *data, size_t nbyte, off_t offset ) const
{
    assert( m_fd != m_badFd );

    ssize_t rc = pread( m_fd, data, nbyte, offset );
    if( rc < 0 || rc != static_cast< ssize_t >( nbyte ) )
    {
        throw std::runtime_error( "UnixFile::Read" );
    }
}

//
//
//
void UnixFile::Fsync() const
{
    assert( m_fd != m_badFd );

    const int rc = fsync( m_fd );
    if( rc != 0 )
    {
        throw std::runtime_error( "UnixFile::Fsync" );
    }
}

