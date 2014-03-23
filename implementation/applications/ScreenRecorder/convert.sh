
files=`find . | grep xwd`
for file in ${files} ; do
    convert -quality 100 $file $file.jpg
    rm $file
done

mencoder "mf://*.jpg" -mf fps=1 -o ./test.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=800
