#-----------------------------------------------------------------------
# Unauthorized use or distribution of this file is not permitted.
#
# Author: Ruud van der Pas, Sun Microsystems
#
# Copyright: Sun Microsystems, All rights reserved, Un-authorized
#            distribution not permitted
#-----------------------------------------------------------------------

FAST     =                                #-- Most important for performance                 
EXTRA    =  #-- Additional optimization options
PAR      =                 #-- Auto-parallelization options
OMP      = -fopenmp                                   #-- OpenMP parallelization options

#-----------------------------------------------------------------------
# These are threshold values to be changed as part of the OpenMP lab
#-----------------------------------------------------------------------
THRESHOLD_ROW = 1
THRESHOLD_COL = 1

#-----------------------------------------------------------------------
# No Changes Needed Below This Line
#-----------------------------------------------------------------------
PROCS    = 1 2 4 8                 #-- Number of threads used for parallel jobs
DEBUG    = -g                    #-- Debug and compiler commentary
ISA      = -std=gnu99         #-- Instruction set
OPT_BASE = $(ISA)        #-- Options for basic routines
OPT      = $(FAST) $(EXTRA)
CFLAGS   = $(OPT) $(ISA)
LDFLAGS  = $(OPT) $(ISA)
SATSLIB  = 
PERFLIB  = 
LIBS     = -lm
MEMALLOC = -lumem
MOMP     = -fopenmp -xO3
CC       = cc
F95      = f95
CODE     = mxv_base

.IGNORE:

.SUFFIXES: .c .o

.IGNORE:

#-----------------------------------------------------------------------
# Definition of matrix sizes
#-----------------------------------------------------------------------
SIZES = 5 10 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 150 200 250 300 \
325 350 375 400 500 525 550 575 600 650 675 700 725 750 800 900 950 975 1000 1025 \
1050 1100 1200 1300 1400 1500 1600 1700 1800 1900 1950 1975 2000 2025 2050 2100 \
3000 3500 4000 4500 5000 5500 6000

PROF_SIZE   = 5000
PROF_REPEAT = 40

#-----------------------------------------------------------------------
# Definition of objects
#-----------------------------------------------------------------------
OBJ_SHRD    = check_results.o 
OBJ_BASE    = main.o init_data.o mxv_row_base.o mxv_col_base.o 
OBJ_SERIAL  = main.o init_data.o mxv_row.o mxv_col.o 
OBJ_PAR     = main.o init_data.o mxv_row_par.o mxv_col_par.o 
OBJ_OMP     = main.o init_data.o mxv_row_omp.o mxv_col_omp.o 
OBJ_PARFT   = main.o init_data_omp.o mxv_row_par.o mxv_col_par.o 
OBJ_OMPFT   = main.o init_data_omp.o mxv_row_omp.o mxv_col_omp.o 

#-----------------------------------------------------------------------
# Definition of executables
#-----------------------------------------------------------------------
EXE0 = mxv_base.exe
EXE1 = mxv_serial.exe
EXE2 = mxv_par.exe
EXE3 = mxv_omp.exe
EXE4 = mxv_parft.exe
EXE5 = mxv_ompft.exe

#-----------------------------------------------------------------------
# Basic commands
#-----------------------------------------------------------------------
default: help

check:
	$(F95) -V
	$(CC) -V
	collect -V
	analyzer -V

