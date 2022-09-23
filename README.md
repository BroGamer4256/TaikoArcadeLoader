# Taiko Arcade Loader

This is a loader for Taiko no Tatsujin Nijiiro ver 08.18

## How to build

### For Ubuntu

- install prerequisites

```
sudo apt install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 build-essential clang
```

- checkout code
```
git clone --recurse-submodules -j8 git@github.com:BroGamer4256/TaikoArcadeLoader.git
```

- build
```
cd TaikoArcadeLoader

make clean

make
```

- distribution
```
make dist
```