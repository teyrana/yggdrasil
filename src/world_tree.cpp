// GPL v3 (c) 2020
// namespace yggdrasil
// GPL v3 (c) 2020

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

using std::cerr;
using std::endl;
using std::string;

#include <Eigen/Geometry>
using Eigen::Vector2d;

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "geometry/layout.hpp"
#include "geometry/polygon.hpp"
// #include "quadtree/tree.hpp"
#include "world_tree.hpp"

using yggdrasil::geometry::Layout;
using yggdrasil::quadtree::WorldTree;
using yggdrasilgeometry::Polygon;
// using yggdrasil::quadtree::Node;

namespace yggdrasil {};

// main method for descending through a tree and returning the appropriate
// location / node / value note: weakly optimized; intended to be a hot path.
void descend(const Vector2d& target, double& x_c, double& y_c,
             const double start_width, Node*& current_node) {
    double current_width = start_width;
    double next_width = start_width * 0.5;

    while (!current_node->is_leaf()) {
        current_width = next_width;
        next_width *= 0.5;

        if (target[0] > x_c) {
            if (target[1] > y_c) {
                x_c += next_width;
                y_c += next_width;
                current_node = current_node->get_northeast();
            } else {
                x_c += next_width;
                y_c -= next_width;
                current_node = current_node->get_southeast();
            }
        } else {
            if (target[1] > y_c) {
                x_c -= next_width;
                y_c += next_width;
                current_node = current_node->get_northwest();
            } else {
                x_c -= next_width;
                y_c -= next_width;
                current_node = current_node->get_southwest();
            }
        }
    }
}

WorldTree::WorldTree() : WorldTree(Layout()) {}

WorldTree::WorldTree(const Layout& _layout) : layout(_layout) { reset(); }

WorldTree::~WorldTree() { root.release(); }

bool WorldTree::contains(const Eigen::Vector2d& p) const {
    return layout.contains(p);
}

cell_value_t WorldTree::classify(const Eigen::Vector2d& p) const {
    // create a R/W copy, initialized at the tree's center.
    Eigen::Vector2d located(layout.get_center());

    auto current_node = root.get();

    descend(p, located[0], located[1], layout.get_width(), current_node);

    return current_node->get_value();
}

void WorldTree::debug_tree(const bool show_pointers) const {
    cerr << "====== Quad WorldTree: ======\n";
    cerr << "##  bounds:     " << layout.to_string() << endl;
    cerr << "##  height:     " << get_height() << endl;
    cerr << "##  precision:  " << layout.get_precision() << endl;

    root->draw(cerr, "    ", "RT", show_pointers);
    cerr << endl;
}

size_t WorldTree::calculate_complete_tree(const size_t height) {
    // see: https://en.wikipedia.org/wiki/M-ary_tree
    //      # properties of M-ary trees
    //
    //    m == branching_factor == 4  ///< for a quadtree, this is trivially 4
    //
    //       (h+1)
    //     m        - 1
    // N = ---------------
    //        m - 1
    //
    return (pow(4, height + 1) - 1) / 3;
}

double WorldTree::get_load_factor() const {
    const size_t height = root->get_height();
    const size_t count = root->get_count();
    const size_t complete = calculate_complete_tree(height);
    return static_cast<double>(count) / static_cast<double>(complete);
}

size_t WorldTree::get_memory_usage() const { return size() * sizeof(Node); }

cell_value_t WorldTree::interp(const Eigen::Vector2d& at) const {

    // cout << "@@" << at << "    near: " << near.get_bounds() << " = " <<
    // near.get_value() << endl;

    // if Eigen::Vector2d is outside the tree, entirely
    if (!contains(at)) {
        return cell_default_value;
    }

    // const Node& near = root->search(at, get_bounds());
    //     const Eigen::Vector2d& cn = near.get_center();
    //     const double dx = std::copysign(1.0, (at.x() - cn.x())) * 2 *
    //     near.get_bounds().half_width; const double dy = std::copysign(1.0,
    //     (at.y() - cn.y())) * 2 * near.get_bounds().half_width; const Node& n2
    //     = root->search({cn.x() + dx, cn.y()     }, get_bounds()); const Node&
    //     n3 = root->search({cn.x() + dx, cn.y() + dy}, get_bounds()); const
    //     Node& n4 = root->search({cn.x()     , cn.y() + dy}, get_bounds());

    //     const auto& interp = near.interpolate_bilinear(at, n2, n3, n4);
    // return interp;

    return NAN;
}

void WorldTree::fill(const cell_value_t fill_value) { root->fill(fill_value); }

size_t WorldTree::get_height() const { return root->get_height() - 1; }

bool WorldTree::load_tree(const nlohmann::json& doc) {
    if (!doc.is_object()) {
        cerr << "?? attempted to load unexpected format: no-object json "
                "document!\n";
        return false;
    }
    return root->load(doc);
}

void WorldTree::prune() { root->prune(); }

void WorldTree::reset() { root = std::make_unique<Node>(0); }

void WorldTree::reset(const Layout& new_layout) {
    layout = new_layout;

    root = std::make_unique<Node>(0);
    root->split(layout.get_precision(), layout.get_width());
}

Sample WorldTree::sample(const Eigen::Vector2d& p) const {
    Vector2d located(layout.get_center());
    auto current_node = root.get();

    descend(p, located[0], located[1], layout.get_width(), current_node);

    return {located, current_node->get_value()};
}

bool WorldTree::store(const Vector2d& p, const cell_value_t new_value) {
    Vector2d located(layout.get_center());
    auto current_node = root.get();

    descend(p, located[0], located[1], layout.get_width(), current_node);

    current_node->set_value(new_value);
    return true;
}

size_t WorldTree::size() const { return root->get_count(); }

json WorldTree::to_json_tree() const { return root->to_json(); }
