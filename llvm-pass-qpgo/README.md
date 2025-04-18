# llvm-pass-qpgo

Two types of probes are implemented for the Quantum Profile-Guided Optimization

1. Counter-based: increase counter for each meta function
2. Context-based: dump arguments and timestamp of function for each meta function

## Requirements

+ QOPS should be cloned into `~/QOPS`.
+ LLVM should be installed at `~/.llvm`.

## Build the QPGO LLVM pass

```bash
$ cd llvm-pass-qpgo
$ export QPGO_HOME=$(pwd) # Optional for convenient path switching
$ mkdir build && cd build
$ cmake -G Ninja .. # with '-DENABLE_DEBUG=ON' can get debug info during compilation with the pass
...
$ ninja # or 'cmake --build .'
...
```

## Usage

+ Commands beginning with `$` are commands entered by the user; otherwise, they are screen output (insignificant information is ignored by `...`)

### Compile the simulator and insert context-based probes

```bash
$ cd $QPGO_HOME
$ cd simplifiedStateVector/src/
$ make -f pgo.mk clean
...
$ make -f pgo.mk 2>|compiler_output.out # default is context-based profiler
make -f makefile CC='~/.llvm/bin/clang' CFLAGS='-O3 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -Qunused-arguments -Xclang -load -Xclang /home/nckucsieserver/pro/selfpro/llvm-pass-qpgo/build/qpgo/libQpgoPass.so -mllvm -profile-gen=default.profdata -mllvm -profile-mode=context'
...
```

### Run the simulator and get the context-based profile data

```bash
$ time make -f pgo.mk run
LD_LIBRARY_PATH=D_LIBRARY_PATH:/home/nckucsieserver/.llvm/lib QPGO_PROFILE_FILE=default.profdata ./qSim
0
3
13
8
2
3
0
3
0
3

real    0m1.306s
user    0m3.568s
sys     0m0.048s
```

```bash
$ cat default.profdata
0 1 0 264093309757175
0 1 3 264093409935986
3 1 2 264093510072925
1 2 1 8 264093810222692
0 1 2 264094010312143
0 1 3 264094110456624
0 1 0 264094210605619
0 1 3 264094310751866
0 1 0 264094410897644
0 1 3 264094511047671
264094611198262
```

### Compile the simulator and insert counter-based probes

```bash
$ cd $QPGO_HOME
$ cd simplifiedStateVector/src/
$ make -f pgo.mk clean
...
$ make -f pgo.mk MODE=counter 2>|compiler_output.out
make -f makefile CC='~/.llvm/bin/clang' CFLAGS='-O3 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -Qunused-arguments -Xclang -load -Xclang /home/nckucsieserver/pro/selfpro/llvm-pass-qpgo/build/qpgo/libQpgoPass.so -mllvm -profile-gen=default.profdata -mllvm -profile-mode=counter'
...
```

### Run the simulator and get the counter-based profile data

```bash
$ time make -f pgo.mk run
...

$ cat default.profdata
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
3 0 1 4 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
...
```

## Custom the profile data filename in runtime

For example: using the datetime as the file name

```bash
$ time make -f pgo.mk run FILE=$(date +%H%M%S%Y%m%d).out
LD_LIBRARY_PATH=D_LIBRARY_PATH:/home/nckucsieserver/.llvm/lib QPGO_PROFILE_FILE=13104020230330.out ./qSim
...
```

## Important Notes

+ Change `simplifiedStateVector` to `~/stateVector` (needs `cp ./simplifiedStateVector/src/pgo.mk ~/stateVector/src`) for a full-featured quantum circuit simulation
+ If you need to use qviz-gui, rename the compiled binaries:
  + `make clean && make 2>|compiler_output.out && mv qSim{,_normal.out}`
  + `make clean && make -f pgo.mk MODE=context 2>|compiler_output.out && mv qSim{,_context.out}`
  + `make clean && make -f pgo.mk MODE=counter 2>|compiler_output.out && mv qSim{,_counter.out}`
