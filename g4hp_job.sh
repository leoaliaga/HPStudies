#!/bin/bash

echo
echo "======== cd to CONDOR_DIR_INPUT ========"
cd $CONDOR_DIR_INPUT
pwd

echo
echo "======== ls ========"
ls

echo
echo "======== UNTARRING... ========"
tar xvfz local_install.tar.gz -C ./ > /dev/null

echo
echo "======== Done untarring. ls ========"
ls

echo
echo "======== SETUP ROOT, BOOST and DK2NU ========"
echo "source setup_g4hp_cvmfs.sh"
source setup.sh

echo "PHYSICS=${PHYSICS}"
echo "PARTICLE=${PARTICLE}"
echo "ENERGY=${ENERGY}"
echo "TARGET=${TARGET}"
echo "NEVTS=${NEVTS}"
echo "NUMRUN=${NUMRUN}"
echo "TUPLEFILE=${TUPLEFILE}"
echo "PROCESS=$PROCESS"

dirOut="tmp_griddir${RANDOM}"

#making the run number: 
THISPROCESS=${PROCESS}
run=$(( 1000000 * ${NUMRUN}  + THISPROCESS))

echo "./g4hp -k ${dirOut} -t ${TARGET} -p ${PARTICLE} -n ${NEVTS} -f ${TUPLEFILE} -x ${run} -e ${ENERGY} -l ${PHYSICS} -y"
./g4hp -k ${dirOut} -t ${TARGET} -p ${PARTICLE} -n ${NEVTS} -f ${TUPLEFILE} -x ${run} -e ${ENERGY} -l ${PHYSICS} -y

echo "Current content of the outdir:"
ls ${dirOut}/

echo "Current content of the dir:"
ls
echo
echo "Moving output to CONDOR_DIR_G4HP: "
echo "=> CONDOR_DIR_G4HP: $CONDOR_DIR_G4HP"
mv ${dirOut}/*.root $CONDOR_DIR_G4HP
