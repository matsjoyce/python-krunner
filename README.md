python-krunner
--------------

Python runner plugin for KF5 krunner

Dependancies
------------

 - Python 3 (tested using 3.5, 3.6)
 - KF5 (tested using 5.8.4, 5.30.0)
 - Qt5 (tested using 5.7.0, 5.8.0)
 - Boost.Python (tested using 1.62, 1.63)
 - Sip + PyQt5 (tested using 4.18.1, 4.19)
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

This will install a shared library to the qt plugins directory and a python module named `krunner`.
