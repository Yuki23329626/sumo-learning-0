#/bin/bash

all_num=19
prefix="log.lteudpMultiPart"

a=$(date +%H%M%S)

for index in `seq 0 ${all_num}`
do
{
    cat ${prefix}${index} >> log.total
    echo ${index}
}
done

wait

b=$(date +%H%M%S)

echo -e "startTime:\t$a"
echo -e "endTime:\t$b"
