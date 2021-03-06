#ifndef BUILDERS_GENERATORS_TREEGENERATOR_HPP_DEFINED
#define BUILDERS_GENERATORS_TREEGENERATOR_HPP_DEFINED

#include "builders/generators/AbstractGenerator.hpp"
#include "builders/generators/CylinderGenerator.hpp"
#include "builders/generators/IcoSphereGenerator.hpp"
#include "meshing/MeshTypes.hpp"

#include <cmath>

namespace utymap { namespace builders {

// Generates tree.
class TreeGenerator
{
public:
    TreeGenerator(const utymap::builders::BuilderContext& builderContext,
                  const utymap::builders::MeshContext& meshContext,
                  const utymap::mapcss::ColorGradient& trunkGradient,
                  const utymap::mapcss::ColorGradient& foliageGradient) :
            trunkGeneratorMeshContext(meshContext.mesh, meshContext.style, trunkGradient),
            foliageGeneratorMeshContext(meshContext.mesh, meshContext.style, foliageGradient),

            trunkGenerator(builderContext, trunkGeneratorMeshContext),
            foliageGenerator(builderContext, foliageGeneratorMeshContext),

            position_(),
            trunkHeight_(0),
            trunkRadius_(0),
            foliageRadius_(0),
            foliageHeight_(0)
    {
    }

    TreeGenerator(const TreeGenerator&) = delete;

    // Sets position of tree
    TreeGenerator& setPosition(const utymap::meshing::Vector3& position)
    {
        position_ = position;
        return *this;
    }

    // Sets heigh of trunk
    TreeGenerator& setTrunkHeight(double height)
    {
        trunkHeight_ = height;
        return *this;
    }

    // Sets radius of trunk
    TreeGenerator& setTrunkRadius(double radius)
    {
        trunkRadius_ = radius;
        return *this;
    }

    // Sets radius of foliage
    TreeGenerator& setFoliageRadius(double radius, double height = 1)
    {
        foliageRadius_ = radius;
        foliageHeight_ = height;
        return *this;
    }

    // Sets trunk color noise freq
    TreeGenerator& setTrunkColorNoiseFreq(double noiseFreq)
    {
        trunkGenerator.setColorNoiseFreq(noiseFreq);
        return *this;
    }

    // Sets foliage color noise freq.
    TreeGenerator& setFoliageColorNoiseFreq(double noiseFreq)
    {
        foliageGenerator.setColorNoiseFreq(noiseFreq);
        return *this;
    }

    void generate()
    {
        // generate trunk
        trunkGenerator
            .setCenter(position_)
            .setHeight(trunkHeight_)
            .setRadius(trunkRadius_)
            .setMaxSegmentHeight(5)
            .setRadialSegments(7)
            .setVertexNoiseFreq(0.1f)
            .generate();

        // generate foliage
        foliageGenerator
            .setCenter(utymap::meshing::Vector3(
                position_.x,
                position_.y + trunkHeight_ + foliageRadius_,
                position_.z))
            .setRadius(foliageRadius_, foliageHeight_)
            .setRecursionLevel(1)
            .setVertexNoiseFreq(0.1f)
            .generate();
    }

private:
    utymap::builders::MeshContext trunkGeneratorMeshContext;
    utymap::builders::MeshContext foliageGeneratorMeshContext;
    CylinderGenerator trunkGenerator;
    IcoSphereGenerator foliageGenerator;
    meshing::Vector3 position_;
    double trunkHeight_, trunkRadius_, foliageRadius_, foliageHeight_;
};
}}

#endif // BUILDERS_GENERATORS_TREEGENERATOR_HPP_DEFINED
