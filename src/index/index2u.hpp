// GPL v3 (c) 2020, Daniel Williams 

#ifndef _INDEX_TYPEDEF_HPP_
#define _INDEX_TYPEDEF_HPP_

#include <cstdint>

namespace chart::index {

class Index2u {
public:
    Index2u();
    Index2u( uint32_t _i, uint32_t _j);
    Index2u( const Index2u& origin ); // copy constructor
    Index2u operator+( const Index2u& other ) const;
    Index2u operator-( const Index2u& other ) const;
    double norm1( const Index2u& other ) const;
    double norm2( const Index2u& other ) const;


public:
    const uint32_t i;
    const uint32_t j;

};

}; // namespace chart::index

#endif // #ifndef _INDEX_TYPEDEF_HPP_