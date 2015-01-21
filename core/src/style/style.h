#pragma once

#include <string>
#include <vector>
#include <set>

#include "util/vertexLayout.h"
#include "util/shaderProgram.h"
#include "util/mapProjection.h"
#include "tile/mapTile.h"
#include "tileData.h"
#include "platform.h"
#include "gl.h"

/* Means of constructing and rendering map geometry
 *
 * A Style defines a way to 
 *   1. Construct map geometry into a mesh for drawing and 
 *   2. Render the resulting mesh in a scene
 * Style implementations must provide functions to construct
 * a <VertexLayout> for their geometry, construct a <ShaderProgram>
 * for rendering meshes, and build point, line, and polygon
 * geometry into meshes. See <PolygonStyle> for a basic implementation.
 */
class Style {
    
protected:
    
    /* Unique name for a style instance */
    std::string m_name;
    
    /* <ShaderProgram> used to draw meshes using this style */
    std::shared_ptr<ShaderProgram> m_shaderProgram;
    
    /* <VertexLayout> shared between meshes using this style */
    std::shared_ptr<VertexLayout> m_vertexLayout;
    
    /* Draw mode to pass into <VboMesh>es created with this style */
    GLenum m_drawMode;
    
    /* Set of strings defining which data layers this style applies to */
    std::set<std::string> m_layers;
    
    /* Create <VertexLayout> corresponding to this style */
    virtual void constructVertexLayout() = 0;
    
    /* Create <ShaderProgram> for this style */
    virtual void constructShaderProgram() = 0;
    
    /* Build styled vertex data for point geometry and add it to the given <VboMesh> */
    virtual void buildPoint(Point& _point, std::string& _layer, Properties& _props, VboMesh& _mesh) = 0;
    
    /* Build styled vertex data for line geometry and add it to the given <VboMesh> */
    virtual void buildLine(Line& _line, std::string& _layer, Properties& _props, VboMesh& _mesh) = 0;
    
    /* Build styled vertex data for polygon geometry and add it to the given <VboMesh> 
     * 
     * Polygon geometry is provided as a vector of all points in the polygon and a vector
     * containing the number of points in each contour (or ring) of the polygon. For a
     * simple polygon (in the mathematical sense), _sizes will have one element which is
     * the number of points in the first vector.
     */
    virtual void buildPolygon(Polygon& _polygon, std::string& _layer, Properties& _props, VboMesh& _mesh) = 0;

public:

    Style(std::string _name, GLenum _drawMode);

    /* Add layers to which this style will apply 
     * TODO: More flexible filtering */
    virtual void addLayers(std::vector<std::string> _layers);
    
    /* Add styled geometry from the given Json object to the given <MapTile> */
    virtual void addData(TileData& _data, MapTile& _tile, const MapProjection& _mapProjection);
    
    /* Perform any setup needed before drawing each frame */
    virtual void setupFrame() = 0;

    /* Perform any setup needed before drawing each tile */
    virtual void setupTile(const std::shared_ptr<MapTile>& _tile) = 0;

    std::shared_ptr<ShaderProgram> getShaderProgram() const { return m_shaderProgram; }
    std::string getName() const { return m_name; }

    virtual ~Style();
};
