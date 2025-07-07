#!/bin/bash --login

#please check makebatchq.sh and q_batch_template.sh

st=0
end=10

for (( x=$st ; x<$end ; x++ ))
do
    cidx=`printf %03i $x`
    echo "Would submit: q_batch.$cidx.sh"

    if [ $x == $st ] ; then
        previd=`sbatch q_batch.$cidx.sh | awk '{print $4}'`
    else
        #previd=`sbatch --dependency=afterok:$previd q_batch.$cidx.sh  | awk '{print $4}'`
        previd=`sbatch --dependency=afterany:$previd q_batch.$cidx.sh | awk '{print $4}'`

    fi


done
