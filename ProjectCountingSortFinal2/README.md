# ContestOMP

## Dependencies

* CMake 3.9+
* OpenMP
* Python3
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake ..
   ```

2. Generate executables with `make`
3. To generate measures (TAKE A LOT OF TIME! Our measures are already included so you should skip this step) run `make genera_output`
4. To extract mean times and speedup curves from them run `make extract_measures`

Results can be found in the `/measure` directory, divided by datetime folder, problem size and the gcc optimization option used.

In measureReport folder, you can see all the measure descripted in the report for Case Study 2


