/* copyright enumer8 2025 */ 

/* Aquarius - tiny aquarium scene
   programmed for the Dreamcast
   using PVR */

/* DCM FILE FORMAT COMPONENT DEFINITIONS TAKEN FROM SIMULANT: https://gitlab.com/simulant/community/dcm */    

#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/fmath.h>
#include <dc/maple/controller.h>
#include <dc/biosfont.h>
// #include <dc/matrix.h>
// #include <dc/matrix3d.h>

#include <kos.h>

#include <stdio.h>
#include <stdbool.h>

#define VERBOSE

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

// TRANSFORMATION MATRICES SECTION
// =================================
// NOTES TO SELF: VERTICES SHOULD BE
// TRANSFORMED BEFORE SUBMITTED  
// KOS ALLOCATES AND MANAGES BUFFER
// HAS TO BE REGENERATED EVERY FRAME
// =================================

      //   | 1 0 0 0 | fr0 ..
      //   | 0 1 0 0 | fr1 ..
      //   | 0 0 1 0 | fr2 .. 
      //   | 0 0 0 1 | fr3 ..

// Just for the sake of understanding: basic identity matrix
// setup -> SH-4 works in column major order 

// basic scale matrix test - thanks @pikuma :)
// should probably init identity matrix first lol
// 
 typedef struct __attribute__((aligned(32))){
   float m[4][4];
 } mat4x4_t;

 typedef struct __attribute__((packed, aligned(4))){
   float x, y, z, w;
 } vec4_simple_t;


// thanks for the explanation @Falco Girgis
void init_identity(){
  __asm__ __volatile__  (

  "frchg\n"             // change to back bank
  "fldi1 fr0\n"         // load 1 into first matrix position
  "fschg\n"             // enable double precision mode
  "fldi0 fr1\n"         // zero loading in suitable spots
  "fldi0 fr2\n"
  "fldi0 fr3\n"
  "fldi0 fr4\n"
  "fldi1 fr5\n"
  "fmov dr2, dr6\n"     // using fmov.d to move 0 pairs into fr6/7
  "fmov dr2, dr8\n"     // same thing here 
  "fldi1 fr10\n"
  "fmov dr2, dr12\n"
  "fschg\n"
  "fldi0 fr14\n"
  "fldi1 fr15\n"                                        
  "frchg\n"                      

  );
printf("Hey, I think this worked!\n");
}


// DCM FILE LOADER FUNCTION - thanks @Falco Girgis
// ========================
void per_frame(void){
  void dcm_vertex_submit(dcm_vertex *vert, bool final_tri_strip){

     pvr_vertex_t submitted_vert;
     
               submitted_vert.flags = final_tri_strip? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
               submitted_vert.x     = vert->x,
               submitted_vert.y     = vert->y,
               submitted_vert.z     = vert->z,
               submitted_vert.argb  = vert->color;
               submitted_vert.u     = vert->u;
               submitted_vert.v     = vert->v;
               submitted_vert.oargb = 0;
    
       pvr_prim(&submitted_vert, sizeof(submitted_vert));
   } 
}

// ENTITY (FISH)
// typedef struct __attribute__ ((packed, aligned(4))) {
//   uint16 pos_x, pos_y, pos_z;
//   uint8  vel_x, vel_y, vel_z;
//   uint8  color_id;
//   fish_state_t state;
// } Fish_entity;

// typedef enum state {
//   FISH_IDLE,
//   FISH_SWIMMING,
//   FISH_SLEEPING,
// } fish_state_t;

// MAIN STARTS HERE!
int main(int argc, char **argv){

  vid_set_mode(DM_640x480, PM_RGB565);
  pvr_init_defaults();  

  pvr_set_bg_color(0.1f, 0.0f, 0.4f);

  dcm_small_file_hdr dcm_file_hdr;
  
// EVENT LOOP BEGINS HERE
  while(1){    

// ADDRESS POINTER TEST
    bfont_draw_str_vram_fmt(30, 30, true, "Address of file_hdr is: %p", (void *)&dcm_file_hdr.version);

// MATRIX TEST

    init_identity();
     
// PVR SCENE SETUP - OPAQUE DRAWING
    pvr_scene_begin();
    pvr_list_begin(PVR_LIST_OP_POLY);
    
    pvr_list_finish();
    pvr_scene_finish();

    }

  return 0;

  }
