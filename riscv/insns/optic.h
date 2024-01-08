//require_either_extension('D', EXT_ZDINX); => double precision
require_either_extension('F', EXT_ZFINX); //=> single precision
require_fp;

//#define OPTIC_DOT_PRODUCT(len)

#define len 128

float32_t array_a[len]; 
float32_t array_b[len]; 
float32_t array_c[len];
float32_t result;
const reg_t baseAddr_a = RS1; 
const reg_t baseAddr_b = RS2; 
const reg_t baseAddr_result = RS3; 

        //parallel multiplciation
        for (reg_t i = 0; i < len; ++i) { 
                array_a[i].v =MMU.load<uint32_t>(baseAddr_a + i*sizeof(uint32_t)); 
                array_b[i].v =MMU.load<uint32_t>(baseAddr_b + i*sizeof(uint32_t)); 
                array_c[i] = f32_mul(array_a[i], array_b[i]);
        }
	//reduction sum
        result.v = 0;
        for (reg_t i = 0; i < len; ++i) { 
                result = f32_add(result, array_c[i]);
        }

softfloat_roundingMode = RM;

MMU.store<uint32_t>(baseAddr_result, result.v);
set_fp_exceptions;

WRITE_FRD_F(result); //-> can be replace by MMU.store to just store the results in the fp regfile
