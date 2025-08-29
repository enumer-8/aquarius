#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <kos.h>


#define tau 65535 	// 0xFFFF 

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

 typedef struct{ 
   float x, y, z, w;
} vec4_simple_t;

typedef struct __attribute__((packed, aligned(4))){
   float pos_x, pos_y, pos_z; 
   float orientation[3];
   float cam_fov; 
} simple_cam_t;


// Thanks for the notes on my diagonal matrix, Falco
__inline__ void init_identity(){
  asm(R"(

	  frchg             // change to back bank
	  fldi1 fr0         // load 1 into first matrix position
	  fschg             // enable double precision mode
	  fldi0 fr1         // zero loading in suitable spots
	  fldi0 fr2
	  fldi0 fr3
	  fldi0 fr4
	  fldi1 fr5
	  fmov dr2, dr6     // using fmov.d to move 0 pairs into fr6/7
	  fmov dr2, dr8     // same thing here 
	  fldi1 fr10
	  fmov dr2, dr12
	  fschg
	  fldi0 fr14
	  fldi1 fr15                                        
	  frchg                      

  )");
}

__inline__ void init_diag_value_matrix(float x, float y, float z, float w){

    asm(R"(  
  
    frchg                
   
    fldi0 fr1            
    fldi0 fr2
    fldi0 fr3
    fschg               
    fmov dr2, dr6        
    fmov dr2, dr8       
    fldi0 fr11
    fmov dr2, dr12      
    fschg                
    fldi0 fr14
    fmov.s @%[x], fr0     
    fmov.s @%[y], fr5
    fmov.s @%[z], fr10
    fmov.s @%[w], fr15
    frchg

    )" 
    
    : 
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z), [w] "r" (&w), "m" (x), "m" (y), "m" (z), "m" (w)
    );

}

__inline__ void init_scale_matrix(float x, float y, float z){
		
	init_diag_value_matrix(x, y, z, 1.0f);
}

inline int32 fast_deg2fsca(int32 deg){
    deg = deg % 360;
    if(deg < 0){
	deg += 360;
    } 
    return (deg * tau) / 360; 
}

__inline__ void mat4x4_rotate_z(int32 z_deg){
        
    asm volatile(R"(

    lds %0, fpul     
    frchg            
    fsca fpul, dr4   

    /* moving on to setting up the rest of the matrix */

    fldi0 fr2       
    fldi0 fr3
    fmov  fr4, fr1
    fmov  fr5, fr0
    fneg  fr4
    fschg    
    fmov  dr2, dr6
    fmov  dr2, dr8
    fldi1 dr10
    fldi0 fr11
    fmov  dr2, dr12
    fldi0 dr14
    fldi1 fr15
    fschg 
    frchg 
      )"
    
    :
    : "f" (z_deg)
    : "fpul"
    );
}

#endif
