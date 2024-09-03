# qe_band_plotter
software for plotting bands data generated by the command bands.x of quantum espresso using ROOT

Command to compile: g++ -o qe_band_plotter qe_band_plotter.cpp $(root-config --cflags --glibs)

Command to run: ./qe_band_plotter

An executable file is already present in the directory. As an example, in the directory there is also an example of how the graphs look like.
There are 40 band plots and the relative input file, created by simply copying the band data from the quantum espresso output file generated by a bands.x calculation (post relax, scf, nscf).
The system is a Carbon nanotube armchair unit cell.