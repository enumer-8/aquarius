#ifndef DCM_H
#define DCM_H

#include <kos.h>

/* DCM FILE FORMAT COMPONENT DEFINITIONS TAKEN FROM SIMULANT: https://gitlab.com/simulant/community/dcm */    

// DEFINING DCM FILE FORMAT FOR PARSING
// ====================================

// DCM FILE HEADER - 12 BYTES WHEN ALIGNED
typedef struct __attribute__ ((packed, aligned(4))) {
  char id[3];
  uint8 version;
  uint8 material_count;
  uint8 mesh_count;
  uint8 index_size;
  uint8 pos_format;
  uint8 color_format;
  uint8 normal_format;
} dcm_small_file_hdr;

// DCM DATA HEADER
typedef struct __attribute__((packed, aligned(4))) {
  uint8 flags;
  uint8 id;
  char path[32];
} dcm_data_hdr;

// DCM MATERIAL HEADER - ONLY KEEPING WHAT IS NECESSARY
typedef struct __attribute__((packed, aligned(4))) {
  uint8 id;
  uint32 color;
  char _padding[3];
} dcm_mat_hdr;

// DCM MESH HEADER - 8 BYTES WHEN ALIGNED
typedef struct __attribute__ ((packed, aligned(4))) {
  dcm_data_hdr data_hdr;
  uint8 sm_count;
  uint32 vert_count;
 } dcm_mesh_hdr;

// DCM VERTEX STRUCT - ALIGNED 32 BYTES FOR PVR
typedef struct __attribute__ ((packed, aligned(32))) {
  float x, y, z;              // 12 bytes
  float u, v;                 // 8  bytes
  char _padding[7];           // 7  bytes
  uint32 color;               // 4  bytes 
  uint8 normal;               // 1  byte
} dcm_vertex; 

// DCM SUBMESH HEADER
typedef struct __attribute__ ((packed, aligned(4))) {
  dcm_data_hdr data_hdr;
  uint8 mat_id;
  uint8 arrangement;
  uint8 type;
  uint8 num_range_or_indices;
} dcm_submesh_hdr;

// DCM VERTEX RANGE STRUCT
typedef struct __attribute__ ((packed, aligned(4))) {
  uint32 start;
  uint32 count;
} dcm_vertex_range;

// DCM SUBMESH FORMAT
typedef struct __attribute__ ((packed, aligned(4))) {
  dcm_submesh_hdr submesh_hdr;
  dcm_vertex_range vert_range;
} dcm_submesh_format;

// DCM MESH FORMAT
typedef struct __attribute__ ((packed, aligned(4))) {
  dcm_mesh_hdr        mesh_hdr;
  dcm_vertex          *vertices;
  dcm_submesh_format  *submesh;
} dcm_mesh_format;

// COMPLETE DCM FILE FORMAT
typedef struct __attribute__ ((packed, aligned(4))) {
  dcm_small_file_hdr dcm_file_hdr;
  dcm_mat_hdr        materials;
  dcm_mesh_format    mesh;
} dcm_file_format;
// ===================================================
// END DCM FILE FORMAT DEFINITION

// DCM FILE LOADER FUNCTION - thanks @Falco Girgis
// ========================
__inline__ void dcm_vertex_submit(dcm_vertex *vert, bool final_tri_strip){

     pvr_vertex_t submitted_vert;
     
               submitted_vert.flags = final_tri_strip ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
               submitted_vert.x     = vert->x,
               submitted_vert.y     = vert->y,
               submitted_vert.z     = vert->z,
               submitted_vert.argb  = vert->color;
               submitted_vert.u     = vert->u;
               submitted_vert.v     = vert->v;
               submitted_vert.oargb = 0;
    
       pvr_prim(&submitted_vert, sizeof(submitted_vert)); 
}

#endif
