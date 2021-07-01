// GPL v3 (c) 2021, Daniel Williams


#include <random>
#include <chrono>

// may not be standard
#include <sys/stat.h>

#include "chart-box.hpp"
#include "io/chart-geojson-loader.hpp"
//#include "io/chart-png-writer.hpp"

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
    // bool enable_input_lidar = false;
    // std::string input_path_terrain;

    std::string boundary_input_path("data/block-island/boundary.polygon.geojson");
    // std::string boundary_input_path("data/block-island/boundary.simple.geojson");

    std::string boundary_output_path;//("debug-height-map.png");

    // bool enable_output_height_map = false;
    // std::string output_path_height_map;

    // configure input:
    
    // std::string output_path("?????");
    // if( enable_output_height_map ){
    //     if( output_path_height_map.empty() ){
    //         output_path_height_map = "worldtree.output.png";
    //     }
    //     cerr << "  ## Save output to: " << output_path << '\n';
    // }

    // ^^^^ Configuration
    // vvvv Execution
    GDALAllRegister();

    chartbox::ChartBox box;

    const auto start_load = std::chrono::high_resolution_clock::now(); 
    {
        if ( ! boundary_input_path.empty() ) {
            std::cout << "    >>> Loading boundary layer from path: " << boundary_input_path << std::endl;

            using chartbox::io::GeoJSONLoader;
            using chartbox::layer::FixedGridLayer;
            auto boundary_loader = GeoJSONLoader<FixedGridLayer>( box.mapping(), box.get_boundary_layer());
            if( ! boundary_loader.load_file(boundary_input_path) ){
                std::cerr << "!!!! error while loading data:!!!!\n";
                return -1;
            }else{
                std::cout << "    <<< Successfuly loaded BoundaryLayer." << std::endl;
            }
        }

        // const bool load_success = chart.get_countour_layer()->load_from_json_stream( *boundary_document_stream);
        // if(!load_success){
        //     cerr << "!!!! error while loading data:!!!!\n";
        //     return -1;
        // }        
    }

    const auto finish_load = std::chrono::high_resolution_clock::now(); 
    const auto load_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(finish_load - start_load).count())/1000;
    std::cout << "<< Loaded in:   " << load_duration << " s \n\n";

    {   // DEBUG
        //print the resultant bounds:
        box.mapping().print();

        // // print a summary of layers in the chartbox...
        // box.print_layers() ;

        const auto boundary_layer = box.get_boundary_layer();
        box.get_boundary_layer().print_contents();

        // // this layer is not yet relevant... or interesting.
        // const auto contour_layer = box.get_contour_layer();
        // box.get_contour_layer().print_contents();

    }   // DEBUG

    // const auto start_write = std::chrono::high_resolution_clock::now();  
    // {
    //     // Optionally load boundary path:
    //     if( ! boundary_output_path.empty() ){
    //         std::cerr << "  ## Write output to: " << boundary_output_path << '\n';
    //         chartbox::io::PNGWriter<chartbox::layer::FixedGridLayer> boundary_writer(stack.get_boundary_layer());
    //             if( ! boundary_writer.write_to_path(boundary_output_path) ){
    //                 std::cerr << "!!!! error while writing data:!!!!\n";
    //                 return -1;
    //             }else{
    //                 std::cout << "    <<< Successfuly wrote BoundaryLayer." << std::endl;
    //             }
    //     }

    // }
    // const auto finish_write = std::chrono::high_resolution_clock::now(); 
    // const auto write_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(finish_write - start_write).count())/1000;
    // std::cerr << "<< Written in:   " << write_duration << " s \n";
    
    // if( enable_output_height_map ){
    //     cerr << "##>> writing output...\n";
    //     to_png(chart.get?layer(), output_path);
    // }


    // make sure this only happens once
    GDALDestroyDriverManager();

    return 0;
}
