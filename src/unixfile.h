#ifndef ROMZDB_UNIXFILE_H
#define ROMZDB_UNIXFILE_H

#include <string>

class UnixFile
{
public:
    enum class Mode{ Open, Create };
public:
    UnixFile( const std::string& path, Mode mode );
    ~UnixFile();



    void Write( const char* data, size_t nbyte, off_t offset ) const;
    void Read( char* data, size_t nbyte, off_t offset ) const;

    static std::string GetTempPath();


private:
    void Open( const std::string& path );
    void Create( const std::string& path );
    void Close( );
    void Lseek( off_t offset ) const;
    off_t GetSize() const;


private:
    // Bad, inproper file descriptor
    static const int m_badFd = -1;

    // File descriptor
    int m_fd;
};

#endif
