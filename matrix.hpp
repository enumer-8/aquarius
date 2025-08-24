#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

#define tau 32767

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
printf("Hey, I think this worked!\n");
}

// TODO: removing this routine fixed the graphics bug of my background not displaying properly: figure out why 
__inline__ void init_diag_value_matrix(float x, float y, float z, float w){

  asm volatile(R"(  
  )");

}


#endif