help:
	@echo "Program targets are:"
	@echo "   base    - compiled without optimization"
	@echo "   serial  - optimized by the compiler"
	@echo "   par     - automatically parallelized by the compiler"
	@echo "   omp     - explicitly parallelized with OpenMP"
	@echo "   ompth   - explicitly parallelized with OpenMP - use thresholds"
	@echo "   parft   - automatically parallelized version optimized for first touch"
	@echo "   ompft   - OpenMP version optimized for first touch"
	@echo ""
	@echo "Note: we will refer to these targets as <key> below"
	@echo ""
	@echo "make <key> - command to build specific a version of the program"
	@echo "  example: % make base - build the non-optimized version"
	@echo ""
	@echo "make run_<key> - command to run a specific version"
	@echo "  example: % make run_base - run the non-optimized version"
	@echo ""
	@echo "make plot_<key> - command to plot the results for a specific version"
	@echo "  example: % make plot_base - plot performance of the non-optimized version"
	@echo ""
	@echo "Global commands (apply to all versions):"
	@echo "make build - build all versions"
	@echo "make run   - run   all versions"
	@echo "make plot  - plot  all performance results"
	@echo ""
	@echo "Various other commands supported:"
	@echo "make                     - Equivalent to make help"
	@echo "make help                - On-line info"
	@echo "make check               - Check release numbers of compilers and tools"
	@echo "make collect             - Run the collect command on $(CODE).exe"
	@echo "make profile             - Use the analyzer GUI to show the results of the"
	@echo "                           collect run"
	@echo "make counters            - Run the performance counters experiments for $(CODE).exe"
	@echo "make profile_counters    - Use the analyzer GUI to display the result of"
	@echo "                           the counter experiments"
	@echo "make profile_cntrs_ascii - Use er_print to display the result of the counter"
	@echo "                           experiments"
	@echo " NOTES - By default these profile and counter experiments will be done 
	@echo "         for $(CODE).exe.
	@echo "         You can override this by changing the make variable CODE"
	@echo "         For example: make counters CODE=mxv_serial"
	@echo "make collect_par         - Run the collect command on the OpenMP version"
	@echo "make profile_par         - Use the analyzer to view the OpenMP performance results"
	@echo "make clean               - Remove all objects and executables"
	@echo "make veryclean           - ARE YOU SURE ? Will remove all objects, executables,"
	@echo "                           listings, analyzer experiments plus all .dat files"

base    : $(EXE0)
serial  : $(EXE1)
par     : $(EXE2)
omp     : $(EXE3)
parft   : $(EXE4)
ompft   : $(EXE5)

#-----------------------------------------------------------------------
# Overall commands to build, run and plot all experiments
#-----------------------------------------------------------------------
build:
	make base serial par omp
# Emil commented out
#	make base serial par omp parft ompft
run:
	make check run_base run_serial collect counters profile_cntrs_ascii run_par \
    run_omp collect_par
plot:
	make plot_base plot_serial plot_par plot_omp

#-----------------------------------------------------------------------
# Plot section
#-----------------------------------------------------------------------
plot_base:
	@echo "Performance plots for non-optimized version"
	gnuplot plot_base.gnu
plot_serial:
	@echo "Performance plots for version optimized by the compiler"
	gnuplot plot_serial.gnu
plot_par:
	@echo "Performance plots for the automatically parallelized version"
	gnuplot plot_par.gnu
plot_omp:
	@echo "Performance plots for the version parallelized with OpenMP"
	gnuplot plot_omp.gnu
plot_ompth:
	@echo "Performance plots for the version parallelized with OpenMP using thresholds"
	gnuplot plot_ompth.gnu
plot_parft:
	@echo "Performance plots for the automatically parallelized version optimized for first touch"
	gnuplot plot_parft.gnu
plot_ompft:
	@echo "Performance plots for the OpenMP version optimized for first touch"
	gnuplot plot_ompft.gnu

#-----------------------------------------------------------------------
# Analyzer experiments (hardware counter experiments are defined below)
#-----------------------------------------------------------------------
collect: $(CODE).exe
	(OMP_NUM_THREADS=1; export OMP_NUM_THREADS                                                 ;\
    SATS_LAB_HEADER=y; export SATS_LAB_HEADER                                                  ;\
	er_rm -f prof_$(CODE).er; /bin/rm -f perf.dat                                              ;\
	collect -o prof_$(CODE).er ./$(CODE).exe -m $(PROF_SIZE) -n $(PROF_SIZE) -f $(PROF_REPEAT) ;\
	mv perf.dat perf_collect.dat)

profile:
	analyzer prof_$(CODE).er

collect_par: $(EXE3)
	(OMP_NUM_THREADS=2; export OMP_NUM_THREADS                                         ;\
    SATS_LAB_HEADER=y; export SATS_LAB_HEADER                                          ;\
	er_rm -f prof_omp.er; /bin/rm -f perf.dat                                          ;\
	collect -o prof_omp.er ./$(EXE3) -m $(PROF_SIZE) -n $(PROF_SIZE) -f $(PROF_REPEAT) ;\
	mv perf.dat perf_collect_omp.dat)

