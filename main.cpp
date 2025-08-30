/* copyright enumer8 2025 */ 

/* Aquarius - tiny aquarium scene
   programmed for the Dreamcast
   using PVR */

#include "matrix.hpp"
#include "dcm.h"
#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/fmath.h>
#include <dc/maple/controller.h>
#include <dc/biosfont.h>
#include <arch/timer.h>
#include <dc/perf_monitor.h>

// #include <dc/matrix.h>
// #include <dc/matrix3d.h>

#include <kos.h>

#include <stdio.h>
#include <stdbool.h>



// MAIN STARTS HERE!
int main(int argc, char **argv){



  vid_set_mode(DM_640x480, PM_RGB565);
  pvr_init_defaults();  

  init_scale_matrix(32.4f, 30.1f, 9.34f);

  pvr_set_bg_color(0.9f, 0.0f, 0.4f);

// EVENT LOOP BEGINS HERE
  while(1){   

    int32 i;
    for(i = 0; i < 360; i++){
      fast_deg2fsca(i);
	    printf("Degrees are: %ld\n", i);
    }

// PVR SCENE SETUP - OPAQUE DRAWING
    pvr_scene_begin();
    pvr_list_begin(PVR_LIST_OP_POLY);
    
    pvr_list_finish();
    pvr_scene_finish();

    }
  return 0;

}
