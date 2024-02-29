#!/bin/bash
device=$(($SLURM_LOCALID%8))

export ROCR_VISIBLE_DEVICES=$device

if  [[ $SLURM_LOCALID == 0 ]]; then
  last_before=`dmesg -T | tail -n 1`
  echo $last_before
  last_time=`echo $last_before | awk '{print $2,$3,$4,$5}' | tr -d ']'`
  starttime=`date --date="${last_time}" +"%s"`
echo $starttime
  fi


#./a.out 1024
#./a.out 2048
#./a.out 4096
#./a.out 8192
./a.out 32768
#./a.out 131072
if  [[ $SLURM_LOCALID == 0 ]]; then
dmesg -T | tail -n 500 | while read line; do 
#    dmesgstamp=$(echo "$line" | awk -F'[' '{print $2}' | awk -F\. '{print $1}')
#    echo $dmesgstamp
    logdate=`echo $line | awk '{print $2,$3,$4,$5}' | tr -d ']'`
    logtime=`date --date="${logdate}" +"%s"`
#    dmesgstamp=$((dmesgstamp += bootime))
    results=$((logtime-starttime))
    if ((results > 0)); then
        echo "$line"
    fi
done
  fi 
