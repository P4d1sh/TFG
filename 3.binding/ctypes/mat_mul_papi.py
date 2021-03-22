#! /usr/bin/env python3
# -- coding: utf-8 --

# standard library
import sys
import ctypes
from ctypes import *
# import pathlib

# 3rd party packages
import numpy as np

# local source
sys.path.append("/home/jlpadillas01/TFG/1.mat_mul/src/")
from mat_mul import matrix

# ------------------------------------------------------------------------ #
# Multiplicacion de dos matrices: A = M  N usando numpy

__author__ = "Juan Luis Padilla Salomé"
__copyright__ = "Copyright 2021"
__credits__ = ["Universidad de Cantabria"]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Juan Luis Padilla Salomé"
__email__ = "juan-luis.padilla@alumnos.unican.es"
__status__ = "Production"
# ------------------------------------------------------------------------ #

if __name__ == "__main__":
    """Dependiendo del valor pasado por parametro, se ejecuta la multipli-
    cacion de dos matrices rellenadas con la funcion empty() o zeros(),
    ambas pertenecientes a la liberia Numpy.
    @param option variable que se pasa por parametro y que indica si se ha
        de rellenar las matrices con la funcion empty() o zeros().
    """
    # standard library
    import sys

    # 3rd party packages

    # TODO: Asignar una opción para generar las matrices
    option = "zeros"
    # if len(sys.argv) > 1:
    #     option = sys.argv[1]

    # Se usan matrices cuadradas para facilitar el calculo de operaciones.
    dim_x = 5000
    dim_y = dim_x

    # Se crea el objeto
    mat = matrix(dim_x, dim_y)
    # mat = matrix()

    # Se generan las dos matrices
    if option == "empty":
        mat.empty_matrices()
    elif option == "zeros":
        mat.zeros_matrices()
    else:
        print("ERROR: Wrong generation of matrices. Run the program with "
              "argument 'empty' or 'zeros'.")
        raise mat.Error

    # Load the shared library into ctypes
    # libname = pathlib.Path().absolute() / "libpapi.so.6.0"
    libname = "/home/jlpadillas01/TFG/2.compilation/lib/libmy_papi.so"
    p_lib = CDLL(libname)

    # TODO: Hay que cambiar el path para que se pueda encontrar el .so
    # export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:/usr/local/lib
    # TODO: Hay que habilitar la lectura de los eventos bajando el nivel de seg.
    # sudo sysctl -w kernel.perf_event_paranoid=1

    # print(type(p_lib))
    # print("PAPI_is_initialized() = ", p_lib.PAPI_is_initialized())

    # ROI
    # # ----------------------------------------
    # ptr_EventSet = ctypes.pointer(ctypes.c_int())
    # values = ctypes.c_longlong

    # # 1.
    # p_lib.my_PAPI_library_init()
    # # 2.
    # p_lib.my_PAPI_create_eventset(ptr_EventSet)
    # # 3.
    # # p_lib.my_PAPI_add_named_event(ptr_EventSet, "SIMD_FP_256:PACKED_DOUBLE")
    # # p_lib.my_PAPI_add_named_event(ptr_EventSet, "FP_COMP_OPS_EXE:SSE_SCALAR_DOUBLE")
    # # 4.
    # p_lib.my_PAPI_start(ptr_EventSet)

    # # ---------------------
    # # ROI -> Se multiplican
    # mat.multiply()
    # # ---------------------

    # # 5.
    # p_lib.stop(ptr_EventSet, values)
    # # 6.
    # p_lib.my_PAPI_shutdown()

    # print(values)

    # # ----------------------------------------

    # 1.
    ev = "CYCLES"
    event = c_wchar_p(ev)
    # event.value = "CYCLES"
    ptr_EventSet = p_lib.my_start_events(event, 1)
    # ptr_EventSet = p_lib.my_start_events("SIMD_FP_256:PACKED_DOUBLE", 1)
    mat.multiply()
    values = p_lib.my_stop_events(ptr_EventSet, 1)


    print(values)