
index=0

while [ 1 ]
do
    xwd -silent -root -out $index.xwd
    index=`expr $index + 1`
done
