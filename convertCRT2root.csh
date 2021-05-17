#!/bin/csh

srtpath newest Linux26SL5_i386_gcc412
#echo "BFCURRENT = $BFCURRENT"
#echo "   BFARCH = $BFARCH"

if ($#argv != 3 && $#argv != 4) then
  echo "Usage: source convertCRT2root.csh <CRT run name> <number of the first file to process> <number of the last file to process>"
  echo "       or"
  echo "Usage: source convertCRT2root.csh <CRT run name> <number of the first file to process> <number of the last file to process> <ROOT output file name>"
  exit
endif

set underscore = "_"

set runName = $1;
echo "runName = $runName"

set first = $2
set last  = $3

echo "Processing files $first -> $last"

set rawDataDir  = "/u3/CRT_DST/exp3"
set rootDataDir = "/nfs/farm/g/superb/u02/crt/crtDataConvertedToRoot"

set currentDir = `pwd`
echo "currentDir = $currentDir"

cd $rawDataDir
if( ! -d $runName ) then
  echo "CRT directory $runName does not exist in area $rawDataDir"
  echo "=> Exiting the script"
  exit
endif

cd $runName
#echo "first = $first last = $last"
set list = `ls -1 *.dst2 | awk -F "." '{ print $1}' | awk -F "_" '{ if( $NF >= '"$first"' && $NF <= '"$last"' ) {print $0".dst2"; } }'`

#echo "LIST"
echo "DST2 files to be processed: $list"
#echo "LIST"

cd $currentDir

if( -e list_CRT.tmp ) then
  rm list_CRT.tmp
endif

foreach file( $list )
  if( ! -e list_CRT.tmp ) then
    echo "$file" >! list_CRT.tmp
  else
    echo "$file" >> list_CRT.tmp
  endif
end

# Remove by hand a bad file with data not ordered by increasing time:
# file 00166 in run16
if( $runName == "run16" && $first <= 166 && 166 <= $last ) then
#if( $runName == "run16" ) then
  echo "Remove a bad file"
  ( cat list_CRT.tmp | grep -v exp3_run16_00166.dst2 ) >! tmp.txt
  mv list_CRT.tmp list_CRT.tmp.old
  mv tmp.txt list_CRT.tmp
  echo "Diffing the two files"
  diff list_CRT.tmp list_CRT.tmp.old
endif

if( $first < 10 ) then
  set first = "0000$first"
  else if( $first < 100 ) then
    set first = "000$first"
    else if( $first < 1000 ) then
      set first = "00$first"
      else if( $first < 10000 ) then
        set first = "0$first"
      endif
    endif
  endif
endif

if( $last < 10 ) then
  set last = "0000$last"
  else if( $last < 100 ) then
    set last = "000$last"
    else if( $last < 1000 ) then
      set last = "00$last"
      else if( $last < 10000 ) then
        set last = "0$last"
      endif
    endif
  endif
endif

if ($#argv == 4 ) then
  set rootFileName = $4
else
  set rootFileName = "CRT_$runName$underscore$first-$last.root"
endif

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

if( -e convertCRT2root.log ) then
  mv convertCRT2root.log convertCRT2root.log.old
endif

#echo "ROOTSYS = $ROOTSYS"
#gmake clean
#gmake
convertCRT2rootApp list_CRT.tmp $rawDataDir/$runName $rootDataDir/$runName/$rootFileName |& tee convertCRT2root.log

echo
echo "---> Newly created root file: $rootDataDir/$runName/$rootFileName"
echo
