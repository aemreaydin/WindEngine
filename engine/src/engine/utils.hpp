#ifndef WINDENGINE_UTILS_HPP
#define WINDENGINE_UTILS_HPP

#include "defines.hpp"
#include "logger.hpp"
#include <filesystem>
#include <fstream>
#include <set>

namespace WindEngine
{

template <typename T> auto G_READ_SHADER_FROM_FILE( const std::string& fileName ) -> std::vector<T>
{
    const auto filePath = std::filesystem::current_path().append( fileName );
    std::fstream file { filePath, std::ios::binary | std::ios::in | std::ios::ate };
    if ( !file.is_open() )
    {
        throw std::runtime_error( "Failed to open file" );
    }
    const auto size = file.tellg();
    file.seekg( 0, std::ios::beg );

    std::vector<T> out( size );
    file.read( reinterpret_cast<char*>( out.data() ), size );
    return out;
}

template <typename T> auto G_IS_VECTOR_UNIQUE( const std::vector<T>& vec ) -> bool
{
    std::set<T> set( vec.begin(), vec.end() );
    return set.size() == vec.size();
}

}  // namespace WindEngine

#endif  // WINDENGINE_UTILS_HPP