profile_par:
	analyzer prof_omp.er

#-----------------------------------------------------------------------
# Run commands
#-----------------------------------------------------------------------
run_base: $(EXE0)
	@echo "===================== Results for non-optimized version ==============="
	(OMP_NUM_THREADS=1; export OMP_NUM_THREADS ;\
    SATS_LAB_HEADER=y; export SATS_LAB_HEADER  ;\
	/bin/rm -f perf.dat                        ;\
	for S in $(SIZES)                          ;\
	do ./$(EXE0) -m $$S -n $$S                 ;\
	   SATS_LAB_HEADER=n                       ;\
	done                                       ;\
	mv perf.dat base.dat)
run_serial: $(EXE1)
	@echo "===================== Results for compiler optimized version =========="
	(OMP_NUM_THREADS=1; export OMP_NUM_THREADS ;\
    SATS_LAB_HEADER=y; export SATS_LAB_HEADER  ;\
	/bin/rm -f perf.dat                        ;\
	for S in $(SIZES)                          ;\
	do ./$(EXE1) -m $$S -n $$S                 ;\
	   SATS_LAB_HEADER=n                       ;\
	done                                       ;\
	mv perf.dat serial.dat)
run_par: $(EXE2)
	@echo "===================== Results for auto-par version ===================="
	for P in $(PROCS)                   ;\
    do OMP_NUM_THREADS=$$P              ;\
       export OMP_NUM_THREADS           ;\
       SUNW_MP_THR_IDLE=spin            ;\
       export SUNW_MP_THR_IDLE          ;\
       SATS_LAB_HEADER=y                ;\
       export SATS_LAB_HEADER           ;\
	   /bin/rm -f perf.dat              ;\
	   for S in $(SIZES)                ;\
	   do ./$(EXE2) -m $$S -n $$S       ;\
	      SATS_LAB_HEADER=n             ;\
	   done                             ;\
	   mv perf.dat par_$$P"_procs.dat"  ;\
    done
run_omp: $(EXE3)
	@echo "===================== Results for OpenMP version ======================"
	for P in $(PROCS)                   ;\
    do OMP_NUM_THREADS=$$P              ;\
       export OMP_NUM_THREADS           ;\
       OMP_DYNAMIC=FALSE                ;\
       export OMP_DYNAMIC               ;\
       SUNW_MP_THR_IDLE=spin            ;\
       export SUNW_MP_THR_IDLE          ;\
       SATS_LAB_HEADER=y                ;\
       export SATS_LAB_HEADER           ;\
	   /bin/rm -f perf.dat              ;\
	   for S in $(SIZES)                ;\
	   do ./$(EXE3) -m $$S -n $$S       ;\
	      SATS_LAB_HEADER=n             ;\
	   done                             ;\
	   mv perf.dat omp_$$P"_procs.dat"  ;\
    done
run_ompth: $(EXE3)
	@echo "===================== Results for OpenMP version with thresholds ======"
	for P in $(PROCS)                                                     ;\
    do OMP_NUM_THREADS=$$P                                                ;\
       export OMP_NUM_THREADS                                             ;\
       OMP_DYNAMIC=FALSE                                                  ;\
       export OMP_DYNAMIC                                                 ;\
       SUNW_MP_THR_IDLE=spin                                              ;\
       export SUNW_MP_THR_IDLE                                            ;\
       SATS_LAB_HEADER=y                                                  ;\
       export SATS_LAB_HEADER                                             ;\
	   /bin/rm -f perf.dat                                                ;\
	   for S in $(SIZES)                                                  ;\
	   do ./$(EXE3) -m $$S -n $$S -r $(THRESHOLD_ROW) -c $(THRESHOLD_COL) ;\
	      SATS_LAB_HEADER=n                                               ;\
	   done                                                               ;\
	   mv perf.dat ompth_$$P"_procs.dat"                                  ;\
    done
