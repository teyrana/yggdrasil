// GPL v3 (c) 2020, Daniel Williams 

#include <iostream>
#include <sstream>

using std::byte;
using std::cerr;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

#include <Eigen/Geometry>
#include <flatbuffers/flatbuffers.h>
#include <nlohmann/json.hpp>

using Eigen::Vector2d;

#include "geometry/bounds.hpp"
using chart::geometry::Bounds;

#include "io/readers.hpp"

#include "tile-node.hpp"

// auto-generated code for flatbuffers serialization.
#include "tile-cache-generated.h"

namespace chart::node {

template <typename cell_t, size_t dimension>
const std::string TileNode<cell_t, dimension>::anchor_key = "anchor";
template <typename cell_t, size_t dimension>
const std::string TileNode<cell_t, dimension>::grid_key = "grid";
template <typename cell_t, size_t dimension>
const std::string TileNode<cell_t, dimension>::x_key = "x";
template <typename cell_t, size_t dimension>
const std::string TileNode<cell_t, dimension>::y_key = "y";

template <typename cell_t, size_t dimension>
TileNode<cell_t, dimension>::TileNode()
    : anchor(Vector2d(0., 0.)), index(data), status_(Uniform)
{
    fill(0);
}

template <typename cell_t, size_t dimension>
TileNode<cell_t, dimension>::TileNode(const Vector2d& _anchor)
    : anchor(_anchor), index(data), status_(Uniform)
{
    fill(0);
}

template <typename cell_t, size_t dimension>
std::unique_ptr<TileNode<cell_t, dimension>>
TileNode<cell_t, dimension>::build_from_json(const std::string& text) {
    const nlohmann::json doc =
        nlohmann::json::parse(text,    // source document
                              nullptr, // callback argument
                              false);  // allow exceptions?
    if (doc.is_discarded()) {
        cerr << "malformed json! ignore.\n" << endl;
        // cerr << source.rdbuf() << endl;
        return {};
    } else if (!doc.is_object()) {
        cerr << "input should be a json _document_!!\n" + doc.dump(4) << endl;
        return {};
    } else if (!doc.contains(anchor_key)) {
        cerr << "input document is missing an anchor object! \n";
        return {};
    }
    const auto anchor_object = doc[anchor_key];

    if (!anchor_object.contains(TileNode<cell_t, dimension>::x_key) ||
        !anchor_object[TileNode<cell_t, dimension>::x_key].is_number()) {
        cerr << "input document is missing the anchor.x field! \n";
        return {};
    } else if (!anchor_object.contains(TileNode<cell_t, dimension>::y_key) ||
               !anchor_object[TileNode<cell_t, dimension>::y_key].is_number()) {
        cerr << "input document is missing the anchor.y field! \n";
        return {};
    }

    const double x = anchor_object[x_key].get<double>();
    const double y = anchor_object[y_key].get<double>();
    auto result = std::make_unique<TileNode>(Vector2d(x, y));

    if (doc.contains(grid_key)) {
        nlohmann::json grid = doc[grid_key];

        if (!grid.is_array() || !grid[0].is_array()) {
            cerr << "chart::io::load_grid expected a array-of-arrays! "
                    "aborting!\n";
            result->fill(cell_default_value);
            return result;
        }

        if (grid.size() != dimension || grid[0].size() != dimension) {
            cerr << "chart::io::load_grid expected a array of the same "
                    "dimension "
                    "as configured!!\n";
            cerr << "    expected: " << dimension << endl;
            cerr << "    found:    " << grid.size() << " x " << grid[0].size()
                 << endl;
            result->fill(cell_default_value);
            return result;
        }

        // populate the tree
        size_t row_index = dimension - 1;
        for (auto& row : grid) {
            size_t column_index = 0;
            // i.e. a cell is the element at [column_index, row_index] <=> [x,y]
            for (auto& cell : row) {
                result->index(column_index, row_index) = cell.get<cell_t>();
                ++column_index;
            }
            --row_index;
        }
    }

    result->status_ = NodeStatus::Mixed;

    return result;
}

template <typename cell_t, size_t dimension>
cell_t TileNode<cell_t, dimension>::classify(const Vector2d& p) const {
    if (contains(p)) {
        const Vector2d scaled = (p - anchor) * scale;
        const size_t xi = static_cast<size_t>(scaled.x());
        const size_t yi = static_cast<size_t>(scaled.y());
        return index(xi, yi);
    }

    return cell_default_value;
}

template <typename cell_t, size_t dimension>
bool TileNode<cell_t, dimension>::contains(const Vector2d& p) const {
    const double x = p.x() - anchor.x();
    if ((x < 0) || (width <= x)) {
        return false;
    }

    const double y = p.y() - anchor.y();
    if ((y < 0) || (width <= y)) {
        return false;
    }

    return true;
}

template <typename cell_t, size_t dimension>
void TileNode<cell_t, dimension>::fill(const cell_t value) {
    memset(data.data(), value, sizeof(cell_t) * size);
    status_ = NodeStatus::Uniform;
}

template <typename cell_t, size_t dimension>
void TileNode<cell_t, dimension>::fill(const std::vector<cell_t>& source) {
    if (source.size() < data.size()) {
        return;
    }

    size_t read_index = 0;
    for (size_t j = dimension - 1; j < dimension; --j) {
        for (size_t i = 0; i < dimension; ++i) {
            index(i, j) = source[read_index];
            ++read_index;
        }
    }

    status_ = NodeStatus::Mixed;
}

template <typename cell_t, size_t dimension>
void TileNode<cell_t, dimension>::fill(const Polygon& poly,
                                       const cell_t fill_value) {
    chart::io::fill_from_polygon(*this, poly, fill_value);
    status_ = NodeStatus::Mixed;
}

template <typename cell_t, size_t dimension>
const Bounds TileNode<cell_t, dimension>::get_bounds() const {
    return {anchor, anchor + Vector2d(width, width)};
}

template <typename cell_t, size_t dimension>
cell_t& TileNode<cell_t, dimension>::get_cell(const size_t xi,
                                              const size_t yi) {
    return index(xi, yi);
}

template <typename cell_t, size_t dimension>
cell_t TileNode<cell_t, dimension>::get_cell(const size_t xi,
                                             const size_t yi) const {
    return index(xi, yi);
}

template <typename cell_t, size_t dimension>
cell_t TileNode<cell_t, dimension>::get_value() const {
    return data[0];
}

template <typename cell_t, size_t dimension>
bool TileNode<cell_t, dimension>::load_from_flatbuffer(const std::byte* const buffer) {
    auto tile_cache = GetTileCache(buffer);

    auto gridv = tile_cache->grid();
    if (gridv->size() != size) {
        return false;
    }

    const uint8_t* read_buffer = gridv->Data();
    uint8_t* write_buffer = data.data();
    memcpy(write_buffer, read_buffer, sizeof(cell_t) * size);

    status_ = NodeStatus::Mixed;

    return true;
}

template <typename cell_t, size_t dimension>
bool TileNode<cell_t, dimension>::load_from_shapefile(const std::string& filepath) {
    status_ = NodeStatus::Mixed;
    return chart::io::load_from_shape_file<TileNode, cell_t>(*this,
                                                                 filepath);
}

template <typename cell_t, size_t dimension>
NodeStatus TileNode<cell_t, dimension>::status() const {
    return status_;
}

template <typename cell_t, size_t dimension>
bool TileNode<cell_t, dimension>::store(const Vector2d& p,
                                        const cell_t new_value) {
    if (contains(p)) {
        const Vector2d scaled = (p - anchor) * scale;
        const size_t xi = static_cast<size_t>(scaled.x());
        const size_t yi = static_cast<size_t>(scaled.y());
        index(xi, yi) = new_value;
        return true;
    }

    return false;
}

template <typename cell_t, size_t dimension>
std::string TileNode<cell_t, dimension>::to_json() const {

    // explicitly create the json object
    nlohmann::json doc = nlohmann::json::object();

    doc[anchor_key] = {{x_key, anchor.x()}, {y_key, anchor.y()}};

    nlohmann::json grid = nlohmann::json::array();
    for (size_t j = dimension - 1; j < dimension; --j) {
        grid[j] = nlohmann::json::array();
        for (size_t i = 0; i < dimension; ++i) {
            grid[j][i] = static_cast<int>(index(i, j));
        }
    }
    doc[grid_key] = grid;

    return doc.dump();
}

template <typename cell_t, size_t dimension>
std::string TileNode<cell_t, dimension>::to_string() const {
    std::ostringstream buf;
    static const std::string header(
        "======== ======= ======= ======= ======= ======= ======= =======\n");
    buf << header;
    for (size_t j = dimension - 1; j < dimension; --j) {
        for (size_t i = 0; i < dimension; ++i) {
            const auto value = index(i, j);
            buf << ' ';
            if (value < 32) {
                buf << ' ';
            } else {
                buf << static_cast<char>(value);
            }
        }
        buf << endl;
    }
    buf << header;

    return buf.str();
}

template <typename cell_t, size_t dimension>
vector<byte> TileNode<cell_t, dimension>::to_raster() const {
    // allocate data buffers
    vector<byte> buffer(size);

    // bottom-row first:
    // std::memcpy( buffer.data(), data.data(), size);

    // top-row first:
    size_t write_index = 0;
    // // Loop through the rows of the image.
    for (size_t j = dimension - 1; j < dimension; --j) {
        for (size_t i = 0; i < dimension; ++i) {
            buffer[write_index] = (std::byte)(index(i, j));
            ++write_index;
        }
    }

    return buffer;
}

template <typename cell_t, size_t dimension>
const std::byte* const TileNode<cell_t, dimension>::to_flatbuffer() {
    // for 1k tiles (32x32) => 1072 bytes
    // for 1M tiles (1024x1024) => 1048624 bytes
    flatbuffers::FlatBufferBuilder builder(1100);

    uint8_t* write_buffer;
    auto grid = builder.CreateUninitializedVector(size, 1, &write_buffer);

    const uint8_t* read_buffer = data.data();
    memcpy(write_buffer, read_buffer, sizeof(cell_t) * size);

    auto tile = CreateTileCache(builder, anchor.x(), anchor.y(), grid);

    builder.Finish(tile);
    //cerr << "::on_finish::buffer.size= " << builder.GetSize() << endl;

    uint8_t* buffer = builder.GetBufferPointer();
    // builder.Clear(); // useful if/when the builder is re-used.

    // because std::byte and uint8T _REALLY_ should be the same size.
    static_assert(sizeof(uint8_t) == sizeof(std::byte));
    static_assert(sizeof(cell_t) == sizeof(std::byte));

    return reinterpret_cast<std::byte*>(buffer);
}

// Explicitly instantiate some type
// ===========

// used for testing
template class TileNode<uint8_t, 4>;

// this should match the Tile1k type:
template class TileNode<uint8_t, 32>;

// // this should match the Tile1M type:
template class TileNode<uint8_t, 1024>;

}; // namespace chart::node
