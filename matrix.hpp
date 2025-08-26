#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <kos.h>


#define tau        32767 	// 0x7FFF, or FSCA maximum value
#define pi 	   16384
#define deg_180    8192
#define deg_45	   4096
#define magic_num  23301        // explained below

#define deg2rad_sf
#define rad2deg_sf



// NOTES ON TAU 
// =================================
// Basically, this is just a test of
// choosing tau over pi for reasons 
// relating to ease of conversion.
// Tau is 2pi and maps quite nicely
// to 32767 (technically 32768) so 
// I can use fixed point math with 
// FSCA and not have to worry too much
// about messing around with float
// truncation with FTRC. The actual
// instruction doesn't have that much
// overhead, but seeing if it's possible
// to avoid it anyway is a fun challenge.
// The plan is to have tau wrap around
// to 0 every time it reaches its true
// value of 32768 with:
// angle = (angle + delta) & 0x7FFF
//
// I guess I could define tau to be 
// 0x7FFF as well but I'm afraid to 
// use hex with defines for the moment 
// because KOS reasons :) 


// NOTES ON CALCULATING MAGIC NUMBER
// =================================
// I suck at math so I was worried 
// about this part. Essentially here's
// what's going on. 
//
// Experimenting with fast and hacky
// fixed-point math to tackle the FSCA
// instruction's requirement to take in
// angles in the form of ints from 0-32767
// convenient because this basically takes
// the form of a 16-bit integer (which I'm
// sure was the point) but regardless. 
// magic_num was calculated through:
//
// 	2pi rads = 360 degrees 
// 	or 1 full rotation
// 	magic_num = (32767 * 256) / 360
// 	= 8384512 / 360 which is roughly
// 	23301
//
// 	256 = 2^8 or >> 8
//
// degrees are taken in as a 32-bit int
// and then the calculation is performed
//
// safety: thinking of having the whole thing
// clamped from 0-32767 by just doing & 0x7FFF or tau


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

inline uint16 safe_deg2fsca(uint16 deg){

	deg = deg % 360;
	if (deg < 0) {
	    deg += 360;
	}

	uint32 deg_32 = (uint32)deg;

	uint32 angle = (deg_32 * 23301 + 128) >> 8;

	return angle & tau;
	
}


inline uint16 fast_deg2fsca(uint16 deg){

	if(deg >=360) deg -= 360;
	if(deg  <  0) deg += 360;

	uint32 deg_32 = (uint32)deg;

	uint32 angle = (deg_32 * 23301 + 128) >> 8;

	return angle & tau;
}
	


#endif
