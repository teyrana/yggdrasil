// GPL v3 (c) 2021, Daniel Williams 

// standard library includes
#include <iostream>
#include <string>
#include <vector>

#pragma once

namespace chartbox::io {

template< typename layer_t, typename writer_t >
class ChartBaseWriter {
public:
    bool write_to_path( const std::string& filename ){ 
        return writer().write(filename); }

    // bool write_to_stream( std::ostream& stream ){ 
    //     return writer().write_to_stream(stream); }

protected:
    ChartBaseWriter() = default;

    writer_t& writer() { 
        return *static_cast<writer_t*>(this); }

    const writer_t& writer() const {
        return *static_cast<const writer_t*>(this);}

    ~ChartBaseWriter() = default;

};

} // namespace chartbox::io
