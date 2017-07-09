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

sample summary output q.csv, q256.csv, q512.csv and q1024.csv
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

pgu@pgu-Inspiron-N5040:~/geeks/QB/fileio/build/src$ ./serialize q256.csv 
File size is :329094144
Parse (reading from csv and writing to binary file) time duration(in microsecs): 65689057
ZFM3.dat
UBM3.dat
ZNM3.dat
ZBM3.dat
ZTM3.dat
Binary file size is :39629568
Binary file size is :49262592
Binary file size is :67911936
Binary file size is :69028352
Binary file size is :5355776
Binary file reading and deserialization time duration(in microsecs): 551955
Sort duration time duration(in microsecs): 5397293
Number of messages wrote in copy_q256.csv: 6449920
Sort and Write to csv time duration(in microsecs): 85986137
Total time duration(in microsecs): 152306189

pgu@pgu-Inspiron-N5040:~/geeks/QB/fileio/build/src$ ./serialize q512.csv 
File size is :658188288
Parse (reading from csv and writing to binary file) time duration(in microsecs): 133463082
ZFM3.dat
UBM3.dat
ZNM3.dat
ZBM3.dat
ZTM3.dat
Binary file size is :79259136
Binary file size is :98525184
Binary file size is :135823872
Binary file size is :138056704
Binary file size is :10711552
Binary file reading and deserialization time duration(in microsecs): 1144259
Sort duration time duration(in microsecs): 12008318
Number of messages wrote in copy_q512.csv: 12899840
Sort and Write to csv time duration(in microsecs): 170997175
Total time duration(in microsecs): 305764975


pgu@pgu-Inspiron-N5040:~/geeks/QB/fileio/build/src$ ./serialize q1024.csv 
File size is :1316376576
Parse (reading from csv and writing to binary file) time duration(in microsecs): 281948879
ZFM3.dat
UBM3.dat
ZNM3.dat
ZBM3.dat
ZTM3.dat
Binary file size is :158518272
Binary file size is :197050368
Binary file size is :271647744
Binary file size is :276113408
Binary file size is :21423104
Binary file reading and deserialization time duration(in microsecs): 2399213
Sort duration time duration(in microsecs): 27587168
Number of messages wrote in copy_q1024.csv: 25799680
Sort and Write to csv time duration(in microsecs): 373364361
Total time duration(in microsecs): 657798002
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


This files are present in log folder.

