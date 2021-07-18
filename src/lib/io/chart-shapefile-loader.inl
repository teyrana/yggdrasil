// GPL v3 (c) 2021, Daniel Williams 
//
// NOTE: This is not an independent compilation unit! 
//       It is a template-class implementation, and should only be included from its header.

// #include <cmath>
// #include <cstddef>
// #include <cstdio>
// #include <fstream>
// #include <iostream>
// #include <memory>
// #include <string>
// #include <unistd.h>

#include <fmt/core.h>

#include <gdal.h>
#include <ogr_geometry.h>

using chartbox::io::ShapefileLoader;


template <typename layer_t>
bool ShapefileLoader<layer_t>::load_file( const string& filepath) {
    
    // might be a duplicate call, but duplicate calls don't seem to cause any
    // problems.
    GDALAllRegister();

    auto* source_dataset = (GDALDataset*)GDALOpenEx( filepath.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (source_dataset == NULL) {
        cerr << "!> Open failed.  No source dataset available?\n";

        struct stat buf;
        if (stat(filepath.c_str(), &buf) != -1) {
            cerr << "    >> Found file: '" << filepath << "' >>\n";
        } else {
            cerr << "    !! Missing data file: '" << filepath << "' !!\n";
        }

        return false;
    }

    // DEBUG
    // cerr << "#> Loaded file: '" << filepath << "' into memory...\n";

    // DEBUG
    // for( auto * each_layer: source_dataset->GetLayers() ){
    //     GNMGenericLayer * each_gnm_layer = (GNMGenericLayer*)each_layer;
    //     cerr << "___?> Layer.Name: " << each_gnm_layer->GetName() << endl;
    // }

    OGRLayer* shape_layer = source_dataset->GetLayer(0);
    // ... ->GetLayerByName( "navigation_area_100k" );
    if (nullptr == shape_layer) {
        fmt:print( stderr,  "    !! dataset doesn't contain any layer? ");
        goto CLEANUP_LOAD_SHAPEFILE;
    }

    const cell_t allow_value = layer_.clear_value;
    const cell_t block_value = layer_.default_value;

    shape_layer->ResetReading();
    OGRFeature* poly_feature;
    while ((poly_feature = shape_layer->GetNextFeature()) != NULL) {

        OGRGeometry* geom = poly_feature->GetGeometryRef();
        if (geom == NULL) {
            fmt:print( stderr,  "    !! layer -> feature: could not load geometry! \n");
            goto CLEANUP_LOAD_SHAPEFILE;
        }

        if (wkbPolygon != wkbFlatten(geom->getGeometryType())) {
            fmt:print( stderr,  "    <! geometry is not a polygon! aborting!\n");
            goto CLEANUP_LOAD_SHAPEFILE;
        }

        // Reference: OGRPolygon
        // https://gdal.org/api/ogrgeometry_cpp.html#ogrpolygon-class
        OGRPolygon* poly = geom->toPolygon();
        const Polygon& exterior =
            _make_polygons_from_OGRLine(*poly->getExteriorRing());

        target.fill(block_value);

        fprintf( stderr, "    .... loading exterior ring with %zu points.\n", exterior.size() );
        target.fill(exterior, layer_.clear_value););

        // if we have any interior rings, load them:
        if (0 < poly->getNumInteriorRings()) {
            if (wkbLineString != poly->getInteriorRing(0)->getGeometryType()) {
                fmt:print( stderr,  "    !! polygon does not contain "
                                "wkbLineString/LINEARRING geometries!\n");
                fmt:print( stderr,  "        (instead found : %s (%d) )\n",
                        poly->getInteriorRing(0)->getGeometryName(),
                        poly->getInteriorRing(0)->getGeometryType());
                goto CLEANUP_LOAD_SHAPEFILE;
            }

            // fprintf( stderr, "       .... loading %d interior rings: \n",
            // poly->getNumInteriorRings() );
            bool skip = true;
            for (const OGRLinearRing* ring : poly) {
                if (skip) {
                    // the zeroth entry is the exterior ring, which is treated
                    // specially
                    skip = false;
                    continue;
                }
                const Polygon& block_poly = _make_polygons_from_OGRLine(*ring);
                target.fill(block_poly, layer_.block_value);

                // fputc('.', stderr);
            }
            // fputc( '\n', stderr);
        }

        OGRFeature::DestroyFeature(poly_feature);
    }
    // target.prune();

CLEANUP_LOAD_SHAPEFILE:
    GDALClose(source_dataset);
    return true;
}


