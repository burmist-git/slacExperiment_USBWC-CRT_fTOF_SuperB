#!/bin/csh

srtpath newest Linux26SL5_i386_gcc412
#echo "BFCURRENT = $BFCURRENT"
#echo "   BFARCH = $BFARCH"

if ($#argv != 1 && $#argv != 2 && $#argv != 3 ) then
  echo "Usage: source convertUSBWC2root.csh <USBWC run name>"
  echo "       or"
  echo "       source convertUSBWC2root.csh <USBWC run name> <ROOT output file name"
  echo "       or"
  echo "       source convertUSBWC2root.csh <USBWC run name> <index of the first file to convert> <index of the last file to convert>"
  exit
endif

set underscore = "_"

set runName = $1
echo "runName = $runName"

if ($#argv == 2 ) then
  set rootFileName = $2
else
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

    set rootFileName = "USBWC$underscore$runName-$firstString$underscore$lastString.root"
    else  
      set rootFileName = "USBWC$underscore$runName.root"
    endif
  endif
endif

echo "rootFileName = $rootFileName"

set rawDataDir  = "/nfs/farm/g/superb/u02/crt/usbWaveCatcher/RawData"
set rootDataDir = "/nfs/farm/g/superb/u02/crt/usbWaveCatcher/RootData"

set currentDir = `pwd`
echo "currentDir = $currentDir"

cd $rawDataDir
if( ! -d $runName ) then
  echo "USBWC directory $runName does not exist in area $rawDataDir"
  echo "=> Exiting the script"
  exit
endif

cd $runName

if( -e $runName.dat ) then
  mv $runName.dat $runName.dat_0000
  chmod g+w $runName.dat_0000
endif
#set list1 = `ls *.dat`

if($#argv == 3 ) then
  set list2 = `ls -1 *.dat_* | awk -F "_" '{ print $NF " " $0}' | sort -n | awk '{ if( $1 >= '"$first"' && $1 <= '"$last"' ) { print $2 "\n"; } }'`
else
  set list2 = `ls -1 *.dat_* | awk -F "_" '{ print $NF " " $0}' | sort -n | awk '{ print $2 "\n" }'`
endif

cd $currentDir

#echo $list1 >! list.tmp
rm list.tmp
touch list.tmp

foreach file( $list2 )
  echo "$file" >> list.tmp
end

cd $rootDataDir
if( ! -d $runName ) then
  mkdir $runName
  chmod g+rwx $runName
endif

cd $runName
if( -e $rootFileName ) then
  mv $rootFileName $rootFileName.old
  chmod g+w $rootFileName.old
endif

cd $currentDir

if( -e convertUSBWC2root.log ) then
  mv convertUSBWC2root.log convertUSBWC2root.log.old
endif

#echo "ROOTSYS = $ROOTSYS"
convertUSBWC2rootApp list.tmp $rawDataDir/$runName $rootDataDir/$runName/$rootFileName |& tee convertUSBWC2root.log

echo
echo "---> Newly created root file:"
ls -larth $rootDataDir/$runName/$rootFileName
echo

# Backup the USBWC ASCII data
if ($#argv == 1 ) then
  source backupUSBWCData.csh $runName
endif
if ($#argv == 3 ) then
  source backupUSBWCData.csh $runName $first $last
endif
