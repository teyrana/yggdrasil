// GPL v3 (c) 2020
#ifndef _QUAD_NODE_HPP_
#define _QUAD_NODE_HPP_

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "geometry/cell_value.hpp"

#include "node/tile_node.hpp"

using yggdrasil::geometry::cell_value_t;

namespace yggdrasil::node {

class TileNode;
class QuadNode;

class WorldTree {
  public:
    WorldTree();
    // QuadNode(const cell_value_t value);

    ~WorldTree();

    void draw(std::ostream& sink, const std::string& prefix,
              const std::string& as, const bool show_pointers) const;

    void fill(const cell_value_t fill_value);

    size_t get_count() const;
    size_t get_height() const;

    cell_value_t& get_value();
    cell_value_t get_value() const;

    bool load(const nlohmann::json& doc);

    bool operator==(const QuadNode& other) const;

    ///! \brief coalesce leaf nodes that have the same value
    void prune();

    void split(const double precision, const double width);

    void reset();

    bool is_leaf() const;

    void set_value(cell_value_t new_value);

    nlohmann::json to_json() const;

    std::string to_string() const;

  private:
    void split();

  private:
    std::unique_ptr<TileNode> head;

    //   private:
    // friend class QuadNodeTest_ConstructDefault_Test;
    // friend class QuadNodeTest_ConstructWithValue_Test;
    // friend class QuadNodeTest_SetGet_Test;
    // friend class QuadNodeTest_SplitNodeImperative_Test;
};

} // namespace yggdrasil::node

#endif // #ifndef _QUAD_NODE_HPP_
