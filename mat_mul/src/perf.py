#! /usr/bin/env python3
# -- coding: utf-8 --

# standard library
import subprocess
from subprocess import run
import warnings

# 3rd party packages
import numpy as np

# local source

# --------------------------------------------------------------------------- #
__author__ = "Juan Luis Padilla Salomé"
__copyright__ = "Copyright 2021"
__credits__ = ["University of Cantabria"]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Juan Luis Padilla Salomé"
__email__ = "juan-luis.padilla@alumnos.unican.es"
__status__ = "Production"
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #


class perf(object):
    """Permite hacer cambios relativos al PC en el que se ejecuta."""

    # Attributes
    # self.cores = [] # Array de cores logicos pertenecientes al mismo fisico

    def __init__(self):
        """Constructor de la clase perf."""
        super(perf, self).__init__()
        self.cores = self.__take_cores()
    # ----------------------------------------------------------------------- #

    def enable_nmi_watchdog(self, enable=True):
        """NMI viene activado, por defecto, y no permite la lectura de ciertos
        eventos. El metodo recibe como parametro un booleano para activarlo
        (True) o desactivarlo (False). Si no se pasa ningun parametro, se
        activa.
        Non-Maskable Interrupt(NMI) is the highest priority interrupt that
        can not be masked by any software."""

        value = 1  # Valor por defecto (activado)
        if not enable:
            value = 0
        # Se modifica el valor con sysctl, que permite hacer cambios al kernel
        command = ["sudo", "sysctl", "-w", "kernel.nmi_watchdog=" + str(value)]
        run(command, stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)
    # ----------------------------------------------------------------------- #

    def get_cores(self):
        """Devuelve un array con los cores logicos que pertenecen al mismo core
        fisico. Dicho array es de enteros y puede tener más de un valor. El
        core fisico devuelto por defecto es el 0 si no se pasa por parametro
        otro. i.e.: [0, 6] """

        return self.cores
    # ----------------------------------------------------------------------- #

    def get_num_logical_cores(self):
        """Ejecuta el comando 'nproc' que te retorna el numero de cores logicos
        en el sistema."""

        command = ["nproc"]
        data_run = run(command, stdout=subprocess.PIPE, encoding="utf-8")
        return data_run.stdout
    # ----------------------------------------------------------------------- #

    def perf(self):
        """Measure with perf."""

        command = ["perf", "list"]
        events = run(command, stdout=subprocess.PIPE, encoding="utf-8")

        command = ["perf"]
        data_run = run(command, stdout=subprocess.PIPE, encoding="utf-8")
        return data_run.stdout
    # ----------------------------------------------------------------------- #

    def set_environment(self, frequency=None):
        """Fija la frecuencia del CPU a una pasada por parametro y desactiva el
         turbo-boost mediante el uso de los registros msr. Si no se pasa una
         frecuencia, se fija por defecto a 2.2GHz. Formato de la frecuencia
         permitida: 800MHz, 1.9GHz, etc."""

        if frequency is None:
            frequency = "2.2GHz"  # Frecuencia por defecto

        # Carga libreria para hacer cambios en los registros msr
        run(["sudo", "modprobe", "msr"],
            stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)
        # Desactiva el turbo-boost
        msr = "0x1a0"
        code = "0x4000850089"
        for core in self.cores:  # Para cada core logico en el mismo fisico
            run(["sudo", "wrmsr", "--processor", str(core), msr, code],
                stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)

        command = ["sudo", "cpupower", "frequency-set"]
        # Establece el governor del CPU
        governor = "userspace"
        run(command + ["--governor", governor],
            stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)
        # Establece la frecuencia del CPU
        run(command + ["--freq", frequency],
            stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)
    # ----------------------------------------------------------------------- #

    def set_perf_event_paranoid(self, value=3):
        """
        ***************************************************************************
        * Insufficient permissions for accessing any hardware counters.           *
        * Your current paranoid level is 3.                                       *
        * Set /proc/sys/kernel/perf_event_paranoid to 2 (or less) or run as root. *
        *                                                                         *
        * Example:                                                                *
        * sudo sh -c "echo 2 > /proc/sys/kernel/perf_event_paranoid"              *
        ***************************************************************************
         -1 - Not paranoid at all
          0 - Disallow raw tracepoint access for unpriv
          1 - Disallow cpu events for unpriv
          2 - Disallow kernel profiling for unpriv
        """

        # value = 3  # Valor por defecto (activado)
        # Se modifica el valor con sysctl, que permite hacer cambios al kernel
        command = ["sudo", "sysctl", "-w",
                   "kernel.perf_event_paranoid=" + str(value)]
        run(command, stdout=subprocess.DEVNULL)  # , stderr=subprocess.STDOUT)
    # ----------------------------------------------------------------------- #

    def __take_cores(self, physical_core=None):
        """Guarda en un array los cores logicos que pertenecen al mismo core
        fisico. Dicho array es de enteros y puede tener más de un valor. El
        core fisico devuelto por defecto es el 0 si no se pasa por parametro
        otro. i.e.: [0, 6]"""

        cores = {}  # Diccionario de cores fisicos (key) y logicos (value)
        if physical_core is None:
            physical_core = 0  # Core fisico por defecto

        # Se ejecuta el comando para obtener la distribucion de los cores
        command = ["lscpu", "--all", "--extended=cpu,core"]
        data_run = run(command, stdout=subprocess.PIPE, encoding="utf-8")

        # Se filtra el resultado para obtener los cores fisicos
        data = data_run.stdout.split()  # ['CPU', 'CORE', '0', '0', '1', ... ]

        for i in range(2, len(data), 2):  # Se ignoran las cabeceras
            CPU = int(data[i])
            CORE = int(data[i + 1])
            aux = cores.get(CORE)
            if aux is None:  # No existe CORE
                aux = []
            aux.append(CPU)
            cores[CORE] = aux

        return cores.get(physical_core)  # print(cores.get(physical_core))
    # ----------------------------------------------------------------------- #

    def __warning_on_one_line(self, message, category, filename, lineno,
                              file=None, line=None):
        """Format the warning output."""

        return '%s:%s:\n\t%s: %s\n' % (filename, lineno, category.__name__,
                                       message)
    # ----------------------------------------------------------------------- #

    # ----------------------------------------------------------------------- #

    # define Python user-defined exceptions

    class Error(Exception):
        """Base class for other exceptions"""
        pass

    class MatricesUndefinedError(Error):
        """Raised when the input values are less than two arguments."""
        pass
    # ----------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
