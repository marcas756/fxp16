import math
import numpy


flt_consts= {
    "M_E":math.e,
    "M_LOG2E": math.log2(math.e),
    "M_LOG10E": math.log10(math.e),
    "M_LN2": math.log(2),
    "M_PI": math.pi,
    "M_PI_2": math.pi/2,
    "M_PI_4": math.pi/4,
    "M_1_PI": 1/math.pi,
    "M_2_PI": 2/math.pi,
    "M_2_SQRTPI": 2/math.sqrt(math.pi),
    "M_SQRT2": math.sqrt(2),
    "M_SQRT1_2": math.sqrt(1/2),
    "M_TWOPI": 2*math.pi,
    "M_3PI_4": 3*math.pi/4,
    "M_SQRTPI": math.sqrt(math.pi),
    "M_SQRT3": math.sqrt(3),
    "M_IVLN10": 1/math.log(10),
    "M_LOG2_E": math.log2(math.e),
    "M_INVLN2": 1/math.log(2),
    "ONE_EIGHTH": 1/8,
    "ONE_QUARTER": 1/4,
    "THREE_EIGHTHS": 3/8,
    "ONE_HALF": 1/2,
    "FIVE_EIGHTHS": 5/8,
    "THREE_QUARTERS": 3/4,
    "SEVEN_EIGHTHS": 7/8,
    "ONE_THIRD": 1/3,
    "TWO_THIRDS": 2/3,
    "ONE": 1,
    "MINUS_ONE": -1
}

def fp16_sat(fp):

    if fp < numpy.iinfo(numpy.int16).min :
        fp =  numpy.iinfo(numpy.int16).min
    if fp > numpy.iinfo(numpy.int16).max :
        fp =  numpy.iinfo(numpy.int16).max

    return fp

def fp16_flt2fp(flt,frac):
    return round(flt * (1 << frac))

def fp16_fp2flt(fp,frac):
    return ((float)(fp)) / (1 << frac)

def print_define_part(x):
    print('{0: <35}'.format(x),end="")


def print_value_part(x):
    print('{0: <20}'.format(x),end="")

def print_Qs():

    for q in range (0,16):
        print("// --------------------------------------------------------------------------")
        print_define_part("#define FP16_Q"+ str(q))
        print_value_part("("+str(q)+")")
        print()

        print_define_part("#define FP16_Q" + str(q)+ "_PREC")
        print_value_part("("+str(fp16_fp2flt(1,q))+")")
        print()

        print_define_part("#define FP16_Q" + str(q) + "_MAX ")
        print_value_part("(" + str(fp16_fp2flt(numpy.iinfo(numpy.int16).max, q)) + ")")
        print()

        print_define_part("#define FP16_Q" + str(q) + "_MIN ")
        print_value_part("(" + str(fp16_fp2flt(numpy.iinfo(numpy.int16).min, q)) + ")")
        print()
        print("// --------------------------------------------------------------------------")

        for key, flt in flt_consts.items():

            fp = fp16_flt2fp(flt, q)

            if fp != fp16_sat(fp):
                print_define_part("#define FP32_Q" + str(q) + "_" + key)
            else :
                print_define_part("#define FP16_Q" + str(q) + "_" + key)

            print_value_part("("+str(fp)+")")
            print("// " + str(fp16_fp2flt(fp,q)))

        print()
        print()



if __name__ == '__main__':
    print_Qs()
