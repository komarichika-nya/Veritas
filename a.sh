rm -rf build

cmake -S . -B build \
  -DCMAKE_CUDA_HOST_COMPILER=/usr/bin/g++-15 \
  -DCMAKE_C_COMPILER=/usr/bin/gcc-15 \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-15

cmake --build build -j

# veritas --help
# veritas --version
# veritas build
# veritas -gpu build
