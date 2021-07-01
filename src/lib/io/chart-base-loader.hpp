// GPL v3 (c) 2021, Daniel Williams 
#pragma once

// standard library includes
#include <iostream>


namespace chartbox::io {

template< typename layer_t, typename loader_t >
class ChartBaseLoader {
public:
    bool load_file(const std::string& filename){ return loader().load_file(filename); }
    bool load_text(const std::string& source){ return loader().load_text(source); }
    // bool load_binary(const std::string& source){ return loader().load_binary(source); }

protected:

    ChartBaseLoader() = default;

    loader_t& loader() { 
        return *static_cast<loader_t*>(this); }

    const loader_t& loader() const {
        return *static_cast<const loader_t*>(this);}

    ~ChartBaseLoader() = default;

};

} // namespace chartbox::io
