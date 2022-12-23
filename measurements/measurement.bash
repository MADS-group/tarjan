#!/bin/bash
TIME_STAMP=$(date +%s)
NMEASURES=5

#ARRAY_RC=( disconnected connected random10000 random50000 random100000 random500000 random1000000 random10000)
ARRAY_RC=(random-1000 random-50000)

ARRAY_THS=(2 4 8)
#TIMEFORMAT='%3U;%3E;%3S;%P'
ARRAY_OPT=(0)

trap "exit" INT

# $1: nome del programma

#SCRIPTPATH=$( cd -- "$(dirname "$0")"  >/dev/null 2>&1 ; pwd -P )
SCRIPTPATH=$( cd -- "$(dirname "$0")" ; pwd -P )
echo $(( $1 == "sequential" ))
if [[ $1 == "sequential" ]]; then
	for input_graph in "${ARRAY_RC[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
			ths_str=$(printf "%02d" $ths)
			
			OUT_FILE=$SCRIPTPATH/measure/$1/graph_type-$input_graph-O$opt/$1-graph_type-$input_graph-O$opt.csv
		
			mkdir -p $(dirname $OUT_FILE) #Se non esiste la cartella di OUTFILE viene creata
			
			echo $(basename $OUT_FILE)
			echo "verteces,init,destroy,tarjan,user,elapsed,system,pCPU" >$OUT_FILE
			
			for ((i = 0 ; i < $NMEASURES; i++)); do
				{ /usr/bin/time -f "%U,%S,%E,%P" ../bin/$1_O$opt.out ../data/$input_graph.bin ../data/$1_output_$input_graph.bin; } 2>&1 | sed -e 's/%/;/g' -e 's/0://g' >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
			done
			printf "\n"
		done
	done
else
	for input_graph in "${ARRAY_RC[@]}"; do
		for ths in "${ARRAY_THS[@]}"; do
			for opt in "${ARRAY_OPT[@]}"; do
				ths_str=$(printf "%02d" $ths)
				
				OUT_FILE=$SCRIPTPATH/measure/$1/graph_type-$input_graph-O$opt/$1-graph_type-$input_graph-O$opt-NTH-$ths_str.csv

				mkdir -p $(dirname $OUT_FILE) #Se non esiste la cartella di OUTFILE viene creata
				
				echo $(basename $OUT_FILE)
				echo "Nvert,init,tarjan,split,merge,user,elapsed,system,pCPU" >$OUT_FILE
				
				for ((i = 0 ; i < $NMEASURES; i++)); do
						
					if [ $1 == "mpi" ]; then
						#2>/dev/null;
						{ /usr/bin/time -f "%U,%S,%E,%P" mpiexec -np $ths ../bin/$1_O$opt.out ../data/$input_graph.bin ../data/$1_output_$input_graph.bin; } 2>&1 | sed -e 's/%/;/g' -e 's/0://g' >> $OUT_FILE
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					else
						printf "Not implemented"
					fi
				done
				printf "\n"
			done
		done
	done
fi