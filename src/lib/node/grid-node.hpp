// GPL v3 (c) 2020, Daniel Williams 

#ifndef _GRID_NODE_HPP_
#define _GRID_NODE_HPP_

#include <array>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

namespace chart::node {

template<size_t n, typename child_t, typename cell_t>
class GridNode {

  public:
    GridNode();
    GridNode(const cell_t value);

    ~GridNode();

    //void draw(std::ostream& sink, const std::string& prefix,
    //          const std::string& as, const bool show_pointers) const;

    void fill(const cell_t value);

    size_t get_height() const;

    uint32_t locate( double x, double y) const;
    child_t* get_child( const size_t i, const size_t j ) const;

    cell_t get_value() const;

    // bool load(const nlohmann::json& doc);

    bool operator==(const GridNode& other) const;

    ///! \brief coalesce leaf nodes that have the same value
    void prune();

    void split(const double precision, const double width);

    void reset();

    bool is_leaf() const;

    void set_value(cell_t new_value);

    std::string to_string() const;

  private:
    void split();

  public:
    constexpr static size_t size = n * n;
    constexpr static size_t width = n;
    constexpr static double scale = child_t::width * child_t::scale;

  private:
    // By design:
    //    If any of the pointers are empty <==> all of the pointers will be empty.
    std::array< std::unique_ptr<child_t>, n> children;

    cell_t value;

  // private:
  //   friend class GridNode_ConstructDefault_Test;
  //   friend class GridNode_ConstructWithValue_Test;
};

// #include "node/tile_node.hpp"

// typedef GridNode<16, TileNode<uint8_t, 16>, uint8_t> Grid16L1;
// typedef GridNode<32, TileNode<uint8_t, 32>, uint8_t> Grid32L1;

// typedef GridNode<16, Grid16L1, uint8_t> Grid16L2;
// typedef GridNode<32, Grid16L2, uint8_t> Grid32L2;

} // namespace chart::node

#endif // #ifndef _GRID_NODE_HPP_
