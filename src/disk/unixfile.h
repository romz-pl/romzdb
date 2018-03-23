#ifndef ROMZDB_DISK_UNIXFILE_H
#define ROMZDB_DISK_UNIXFILE_H

#include <string>

class UnixFile
{
public:
    enum class Mode{ Open, Create };
public:
    UnixFile( const std::string& path, Mode mode );
    ~UnixFile();

    void Write( const void* data, size_t nbyte, off_t offset ) const;
    void Read( void* data, size_t nbyte, off_t offset ) const;

    void Fsync() const;

private:
    void Open( const std::string& path );
    void Create( const std::string& path );
    void Close( );


private:
    // Bad, inproper file descriptor
    static const int m_badFd = -1;

    // File descriptor
    int m_fd;
};

#endif
