make
RAND=$RANDOM
./demo.o inputs/input1.txt outputs/img_tp_$RAND.ppm 1200 675 80
open outputs/img_tp_$RAND.ppm;
