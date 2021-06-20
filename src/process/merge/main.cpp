// GPL v3 (c) 2020

#include <fstream>
#include <iostream>
#include <random>
#include <chrono>

#include <sys/stat.h> // may not be standard

#include <nlohmann/json.hpp>
using nlohmann::json;

// #include "node/quad_node.hpp"
// #include "node/tile_node.hpp"

#include "chart-box.hpp"
#include "io/chart-json-loader.hpp"
// #include "io/writers.hpp"

// using namespace chartbox::io;

// using chartbox::grid::Grid;
// using chartbox::quadtree::Tree;
// using chartbox::WorldTree;

constexpr double boundary_width = 4096.;
constexpr double desired_precision = 1.0;

constexpr size_t test_seed = 55;
static std::mt19937 generator;

// int main(int argc, char* argv[]){
int main( void ){
    bool enable_input_lidar = false;
    std::string input_path_terrain;
    bool enable_input_boundary = false;
    std::unique_ptr<std::istream> boundary_document_stream;


    bool enable_output_height_map = false;
    std::string output_path_height_map;

    // configure input:
    if ( ! enable_input_boundary ) {
        std::cerr << "  ## using default input.\n";
        // load default json source data
        // const json source = chart::generate::diamond(  boundary_width, desired_precision);
        const std::string source = "placeholder";
        boundary_document_stream.reset(new std::istringstream(source));
    } else {
        std::string input_path_boundary;
        std::cerr << "  ## File input: ??" << input_path_boundary << std::endl;

        struct stat buf;
        if( !input_path_boundary.empty() && (-1 == stat(input_path_boundary.c_str(), &buf)) ){
            std::cerr << "!? could not find file!\n";
            return -1;
        }
    }

    chartbox::ChartBox stack;
    
    // std::string output_path;
    // if( enable_output_height_map ){
    //     if( output_path_height_map.empty() ){
    //         output_path_height_map = "worldtree.output.png";
    //     }
    //     cerr << "  ## Save output to: " << output_path << '\n';
    // }

    // ^^^^ Configuration
    // vvvv Execution

    std::cerr << ">> loading input file... \n";
    const auto start_load = std::chrono::high_resolution_clock::now(); 
    
    // const bool load_success = chart.get_countour_layer()->load_from_json_stream( *boundary_document_stream);
    // if(!load_success){
    //     cerr << "!!!! error while loading data:!!!!\n";
    //     return -1;
    // }
    const auto finish_load = std::chrono::high_resolution_clock::now(); 
    const auto load_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(finish_load - start_load).count())/1000;
    std::cerr << "<< Loaded in:   " << load_duration << " s \n";

    
    std::cerr << stack.summary() << std::endl;

    // if( enable_output_height_map ){
    //     cerr << "##>> writing output...\n";
    //     to_png(chart.get?layer(), output_path);
    // }

    return 0;
}
