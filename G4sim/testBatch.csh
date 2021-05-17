#!/usr/local/bin/csh

setenv ROOTSYS /usr/local/root/pro/root
root_env
geant4_env

mv /sps/superb/narnaud/FTOF/2DMaps/Log/testBatch.log /sps/superb/narnaud/FTOF/2DMaps/Log/testBatch.log.old
qsub -q T -eo -o /sps/superb/narnaud/FTOF/2DMaps/Log/testBatch.log -V -l platform=LINUX,u_sps_superb -N testBatch testScript
