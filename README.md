python-krunner
--------------

Python runner plugin for KF5 krunner

Dependancies
------------

 - Python 3 (tested using 3.5)
 - KF5 (tested using 5.8.4)
 - Qt5 (tested using 5.7.0)
 - Boost.Python (tested using 1.62)
 - Sip + PyQt5 (tested using 4.18.1)
 - CMake

Build and install
-----------------

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```

This will install a shared libary to the qt plugins directory and a python module named `krunner`.
