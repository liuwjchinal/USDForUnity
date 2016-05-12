#pragma once

#define usdiCLinkage extern "C"
#ifdef _WIN32
    #ifndef usdiStaticLink
        #ifdef usdiImpl
            #define usdiExport __declspec(dllexport)
        #else
            #define usdiExport __declspec(dllimport)
        #endif
    #else
        #define usdiExport
    #endif
#else
    #define usdiExport
#endif

namespace usdi {

class Context;
class ImportContext;
class ExportContext;

class Schema;
class Xform;
class Camera;
class Mesh;


typedef unsigned int uint32;

struct float2
{
    float x, y;

    float2() {}
    float2(float _x, float _y) : x(_x), y(_y) {}
};

struct float3
{
    float x, y, z;

    float3() {}
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct float4
{
    float x, y, z, w;

    float4() {}
    float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), w(_w) {}
};

enum class SchemaType
{
    Unknown,
    Xform,
    Camera,
    Mesh,
};

enum class TopologyVariance
{
    Constant, // both vertices and topologies are constant
    Homogenous, // vertices are not constant (= animated). topologies are constant.
    Heterogenous, // both vertices and topologies are not constant
};

struct Time
{
    double time;
};

struct ImportConfig
{
    bool triangulate;
    bool swap_handedness;
    bool swap_faces;

    ImportConfig()
        : triangulate(true)
        , swap_handedness(true)
        , swap_faces(false)
    {}
};

struct ExportConfig
{
    bool swap_handedness;
    bool swap_faces;

    ExportConfig()
        : swap_handedness(true)
        , swap_faces(false)
    {}
};

struct XformData
{
    float3 position;
    float4 rotation;
    float3 scale;
};

struct CameraData
{
};

struct MeshData
{
    // these pointers can be null (in this case, just be ignored).
    // otherwise, if you pass to usdiMeshSampleReadData(), pointers must point valid memory block to store data.
    float3  *points;
    float3  *normals;
    int     *face_vertex_counts;
    int     *face_vertex_indices;
    int     *face_vertex_indices_triangulated;

    uint32  num_points;
    uint32  num_face_vertex_counts;
    uint32  num_face_vertex_indices;
    uint32  num_face_vertex_indices_triangulated;
};

} // namespace usdi

extern "C" {

usdiExport usdi::ImportContext*     usdiCreateImportContext(const char *path);
usdiExport void                     usdiDestroyImportContext(usdi::ImportContext *ctx);
usdiExport void                     usdiSetImportConfig(usdi::Context *ctx, const usdi::ImportConfig *conf);
usdiExport void                     usdiGetImportConfig(usdi::Context *ctx, usdi::ImportConfig *conf);
usdiExport void                     usdiSetExportConfig(usdi::Context *ctx, const usdi::ExportConfig *conf);
usdiExport void                     usdiGetExportConfig(usdi::Context *ctx, usdi::ExportConfig *conf);
usdiExport usdi::Schema*            usdiGetRoot(usdi::ImportContext *ctx);

usdiExport const char*              usdiGetPath(usdi::Schema *schema);
usdiExport const char*              usdiGetName(usdi::Schema *schema);
usdiExport usdi::SchemaType         usdiGetType(usdi::Schema *schema);
usdiExport usdi::Schema*            usdiGetParent(usdi::Schema *schema);
usdiExport int                      usdiGetNumChildren(usdi::Schema *schema);
usdiExport usdi::Schema*            usdiGetChild(usdi::Schema *schema, int i);

usdiExport usdi::Xform*             usdiAsXform(usdi::Schema *schema);
usdiExport usdi::Xform*             usdiCreateXform(usdi::Schema *parent, const char *name);
usdiExport void                     usdiXformReadSample(usdi::Xform *xf, usdi::XformData *dst, usdi::Time t);
usdiExport void                     usdiXformWriteSample(usdi::Xform *xf, const usdi::XformData *src, usdi::Time t);

usdiExport usdi::Camera*            usdiAsCamera(usdi::Schema *schema);
usdiExport usdi::Camera*            usdiCreateCamera(usdi::Schema *parent, const char *name);
usdiExport void                     usdiCameraReadSample(usdi::Camera *cam, usdi::CameraData *dst, usdi::Time t);
usdiExport void                     usdiCameraWriteSample(usdi::Camera *cam, const usdi::CameraData *src, usdi::Time t);

usdiExport usdi::Mesh*              usdiAsMesh(usdi::Schema *schema);
usdiExport usdi::Mesh*              usdiCreateMesh(usdi::Schema *parent, const char *name);
usdiExport void                     usdiMeshReadSample(usdi::Mesh *mesh, usdi::MeshData *dst, usdi::Time t);
usdiExport void                     usdiMeshWriteSample(usdi::Mesh *mesh, const usdi::MeshData *src, usdi::Time t);

} // extern "C"

