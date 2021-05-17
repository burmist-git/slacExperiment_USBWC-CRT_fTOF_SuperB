# collectionEff = 014 
#  (primPhi>-100.0 && primPhi<100.0)
#  (primTheta<175.0)

#make clean;
#rm ../anaConst.hh
#sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.14/g' ../anaConst.hh.BackUp > ../anaConst.hh1
#sed 's/static const Double_t phiMin = -100.0/static const Double_t phiMin = -20.0/g' ../anaConst.hh1 > ../anaConst.hh2
#sed 's/static const Double_t phiMax = 100.0/static const Double_t phiMax = 20.0/g' ../anaConst.hh2 > ../anaConst.hh3
#sed 's/static const Double_t thetaMax = 175.0/static const Double_t thetaMax = 175.0/g' ../anaConst.hh3 > ../anaConst.hh
#rm ../anaConst.hh1 ../anaConst.hh2 ../anaConst.hh3
#make;
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log


# no cuts
# collectionEff = 014 
#  (primPhi>-180.0 && primPhi<180.0)
#  (primTheta<180.0)

#make clean;
#rm ../anaConst.hh
#sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.14/g' ../anaConst.hh.BackUp > ../anaConst.hh1
#sed 's/static const Double_t phiMin = -180.0/static const Double_t phiMin = -20.0/g' ../anaConst.hh1 > ../anaConst.hh2
#sed 's/static const Double_t phiMax = 180.0/static const Double_t phiMax = 20.0/g' ../anaConst.hh2 > ../anaConst.hh3
#sed 's/static const Double_t thetaMax = 175.0/static const Double_t thetaMax = 175.0/g' ../anaConst.hh3 > ../anaConst.hh
#rm ../anaConst.hh1 ../anaConst.hh2 ../anaConst.hh3
#make;
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log


make clean;
rm ../anaConst.hh
sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.14/g' ../anaConst.hh.BackUp > ../anaConst.hh1
sed 's/static const Double_t phiMin = -100.0/static const Double_t phiMin = 0.0/g' ../anaConst.hh1 > ../anaConst.hh2
sed 's/static const Double_t phiMax = 100.0/static const Double_t phiMax = 0.0/g' ../anaConst.hh2 > ../anaConst.hh3
sed 's/static const Double_t thetaMax = 175.0/static const Double_t thetaMax = 180.0/g' ../anaConst.hh3 > ../anaConst.hh
rm ../anaConst.hh1 ../anaConst.hh2 ../anaConst.hh3
make;
./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/4/014/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/4/014/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 235345 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/4/014/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 456546 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/4/014/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 583344 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/4/014/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 901129 | tee ../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log

#make clean;
#rm ../anaConst.hh
#sed 's/static const double collectionEff = 0.03/static const double collectionEff = 0.14/g' ../anaConst.hh.BackUp > ../anaConst.hh1
#sed 's/static const Double_t phiMin = -100.0/static const Double_t phiMin = -140.0/g' ../anaConst.hh1 > ../anaConst.hh2
#sed 's/static const Double_t phiMax = 100.0/static const Double_t phiMax = 140.0/g' ../anaConst.hh2 > ../anaConst.hh3
#sed 's/static const Double_t thetaMax = 175.0/static const Double_t thetaMax = 165.0/g' ../anaConst.hh3 > ../anaConst.hh
#rm ../anaConst.hh1 ../anaConst.hh2 ../anaConst.hh3
#make;
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root 123123 | tee ../../HistosSim/3/014/phiMinm20phiMax20ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_01.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_02.root 545634 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_02.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_03.root 566600 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_03.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_04.root 677996 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_04.log
#./runApp 1 ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_05.root 393930 | tee ../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_05.log

