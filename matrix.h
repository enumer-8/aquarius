#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

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

__inline__ void init_identity(){
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

__inline__ void init_diag_value_matrix(float x, float y, float z, float w){

  init_identity();

  __asm__ __volatile__ (
  
  "fmov %[x], fr0\n"
  "fmov %[y], fr5\n"
  "fmov %[z], fr10\n"
  "fmov %[w], fr15\n"
  "frchg\n"  
  
    :
    : [x] "f" (x),
      [y] "f" (y),
      [z] "f" (z),
      [w] "f" (w)

    : "fr0", "fr5", "fr10", "fr15"

  );

}

__inline__ void rotation_matrix(){
  
}

#endif
