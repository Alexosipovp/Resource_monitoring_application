rm -rf build
cmake -S . -B build
cmake --build build

export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
export LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0
export QT_QPA_PLATFORM=xcb

./build/bin/Resource_monitoring_application
