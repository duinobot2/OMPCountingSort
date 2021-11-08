#!/bin/bash
#
# Course: High Performance Computing 2021/2022
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Avella Antonello            a.avella19@studenti.unisa.it             
# D'Andrea Anna               a.dandrea26@studenti.unisa.it 
# De Pisapia Claudio          c.depisapia1@studenti.unisa.it
# 
# Purpose: 
# this file file.bash contains the instructions in bash for the automatic execution of the program both with 
# the sequential and parallel version, with different numbers of threads and different levels of optimization
#
# Copyright (C) 2021 - All Rights Reserved
#
# This file is part of ContestOMP.
#
# ContestOMP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ContestOMP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ContestOMP.  If not, see <http://www.gnu.org/licenses/>.
#

YELLOW='\033[0;92m'
NORMAL='\033[0;39m'
BLUE='\033[0;34m'

TIMEFORMAT='%4U;%4E;%4S;%P'
TIMESTAMP=$(date +%F.%T)

NMEASURES=200
ARRAY_SIZE=(5000 100000 10000000) #carico
ARRAY_MAX_NUM=(100000) #cnum max
ARRAY_THS=(0 1 2 4 8 16 32)
ARRAY_OPT=(0 1 2 3)


SCRIPTPATH=$2

for size in "${ARRAY_SIZE[@]}"; do
	for nTh in "${ARRAY_THS[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
            for num in "${ARRAY_MAX_NUM[@]}"; do
                nThStr=$(printf "%02d" $nTh)
                    
                OUT_FILE=$SCRIPTPATH/measure/$TIMESTAMP/SIZE-$size-MAX-$num-O$opt/SIZE-$size-NTH-$nThStr-O$opt.csv

                mkdir -p $(dirname $OUT_FILE)
                
                printf "${NORMAL}"
                echo "MAX= $num FILE= $(basename $OUT_FILE)"
                echo 'nThread;timeInt;user;real;sys;pCPU'>$OUT_FILE 

                for (( nExec = 0 ; nExec < $NMEASURES ; nExec += 1 )) ; do

                    (time $1/eseguibileO$opt $size $num $nTh )2>&1 | sed -e 's/,/./g' -e 's/\n/;/g'  >> $OUT_FILE 
                    printf "\r${YELLOW}> ${BLUE}%5d/%d ${YELLOW}%3.1d%% [ " $(expr $nExec + 1) $NMEASURES $(expr \( \( $nExec + 1  \) \* 100 \) / $NMEASURES)
                    printf "#%.0s" $(seq -s " " 1 $(expr \( $nExec \* 40 \) / $NMEASURES))
                    printf " %.0s" $(seq -s " " 1 $(expr 40 - \( $nExec \* 40 \) / $NMEASURES))
                    printf "] "
                    
                done

                printf "\n"
            done
        done
    done
done



