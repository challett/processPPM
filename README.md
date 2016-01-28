# processPPM

C program made with MPI to apply average and mean filtering to PPM images with any size window on any number of processses

Compile with `make`

Run as: `mpirun -np <processes> ppmf <input ppm> <output ppm> <frame size> <Processing type (A=average, M=median)>`
