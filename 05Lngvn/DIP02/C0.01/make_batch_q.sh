#!/bin/bash

start=0
end=10

for (( x=$start; x<$end; x++ ))
do
    
    cidx=`printf %03i $x`
    cp q_batch_template.sh q_batch.$cidx.sh
    sed -i "s/c_val/`echo $cidx`/g" ./q_batch.$cidx.sh

    if [ $x -eq 0 ];
    then
        pidx=`echo none`
        dfile=$(echo "\/mnt\/research\/Cheng_Polymer\/CCF_PROJ\/04Deform\/DIP02\/C\$CC\/T\$tt\/deform.dat")
        
        sed -i "s/dfile_val/`echo $dfile`/g" ./q_batch.$cidx.sh
        #echo $res
    else
        pidx=$(printf %03i `echo $x-1 | bc`)
        sed -i "s/dfile_val/`echo  prod.restart2.$pidx`/g" ./q_batch.$cidx.sh
    fi
    
    #echo $cidx $pidx
done

