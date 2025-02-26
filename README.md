# Perf stuff

## Dependencies

- [`nasm`](https://www.nasm.us/)

## Run build

```bash
cmake --workflow --preset gcc-release
```

## Page fault test

```bash
cmake --workflow --preset gcc-release && ./build/gcc-release/hardware_tests/memory/perf_stuff.page_faults
```
