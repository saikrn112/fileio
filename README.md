####################################################################################
This project read a csv file and in particular format writes it to several binary files , re-read binary files again to create a duplicate csv file. It also provides summary of run timetaken by different operations.

BUILD
####################################################################################
1. cd fileio
2. mkdir build && cd build
3. cmake ../
4. make all -j

Binary executable serialize will be created in build/src folder.

RUN
####################################################################################
./serialize q.csv

sample summary output
#####################################################################################
pgu@pgu-Inspiron-N5040:~/geeks/QB/build/src$ ./serialize q.csv
File size is :1285524
Parse (reading from csv and writing to binary file) time duration(in microsecs): 64393
ZFM3.dat
UBM3.dat
ZNM3.dat
ZBM3.dat
ZTM3.dat
Binary file size is :360291
Binary file size is :435360
Binary file size is :627585
Binary file size is :637802
Binary file size is :51401
Binary file reading and deserialization time duration(in microsecs): 1894
Sort duration time duration(in microsecs): 24521
Number of messages wrote in copy_q.csv: 25195
Sort and Write to csv time duration(in microsecs): 154437
Total time duration(in microsecs): 221251

This files are present in log folder.

