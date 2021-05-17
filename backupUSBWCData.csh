#!/bin/csh

if ($#argv != 1 && $#argv != 3 ) then
  echo "Usage: source backupUSBWCData.csh <USBWC run name>"
  echo "       or"
  echo "       source backupUSBWCData.csh <USBWC run name> <index of the first file to backup> <index of the last file to backup>"
  exit
endif

set currentRun = $1
echo "currentRun = $currentRun"

set underscore = "_"

if($#argv == 3 ) then
  set first = $2
  if( $first < 10 ) then
    set firstString = "000$first"
  else 
    if( $first < 100 ) then
      set firstString = "00$first"
    else 
      if( $first < 1000 ) then
        set firstString = "0$first"
      endif
    endif
  endif

  set last  = $3
  if( $last < 10 ) then
    set lastString = "000$last"
  else 
    if( $last < 100 ) then
      set lastString = "00$last"
    else 
      if( $last < 1000 ) then
        set lastString = "0$last"
      endif
    endif
  endif

  echo "Backuping files with indexes between $firstString and $lastString"

  set archiveName = "$currentRun-$firstString$underscore$lastString.tar"
else
  set archiveName = "$currentRun.tar"
endif

echo "archiveName = $archiveName"

set gzippedArchiveName = "$archiveName.gz"

cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/$currentRun

if( -e $gzippedArchiveName ) then
  mv $gzippedArchiveName $gzippedArchiveName.old
endif

echo "Creating the tar file"

if($#argv == 1 ) then
  tar -cvf $archiveName *.dat*
else
  set list = `ls -1 *.dat_* | awk -F "_" '{ print $NF " " $0}' | sort -n | awk '{ if( $1 >= '"$first"' && $1 <= '"$last"' ) { print $2 " "; } }'`
  #echo "list = *** $list ***"
  tar -cvf $archiveName $list
endif

echo "Compressing the tar file"

gzip $archiveName

#echo "Moving the gzipped tar file to the Backup/ directory"

mv $gzippedArchiveName ../Backup/

cd -

#########################################################################

# ==================================================== #
# Directories below should have been properly archived #
# => Move this comment above previous directory once = #
# corresponding run is finished ====================== #
# ==================================================== #

#set currentRun = "20101102"

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/$currentRun
#if( -e $currentRun.tar.gz ) then
#  mv $currentRun.tar.gz $currentRun.tar.gz.old
#endif
#echo "Creating the tar file"
#tar -cvf $currentRun.tar *.dat*
#echo "Compressing the tar file"
#gzip $currentRun.tar
#echo "Moving the gzipped tar file to the Backup/ directory"
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101101
#if( -e 20101101.tar.gz ) then
#  mv 20101101.tar.gz 20101101.tar.gz.old
#endif
#echo "Creating the tar file"
#tar -cvf 20101101.tar *.dat*
#echo "Compressing the tar file"
#gzip 20101101.tar
#echo "Moving the gzipped tar file to the Backup/ directory"
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101025
#if( -e 20101025.tar.gz ) then
#  mv 20101025.tar.gz 20101025.tar.gz.old
#endif
#echo "Creating the tar file"
#tar -cvf 20101025.tar *.dat*
#echo "Compressing the tar file"
#gzip 20101025.tar
#echo "Moving the gzipped tar file to the Backup/ directory"
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101020
#tar -cvf 20101020.tar *.dat*
#gzip 20101020.tar
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101019
#tar -cvf 20101019.tar *.dat*
#gzip 20101019.tar
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101010
#tar -cvf 20101010.tar *.dat*
#gzip 20101010.tar
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101005_overnight_nicolas
#tar -cvf 20101005_overnight_nicolas.tar *.dat*
#gzip 20101005_overnight_nicolas.tar
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/20101004_overnight_nicolas
#tar -cvf 20101004_overnight_nicolas.tar *.dat*
#gzip 20101004_overnight_nicolas.tar
#mv *.tar.gz ../Backup/
#cd -

#cd /nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData/Run_16_cosmic_Data_9_24_2010
#mv *.gz ../Backup/
#cd -