run_parft: $(EXE4)
	@echo "===================== Results AutoPar version optimized for First Touch"
	for P in $(PROCS)                                                     ;\
    do OMP_NUM_THREADS=$$P                                                ;\
       export OMP_NUM_THREADS                                             ;\
       OMP_DYNAMIC=FALSE                                                  ;\
       export OMP_DYNAMIC                                                 ;\
       SUNW_MP_THR_IDLE=spin                                              ;\
       export SUNW_MP_THR_IDLE                                            ;\
       SATS_LAB_HEADER=y                                                  ;\
       export SATS_LAB_HEADER                                             ;\
	   /bin/rm -f perf.dat                                                ;\
	   for S in $(SIZES)                                                  ;\
	   do ./$(EXE4) -m $$S -n $$S -r $(THRESHOLD_ROW) -c $(THRESHOLD_COL) ;\
	      SATS_LAB_HEADER=n                                               ;\
	   done                                                               ;\
	   mv perf.dat parft_$$P"_procs.dat"                                  ;\
    done
run_ompft: $(EXE5)
	@echo "===================== Results OpenMP version optimized for First Touch "
	for P in $(PROCS)                                                     ;\
    do OMP_NUM_THREADS=$$P                                                ;\
       export OMP_NUM_THREADS                                             ;\
       OMP_DYNAMIC=FALSE                                                  ;\
       export OMP_DYNAMIC                                                 ;\
       SUNW_MP_THR_IDLE=spin                                              ;\
       export SUNW_MP_THR_IDLE                                            ;\
       SATS_LAB_HEADER=y                                                  ;\
       export SATS_LAB_HEADER                                             ;\
	   /bin/rm -f perf.dat                                                ;\
	   for S in $(SIZES)                                                  ;\
	   do ./$(EXE5) -m $$S -n $$S -r $(THRESHOLD_ROW) -c $(THRESHOLD_COL) ;\
	      SATS_LAB_HEADER=n                                               ;\
	   done                                                               ;\
	   mv perf.dat ompft_$$P"_procs.dat"                                  ;\
    done

#-----------------------------------------------------------------------
# Hardware Performance Counter Experiments (system specific)
#-----------------------------------------------------------------------
counters: $(CODE).exe
	(OMP_NUM_THREADS=1; export OMP_NUM_THREADS                                                                              ;\
     SATS_LAB_HEADER=y; export SATS_LAB_HEADER                                                                              ;\
     er_rm -f exp1.er exp2.er exp3.er; /bin/rm -f perf.dat                                                                  ;\
     collect -o exp1.er -h cycles,on,insts,on,fpadd,on,fpmul ./$(CODE).exe -m $(PROF_SIZE) -n $(PROF_SIZE) -f $(PROF_REPEAT);\
     SATS_LAB_HEADER=n                                                                                                      ;\
     collect -o exp2.er -h ecd,on,ecdm,on,dc,on,dcm          ./$(CODE).exe -m $(PROF_SIZE) -n $(PROF_SIZE) -f $(PROF_REPEAT);\
     collect -o exp3.er -h memstall,on,dtlbm                 ./$(CODE).exe -m $(PROF_SIZE) -n $(PROF_SIZE) -f $(PROF_REPEAT);\
     mv perf.dat perf_counters.dat)

profile_counters:
	analyzer exp1.er exp2.er exp3.er

profile_cntrs_ascii:
	echo limit 10                                       > PROF.CNTRS
	echo sort e.cycles                                 >> PROF.CNTRS
	echo metrics name:e.insts:e.%+cycles:e.%memstall   >> PROF.CNTRS
	echo functions                                     >> PROF.CNTRS
	echo metrics name:e.dc:e.dcm:e.ecd:e.ecdm::e.dtlbm >> PROF.CNTRS
	echo functions                                     >> PROF.CNTRS
	echo metrics name:e.%fpmul:e.%fpadd                >> PROF.CNTRS
	echo functions                                     >> PROF.CNTRS
	er_print -script PROF.CNTRS exp1.er exp2.er exp3.er

#-----------------------------------------------------------------------
# Dependencies for executables and objects
#-----------------------------------------------------------------------
$(EXE0): $(OBJ_SHRD) $(OBJ_BASE)
	$(CC) -o $(EXE0) $(ISA) $(OBJ_SHRD) $(OBJ_BASE) $(LIBS) $(MOMP)
$(EXE1): $(OBJ_SHRD) $(OBJ_SERIAL)
	$(CC) -o $(EXE1) $(LDFLAGS) $(OBJ_SHRD) $(OBJ_SERIAL) $(LIBS) $(MOMP)
