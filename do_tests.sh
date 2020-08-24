#!/bin/bash

FZY=fuzzycm

BIN_DIR=bin
RES_DIR=results

TIME=perftime

PAR=parallel
MPI=mpi
SEQ=sequential

ST=static
DY=dynamic
GU=guided

OPT='O0 O2 Ofast xHost'
SCHED="$ST $DY $GU"

N_REP=10
NREP_SCALING=3

function print_title() {
	len=${#1}
	for i in `seq 1 $(( len*3 + 2 ))`; do
		echo -n '*'
	done
	echo ''
	for i in `seq 1 $len`; do
		echo -n '*'
	done
	echo -n " $1 "
	for i in `seq 1 $len`; do
		echo -n '*'
	done
	echo ''
	for i in `seq 1 $(( len*3 + 2 ))`; do
		echo -n '*'
	done
	echo ''
}


############
### MAIN ###
############

#print_title SEQUENTIAL
#
## sequential
#SEQ_BIN="$BIN_DIR/$SEQ/${FZY}_"
#SEQ_OUT="$RES_DIR/$SEQ/${TIME}_"
#for opt in $OPT; do
#	echo -n "Testing: $SEQ_BIN$opt"
#	for i in `seq 1 $N_REP`; do
#		./"$SEQ_BIN$opt" >> "$SEQ_OUT$opt"
#		echo -n '.'
#	done
#	echo ''
#done
#
#print_title PARALLEL
#
## parallel - OpenMP
#PAR_BIN="$BIN_DIR/$PAR/"
#PAR_OUT="$RES_DIR/$PAR/${TIME}_"
#for sched in $SCHED; do
#	for opt in $OPT; do
#		echo -n "Testing: $PAR_BIN/$sched/${FZY}_${sched}_$opt"
#		for i in `seq 1 $N_REP`; do
#			./"$PAR_BIN/$sched/${FZY}_${sched}_$opt" >> "$PAR_OUT${sched}_$opt"
#			echo -n '.'
#		done
#		echo ''
#	done
#done
#
#
#print_title MPI SCALING
#
## Weak VS Strong MPI scaling
#MPIS_BIN="$BIN_DIR/$MPI/${FZY}_${MPI}_scaling"
#MPI_OUT="$RES_DIR/$MPI/${TIME}_scaling_"
#
#
#
print_title MPI
#
# OpenMPI scaling
#SCALING_DIR=common/dataset/scaling
#MPI_BIN_SC="$BIN_DIR/$MPI/${FZY}_${MPI}_scaling"
#MPI_SSC_RES="$RES_DIR/$MPI/strong_scaling"
#MPI_WSC_RES="$RES_DIR/$MPI/weak_scaling"
#
## STRONG
#echo 'Testing STRONG SCALING:'
#for np in `seq 2 20`; do
#	echo -n "Num processors: $np"
#	echo $np >> $MPI_SSC_RES
#	for i in `seq 1 $NREP_SCALING`; do
#		mpirun -n $np -nolocal --hostfile OpenMPI/host_file $MPI_BIN_SC $SCALING_DIR/STRONG.csv >> $MPI_SSC_RES
#		echo -n '.'
#	done
#	echo ''
#done
#echo ''
#
## WEAK
#echo 'Testing WEAK SCALING (1000 points/process):'
#for np in `seq 10 20`; do
#	SIZE=$(( np * 1000 ))
#	echo -ne "Num processors: $np\tDataset size: $SIZE"
#	echo "$np $SIZE" >> $MPI_WSC_RES
#	for i in `seq 1 $NREP_SCALING`; do
#		mpirun -n $np -nolocal --hostfile OpenMPI/host_file $MPI_BIN_SC $SCALING_DIR/WEAK_$SIZE.csv >> $MPI_WSC_RES
#		echo -n '.'
#	done
#	echo ''
#done
#
# MPI (best np)
MPI_BIN="$BIN_DIR/$MPI/${FZY}_${MPI}_"
MPI_OUT="$RES_DIR/$MPI/${TIME}_${MPI}_"
for opt in $OPT; do
	echo -n "Testing: $MPI_BIN$opt"
	for i in `seq 1 $N_REP`; do
		mpirun -n 12 -nolocal --hostfile "OpenMPI/host_file" "$MPI_BIN$opt" >> "$MPI_OUT$opt"
		echo -n '.'
	done
	echo ''
done
