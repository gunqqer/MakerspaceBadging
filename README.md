# Makerspace Badging system (WIP!)
A system that tracks user training in a Makerspace. If your machines are different you'll have to fork and adapt it yourself.

## Building
Requires [MariaDB >= 10.7](https://mariadb.com/downloads/)

Requires [MariaDB C++ Connector](https://mariadb.com/docs/skysql/connect/programming-languages/cpp/)

Requires [argparse](https://github.com/jamolnng/argparse)

```
git clone https://github.com/gunqqer/MakerspaceBadging.git
cd MakerspaceBadging
mkdir build; cd build;
cmake ..
make
```

#TODO
- [ ] Finish SQLBridge
- [ ] Create user interface
- [ ] Add testing
