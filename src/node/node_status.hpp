// GPL v3 (c) 2020

#ifndef _NODE_STATUS_HPP_
#define _NODE_STATUS_HPP_


namespace chart::node {

enum NodeStatus : uint8_t {
    Cached=0,         // a copy exists on disk, but is not loaded into memory
    Busy=1,           // Something is modifying this node. Don't touch. (does this need to be a Mutex?)
    Uniform=2,        // This node contains only one value and is ripe for culling
    Mixed=3,          // This node has a mix of values in its children.  Most common case.
    ErrorUnknown=10,  // something (unknown) is wrong 
};

}; // namespace chart::node

#endif // #ifdef _NODE_STATUS_HPP_