$(EXE2): $(OBJ_SHRD) $(OBJ_PAR)
	$(CC) -o $(EXE2) $(LDFLAGS) $(PAR) $(OBJ_SHRD) $(OBJ_PAR) $(LIBS) $(MOMP)
$(EXE3): $(OBJ_SHRD) $(OBJ_OMP)
	$(CC) -o $(EXE3) $(LDFLAGS) $(OMP) $(OBJ_SHRD) $(OBJ_OMP) $(LIBS)
$(EXE4): $(OBJ_SHRD) $(OBJ_PARFT)
	$(CC) -o $(EXE4) $(LDFLAGS) $(PAR) $(OBJ_SHRD) $(OBJ_PARFT) $(LIBS) $(MEMALLOC)
	ldd $(EXE4)
$(EXE5): $(OBJ_SHRD) $(OBJ_OMPFT)
	$(CC) -o $(EXE5) $(LDFLAGS) $(OMP) $(OBJ_SHRD) $(OBJ_OMPFT) $(LIBS) $(MEMALLOC)
	ldd $(EXE5)

mxv_row_base.o: mxv_row.c
	$(CC) -c -o mxv_row_base.o $(OPT_BASE) $(DEBUG) mxv_row.c
mxv_row.o: mxv_row.c
	$(CC) -c $(CFLAGS) $(DEBUG) $*.c
mxv_row_par.o: mxv_row.c
	$(CC) -c -o mxv_row_par.o $(CFLAGS) $(DEBUG) $(PAR) mxv_row.c
mxv_row_omp.o: mxv_row_omp.c
	$(CC) -c $(CFLAGS) $(DEBUG) $(OMP) mxv_row_omp.c

mxv_col_base.o: mxv_col.c
	$(CC) -c -o mxv_col_base.o $(OPT_BASE) $(DEBUG) mxv_col.c
mxv_col.o: mxv_col.c
	$(CC) -c $(CFLAGS) $(DEBUG) $*.c
mxv_col_par.o: mxv_col.c
	$(CC) -c -o mxv_col_par.o $(CFLAGS) $(DEBUG) $(PAR) mxv_col.c
mxv_col_omp.o: mxv_col_omp.c
	$(CC) -c $(CFLAGS) $(DEBUG) $(OMP) mxv_col_omp.c

mxv_lib_base.o: mxv_lib.c
	$(CC) -c -o mxv_lib_base.o $(OPT_BASE) $(DEBUG) mxv_lib.c
mxv_lib.o: mxv_lib.c
	$(CC) -c $(CFLAGS) $(DEBUG) $*.c

init_data.o: init_data.c
	$(CC) -c $(OPT_BASE) $(DEBUG) $*.c
init_data_omp.o: init_data_omp.c
	$(CC) -c $(CFLAGS) $(DEBUG) $(OMP) init_data_omp.c
check_results.o: check_results.c
	$(CC) -c $(OPT_BASE) $(DEBUG) $*.c
main.o: main.c
	$(CC) -c $(CFLAGS) $(OPT_BASE) $(DEBUG) $(OMP) $*.c
        
init_data.o              : labs.h
init_data_omp.o          : labs.h
main.o                   : labs.h interface.h
mxv_row_base.o           : labs.h
mxv_row.o                : labs.h
mxv_row_par.o            : labs.h
mxv_row_omp.o            : labs.h
mxv_col_base.o           : labs.h
mxv_col.o                : labs.h
mxv_col_par.o            : labs.h
mxv_col_omp.o            : labs.h

.c.o:
	$(CC) -c $(CFLAGS) $(DEBUG) $*.c

#-----------------------------------------------------------------------
# Commands to clean up
#-----------------------------------------------------------------------
clean:
	/bin/rm -f $(OBJ_SHRD) $(OBJ_BASE) $(OBJ_SERIAL) $(OBJ_PAR) $(OBJ_OMP) $(OBJ_PARFT) $(OBJ_OMPFT)\
	            $(EXE0) $(EXE1) $(EXE2) $(EXE3) $(EXE4) $(EXE5)

veryclean:
	make clean
	/bin/rm -f PROF.CNTRS *.dat *.list
	er_rm -f *.er
 
