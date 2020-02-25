// GPL v3 (c) 2020, Daniel Williams 

#include <cmath> 

#include "index/index2u.hpp"

namespace chart::index {

Index2u::Index2u()
    : i(0), j(0)
{}

Index2u::Index2u( uint32_t _i, uint32_t _j)
    : i(_i), j(_j)
{}

Index2u::Index2u( const Index2u& origin ) 
    : i(origin.i), j(origin.j)
{}

Index2u Index2u::operator+( const Index2u& other ) const {
    return {i + other.i, j + other.j};
}

Index2u Index2u::operator-( const Index2u& other ) const {
    return {i - other.i, j - other.j};
}

double Index2u::norm1( const Index2u& other ) const {
    return std::abs(static_cast<float>(i - other.i))
           + std::abs(static_cast<float>(j - other.j));
}

double Index2u::norm2( const Index2u& other ) const {
    return std::sqrt( std::pow(static_cast<double>(i - other.i), 2.)
                    + std::pow(static_cast<double>(j - other.j), 2.) );
}


}; // namespace chart::index
