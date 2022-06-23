#!/bin/bash
# DELETE LINKS TO ALL USED FILES FROM LIB
arr=(${VSCA_SKETCH//\// })
dir=${VSCA_SKETCH:0:${#VSCA_SKETCH} - ${#arr[${#arr[@]} - 1]} - 1}

inc=`grep -R "include \"" ${VSCA_SKETCH}`

link () {
    inc_e=$*
    inc_a=(${inc_e//\#include \"/ })
    for inc_s in ${inc_a[@]}
    do
        inc_t=(${inc_s//./ })
        inc_f=${inc_s:0:${#inc_s} - ${#inc_t[${#inc_t[@]} - 1]} - 1}
        if [[ $inc_f != "lib" ]]
        then
            rm "${dir}/${inc_f}."*

            ret=`grep -R "include \"" "lib/${inc_f}.h"`
            if [[ $ret != "" ]]
            then
                link $ret
            fi
        fi
    done
}

link $inc