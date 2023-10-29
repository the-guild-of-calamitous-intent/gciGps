# gciGps

Add this to your `CMakeLists.txt` file, since this currently uses a 
try/catch in a couple places.

```cmake
set(PICO_CXX_ENABLE_EXCEPTIONS 1) # for gciGPS
```

## ToDo

- [ ] Investigate removing try/catch so you don't need to enable exceptions 
