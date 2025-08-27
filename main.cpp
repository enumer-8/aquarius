/* copyright enumer8 2025 */ 

/* Aquarius - tiny aquarium scene
   programmed for the Dreamcast
   using PVR */

/* DCM FILE FORMAT COMPONENT DEFINITIONS TAKEN FROM SIMULANT: https://gitlab.com/simulant/community/dcm */    

#include "matrix.hpp"
#include "dcm.h"

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

// MAIN STARTS HERE!
int main(int argc, char **argv){



  vid_set_mode(DM_640x480, PM_RGB565);
  pvr_init_defaults();  

  pvr_set_bg_color(0.1f, 0.0f, 0.4f);

  dcm_small_file_hdr dcm_file_hdr;
  fast_deg2fsca(319);

// EVENT LOOP BEGINS HERE
  while(1){    

// ADDRESS POINTER TEST
    bfont_draw_str_vram_fmt(30, 30, true, "Address of file_hdr version  is: %p",   (void *)&dcm_file_hdr.version);
    bfont_draw_str_vram_fmt(30, 60, true, "Address of file_hdr material is: %p",   (void *)&dcm_file_hdr.material_count);
    bfont_draw_str_vram_fmt(30, 90, true, "Address of file_hdr index size is: %p", (void *)&dcm_file_hdr.index_size);

// PVR SCENE SETUP - OPAQUE DRAWING
    pvr_scene_begin();
    pvr_list_begin(PVR_LIST_OP_POLY);
    
    pvr_list_finish();
    pvr_scene_finish();

    }

  return 0;

}
