echo "Testing the times of each implementation"

echo "=================================="
echo "Python implementation:"
echo "=================================="
cd python_version; time python3 q_learning.py; cd ..
echo "\n"

echo "=================================="
echo "C implementation:"
echo "=================================="
cd c_version; make; time ./run; cd ..
echo "\n"

echo "=================================="
echo "Java implementation:"
echo "=================================="
cd java_version; make testing; time java Q_Learning; cd ..
echo "\n"

echo "=================================="
echo "C++ implementation:"
echo "=================================="
cd cpp_version; make; time ./run; cd ..
echo "\n"