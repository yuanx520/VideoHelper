make clean
make
echo "Memory Test"
valgrind --leak-check=full --show-leak-kinds=all ./test_memory > test.log  2>&1
echo "Memory Infomation"
cat test.log
