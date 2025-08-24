
#define PI 16384



__inline__ void rotation_matrix_y(){

    __asm__ __volatile__ (

      "FSCHG\n"           /* switch to double precision mode */ 
      "FSCA FPUL, DR4\n"  /* sin in FR4, cos in FR5
      ""
  )
}
