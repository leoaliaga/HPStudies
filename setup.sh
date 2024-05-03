#!/bin/sh

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups
source /cvmfs/larsoft.opensciencegrid.org/products/setup
setup cmake v3_14_3
setup geant4 v4_10_3_p03b -q e15:prof
setup fftw v3_3_6_pl2 -q prof
setup python v2_7_14
setup dk2nu v01_05_01b -q e15:prof
unset G4BIN

SETUPDIR=${PWD}
export G4WORKDIR=${SETUPDIR}
export LD_LIBRARY_PATH=${G4WORKDIR}/build:${G4WORKDIR}/ana:${LD_LIBRARY_PATH}

#change this to your experiment
export EXPERIMENT=sbnd

setup cigetcert                                    
kx509
setup fife_utils
#voms-proxy-init -noregen -rfc -voms 'fermilab:/fermilab/sbnd/Role=Analysis'
setup jobsub_client

#cmake .
#make all


