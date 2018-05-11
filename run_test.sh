#start sh run_prefetch.sh name_of_test
#
# name_of_test:
#          band_witdth
#


set -e

TARGET=test_$1

cd src
g++ $TARGET.cpp -O3 -std=c++11 -I../timeitcpp/include -o $TARGET 
./$TARGET > output_$TARGET.txt
python create_report.py output_$TARGET.txt


