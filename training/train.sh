#!/bin/sh
# [width] [height] [numPos] [numNeg] [numPosSample]

echo 'start'
find ./positive_images -iname "*.jpg" > positives.txt
find ./negative_images -iname "*.jpg" > negatives.txt
echo 'image list created'
perl bin/createsamples.pl positives.txt negatives.txt samples $5\
          "opencv_createsamples -bgcolor 0 -bgthresh 0 -maxxangle 1.1\
          -maxyangle 1.1 maxzangle 0.5 -maxidev 40 -w $1 -h $2"
echo 'sample generated'
find ./samples -name '*.vec' > samples.txt
./mergevec samples.txt samples.vec
echo 'prepare for training...'
opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt\
          -numStages 20 -minHitRate 0.999 -maxFalseAlarmRate 0.5 -numPos $3\
          -numNeg $4 -w $1 -h $2 -mode ALL -precalcValBufSize 1024\
          -precalcIdxBufSize 1024
echo 'done'
