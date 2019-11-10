all: run clean

run: mpi openmp sequential
	@echo "\n\nGRAYSCALE WITH MPI"
	@mpirun -np 4 mpi_grayscale
	@echo "\nGRAYSCALE WITH OPENMP"
	@./penmp
	@echo "\nGRAYSCALE SEQUENTIAL"
	@./sequential_grayscale
	@echo "\n"

mpi:
	mpic++ MPI.cpp `pkg-config --cflags --libs opencv` -o mpi_grayscale

openmp:
	g++ openMP.cpp `pkg-config --cflags --libs opencv` -o penmp -fopenmp

sequential:
	g++ sequential.cpp `pkg-config --cflags --libs opencv` -o sequential_grayscale

clean:
	@rm mpi_grayscale
	@rm penmp
	@rm sequential_grayscale
