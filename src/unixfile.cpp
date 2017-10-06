#include "unixfile.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <stdexcept>

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

    if( mode == Mode::Create )
    {
        Create( path );
        return;
    }

    assert( 0 );
}

UnixFile::~UnixFile()
{
    Close();
}

//
// Opens file
//
void UnixFile::Open( const std::string& path )
{
    if( m_fd != m_badFd )
        return;

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
    if( m_fd != m_badFd )
        return;

    const int oflag = O_CREAT | O_EXCL | O_RDWR;
    const int mask = 0600;    // r/w privileges to owner only
    m_fd = open( path.c_str(), oflag, mask );
    if( m_fd < 0 )
    {
        const std::string msg = std::string( "UnixFile::Create. Cannot create file ") + path;
        throw std::runtime_error( msg );
    }
}

void UnixFile::Close( )
{
    if( m_fd != m_badFd )
    {
        close( m_fd );
        m_fd = m_badFd;
    }
}


void UnixFile::Write( const char* data, size_t nbyte, off_t offset ) const
{
    assert( m_fd != m_badFd );

    Lseek( offset );

    ssize_t rcw = write( m_fd, data, nbyte );
    if( rcw < 0 || rcw != static_cast< ssize_t >( nbyte ) )
    {
        throw std::runtime_error( "UnixFile::Write: write" );
    }
}

void UnixFile::Read( char* data, size_t nbyte, off_t offset ) const
{
    assert( m_fd != m_badFd );

    Lseek( offset );

    ssize_t rc = read( m_fd, data, nbyte );
    if( rc < 0 || rc != static_cast< ssize_t >( nbyte ) )
    {
        throw std::runtime_error( "UnixFile::Read" );
    }
}

void UnixFile::Lseek( off_t offset ) const
{
    assert( m_fd != m_badFd );

    off_t rc = lseek( m_fd, offset, SEEK_SET );
    if( rc < 0 )
    {
        throw std::runtime_error( "UnixFile::Lseek." );
    }
}
