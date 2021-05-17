# collectionEff = 003
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.03/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 103873 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_06.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 848403 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_07.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 370659 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_08.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 186037 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_09.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_10.root 344554 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_10.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_11.root 234622 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_11.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_12.root 222332 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_12.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_13.root 334553 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_13.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_14.root 555363 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_14.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_15.root 666733 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_15.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_16.root 777777 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_16.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_17.root 888653 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_17.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_18.root 996543 | tee ../../HistosSim/3/003/Histos_fTOFslacCRT_QUARTZ_fTOF_18.log

# collectionEff = 004
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.04/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 103873 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_06.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 848403 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_07.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 370659 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_08.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 186037 | tee ../../HistosSim/3/004/Histos_fTOFslacCRT_QUARTZ_fTOF_09.log

# collectionEff = 0045
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.045/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 103873 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_06.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 848403 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_07.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 370659 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_08.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 186037 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_09.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_10.root 555555 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_10.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_11.root 333333 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_11.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_12.root 444444 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_12.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_13.root 222222 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_13.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_14.root 111111 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_14.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_15.root 434336 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_15.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_15.root 773488 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_16.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_17.root 999999 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_17.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_18.root 566322 | tee ../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF_18.log

# collectionEff = 005
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.05/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 103873 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_06.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 848403 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_07.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 370659 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_08.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 186037 | tee ../../HistosSim/3/005/Histos_fTOFslacCRT_QUARTZ_fTOF_09.log

# collectionEff = 007
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.07/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 103873 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_06.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 848403 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_07.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 370659 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_08.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 186037 | tee ../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF_09.log

# collectionEff = 014
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.14/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log

# collectionEff = 042
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.42/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log

# collectionEff = 07
make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.7/g' ../anaConst.hh.BackUp > ../anaConst.hh
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log


#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 345345
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 456467
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 676764
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 897897
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_06.root 232342
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_07.root 567567
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_08.root 909876
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_09.root 789797
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_10.root 145671
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_11.root 123123345
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_12.root 345345655
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_13.root 456467455
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_14.root 676764522
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_15.root 897897566
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_16.root 232342123
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_17.root 567567997
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_18.root 909876654
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_19.root 789797232
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_20.root 145671344
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_21.root 1231231231
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_22.root 3453451235
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_23.root 4564676753
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_24.root 6767645674
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_25.root 8978972466
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_26.root 2323426453
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_27.root 5675675632
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_28.root 9098765533
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_29.root 7897971242
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/Histos_fTOFslacCRT_QUARTZ_fTOF_30.root 1456713424