// GPL v3 (c) 2021, Daniel Williams 

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

#include <Eigen/Dense>
#include <Eigen/Geometry>

// #ifdef ENABLE_GDAL
// #include "gdal.h"
// #include "gdal_priv.h"
// #endif

// #include <nlohmann/json.hpp>
// using nlohmann::json;

#include "chart-box.hpp"

using chartbox::ChartBox;

ChartBox::ChartBox()
    : bounds_(Eigen::Vector2d(0,0), Eigen::Vector2d(1,1))
    , _grid_layer(bounds_)
{
#ifdef ENABLE_GDAL
    GDALAllRegister();
#endif
}

int ChartBox::classify( const Eigen::Vector2d& /*p*/) const { 
    return 0;
}

// size_t ChartBox::get_layer_count() {
//     return layers.size();
// }

// void* ChartBox::get_layer_at_index( const size_t index) {
//     return this->operator[](index);
// }

int ChartBox::locate( const Eigen::Vector2d& center, const double width ) {
    Eigen::Vector2d half_diag( width/2, width/2 );
    bounds_.min() = center - half_diag;
    bounds_.max() = center + half_diag;
    return 0;
}

// void* ChartBox::operator[]( const size_t index) {
//     return index<layers.size() ? layers[index] : nullptr;
// }

std::string ChartBox::summary() const {
    std::ostringstream buf;

    buf << "======== ======== Printing Layers: ======== ======== \n";

    int layer_index = 0;

    // Bottom Layer
    buf << "    [" << std::setw(2) << layer_index << ']' 
                   << '<' << _grid_layer.type() << '>'
                   << ':' << _grid_layer.name() << '\n';
    ++layer_index;

    // next layer 
    // ...


    buf << "======== ======== " << layer_index << " layers total ======== ========\n";

    return buf.str();
}
    
ChartBox::~ChartBox() { 

#ifdef ENABLE_GDAL
    GDALDestroyDriverManager();
#endif


}
