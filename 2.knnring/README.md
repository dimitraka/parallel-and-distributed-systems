======================================================

		Dimitra  Karatza   8828

======================================================

1) Open a Terminal

2) Create a folder to work in

			> mkdir newFolder

3) Download code.tar.gz and files tester_helper.h, tester.c and tester_mpi.c. to the folder newFolder

4) Change directory to newFolder

			> cd newFolder/

5) Untar the the file code.tar.gz

			> tar -xvzf code.tar.gz

6) Move file "TesterMakefile" to the Desktop and rename it as "Makefile".

			> mv knnring/TesterMakefile ./Makefile

7) Type one of the following:	

			> make test_sequential
			> make test_synchronous
			> make test_asynchronous
