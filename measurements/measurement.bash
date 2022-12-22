#!/bin/bash
TIME_STAMP=$(date +%s)
NMEASURES=5

#ARRAY_RC=( disconnected connected random10000 random50000 random100000 random500000 random1000000 random10000)
ARRAY_RC=(random500000)

ARRAY_THS=(4 8 16)
#TIMEFORMAT='%3U;%3E;%3S;%P'
ARRAY_OPT=(0 1 2 3)

trap "exit" INT

# $1: nome del programma

#SCRIPTPATH=$( cd -- "$(dirname "$0")"  >/dev/null 2>&1 ; pwd -P )
SCRIPTPATH=$( cd -- "$(dirname "$0")" ; pwd -P )
echo $(( $1 == "sequential" ))
if [[ $1 == "sequential" ]]; then
	for size in "${ARRAY_RC[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
			ths_str=$(printf "%02d" $ths)
			
			if [[ $opt -eq 0 ]]; then
				OUT_FILE=$SCRIPTPATH/measure/SIZE-$size/SIZE-$size-NTH-$ths_str-O$opt.csv
			else
				OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt/SIZE-$size-NTH-$ths_str-O$opt.csv
			fi
		
			mkdir -p $(dirname $OUT_FILE) #Se non esiste la cartella di OUTFILE viene creata
			
			echo $(basename $OUT_FILE)
			echo "row,columns,threads,init,dotprod,user,elapsed,sys,pCPU" >$OUT_FILE
			
			for ((i = 0 ; i < $NMEASURES; i++)); do
				echo "NOOOOO"
				{ /usr/bin/time -f "%U,%S,%E,%P" ../bin/$1_O$opt.out ../data/$size.bin ../data/$1_output_$size.bin; } 2>&1 | sed -e 's/%/;/g' -e 's/0://g' >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
			done
			printf "\n"
		done
	done
else
	for size in "${ARRAY_RC[@]}"; do
		for ths in "${ARRAY_THS[@]}"; do
			for opt in "${ARRAY_OPT[@]}"; do
				ths_str=$(printf "%02d" $ths)
				
				if [[ $opt -eq 0 ]]; then
					OUT_FILE=$SCRIPTPATH/measure/SIZE-$size/SIZE-$size-NTH-$ths_str-O$opt.csv
				else
					OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt/SIZE-$size-NTH-$ths_str-O$opt.csv
				fi
			
				mkdir -p $(dirname $OUT_FILE) #Se non esiste la cartella di OUTFILE viene creata
				
				echo $(basename $OUT_FILE)
				echo "row,columns,threads,init,dotprod,user,elapsed,sys,pCPU" >$OUT_FILE
				
				for ((i = 0 ; i < $NMEASURES; i++)); do

					if [ $1 == "mpi" ]; then
						{ /usr/bin/time -f "%U,%S,%E,%P" { mpiexec -np $ths ../bin/$1_O$opt.out ../data/$size.bin ../data/$1_output_$size.bin 2>/dev/null; } } 2>&1 | sed -e 's/%/;/g' -e 's/0://g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					else
						printf "Not implemented"
						# { /usr/bin/time -f "%U,%S,%E,%P" ../bin/$1_O$opt ../data/$size.bin data/$size.bin; } 2>&1 | sed -e 's/%/;/g' -e 's/0://g' >> $OUT_FILE
						#printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						#printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					fi
				done
				printf "\n"
			done
		done
	done
fi