# Distributed Parallel Computing
The architecture of the solution we created consists of a main.c file that has the capabilities to multiply two given matrices via txt file, as well as the ability to multiply matrices from sizes 1-1000 and automatically output the results to a graph. Along with the normal way of multiplying matrices, we are able to multiply the matrices using SIMD, OMP, and MPI separately. This is done by having each separate way of multiplying the matrices either in their own files or functions to be called by main. The one exception to this is that the MPI algorithm's variation is run in its own main function, in a file called main_mpi.  

We implemented 4 different types of algorithms, normal mmult, vectorized mmult (SIMD), OMP mmult, and MPI mmult. Normal mmult(shown below) is the approach a normal programmer would take to create a matrix multiplication algorithm; it goes through a triple nested for loop that multiplies different values together. It first sets the value in the output matrix that it is computing to zero and then adds up the multiplied values of the first two matrices to create a single value for the output matrix multiple times until the output matrix is completed

	Normal mmult:
	
	for (i = 0; i < aRows; i++){
		for (j = 0; j < bCols; j++) {
			c[i * bCols + j] = 0;

			for (k = 0; k < aRows; k++){
				c[i * bCols + j] += a[i * aRows + k] * b[k * bCols + j];
			}
		}
	}

<p align="center">
<b>MPI</b>
</p>
<p>MPI mmult is implemented by using the Wolfgand cluster provided for us, it uses the different nodes in the cluster as different processes that can each do different actions. The basis of MPI(shown below) is shown by clarifying your master process and your slave processes. We then use the master process to send each slave process a row of data from a matrix to perform matrix multiplication on. Therefore, we can send different rows to different slave processes from the master process and compute faster than if we only had one process. The slaves then send back the computed data to the master process to be combined into one final output matrix.</p>
	
	MPI Basics:
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if (myid == 0){
		printf("I am the master node");
	}
	else{
		printf("I am a slave node");
	}

<p align="center">
<b>OMP</b>
</p>
<p>OMP mmult(shown below) is a method to use multi-threading with a level of abstraction to try and make multi-threading easier. This is done by using the ???#pragma??? directive. In our case, we said ???shared(a, b, c, aRows, aCols, bRows, bCols)??? to let OMP know that we want these variables to be shared between threads. As well as ???private(i, k, j)??? to tell OMP that we want these to be local variables per thread. Along with ???#pragma omp parallel??? to initialize a parallel region, then ???#pragma omp for??? to let OMP know that we want the multi-threading region to be within the for loop. Then it will take care of the mult-threading part for you as it does the mmult algorithm.</p>

	OMP mmult:
	
	#pragma omp parallel default(none) shared(a, b, c, aRows, aCols, bRows, bCols) private(i, k, j)
	#pragma omp for
	for (i = 0; i < aRows; i++) {
		for (j = 0; j < bCols; j++)
			c[i * bCols + j] = 0;
            
		for (k = 0; k < aCols; k++)
			for (j = 0; j < bCols; j++)
				c[i * bCols + j] += a[i * aCols + k] * b[k * bCols + j];
	}

<p align="center">
<b>SIMD</b>
</p>
<p>Vectorized mmult(shown below) using SIMD is implemented by having the concept of locality in mind. This algorithm also uses a triple nested for loop but it first goes through the output matrix and sets each value to zero, then it will multiply values from the other matrices with values that are closer to each other within the physical machine. Since the values are closer together (locality), the machine computes this algorithm faster than if we were to use normal mmult.</p>

	SIMD mmult:
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++){
			c[i * N + j] = 0;
		}
		
		for (k = 0; k < N; k++){
			for (l = 0; l < N; l++){
				c[i * N + l] += a[i * N + k] * b[k * N + l];
			}	
		}
	}

# Teamwork
### Alex
Helped write the ???What is SIMD, OMP, MPI???? discussion question. Helped with running the matrix multiplication without SIMD and recorded the results. Added SIMD capabilities to the mmult.c code and ran the matrix multiplication with SIMD and recorded results. Wrote the answer to the ???Describe what is shown on the graph you have produced??? question by explaining the graph and what the results meant. Updated main to allow user to specify which variation of the mmult algorithm they would like to use. Helped create main_mpi.c which is the implementation of the mmult algorithm using mpi. 

### Ayser
Automated running the matrix multiplication on different size matrices by creating a main.c file that interacts with the mmult.c file and automatically creates the matrices to multiply and records the matrix size along with the time taken in an external text file. Helped with running the matrix multiplication without SIMD and recorded the results. Ran the matrix multiplication with SIMD and recorded results. Produced the graph in excel with all of the results compiled together. Split mmult into vectorized and non vectorized functions. Created mmult_omp.c which is the OMP variation of mmult. Updated main to allow user to specify which variation of the mmult algorithm they would like to use. Helped create main_mpi.c which is the implementation of the mmult algorithm using mpi. Automated the graphing of the data using Gnuplot. 

### Chris
Helped write the ???What is SIMD, OMP, MPI???? discussion question. Helped with running the matrix multiplication without SIMD and recorded the results. Helped with running the matrix multiplication without SIMD and recorded the results. Produced the graph in excel with all of the results compiled together. Updated main to allow user to specify which variation of the mmult algorithm they would like to use. Created and answered all the questions in the design document.

We locked the master branch as shown in HW 2, whenever we had finished a week???s worth of tasks, we would create a pull request to merge the current week???s branch with the master branch. Then we would let the other team members know that a pull request was created and then the other team members would review the pull request then write a review and approve the request. 

# Full Project Life Cycle
Our team used a project board through Trello. We were able to write down the current week???s tasks and tell the other team members what tasks we were in the midst of doing and which tasks we needed to complete next. Most of the planning and managing we went through was mostly just communication between team members rather than using the Trello board though, we were able to talk to each other to let each other know what we were thinking of doing, but the Trello board was able to cement exactly what each person was doing and made sure we didn???t veer off track.

The usual cycle wasn???t too different from how we usually develop code, since in our previous class (Operating Systems) we needed to compile code on the Temple servers, we already had most of the remote development part setup. Although, Ayser showed us how to create scripts for ssh???ing into the Wolfgand cluster so we were able to open up Command Prompt, type ???ssh -L 2222:localhost:22 temple-chinook??? then input our password for our separate TU accessnet accounts and we were able to connect to the temple servers through the CMD and through the IDE we used (CLion). 

Most of the testing we thought of for this project was to see if the time taken per multiplication of increasing sizes of matrices was getting longer and longer. Another way of testing we used was to compare the output matrices of the multiplication with a certain algorithm with the normal mmult method that was given to us. Therefore, we could check to make sure that the multiplication was done correctly. Furthermore, a lot of the testing was done by printing out the matrices and seeing exactly what was happening to the data. We considered that the mmult code given to us was without flaws, however, we did notice that the way of timing the mmult given to us was only outputting integers rather than numbers with decimals of detail, therefore we changed the way we recorded time for our project.

We wrote code to automate the testing of random matrices of sizes from 1-1000. This was a task on the Trello board since the start of the project and we have had to make several iterations of main.c as we continued adding different algorithms to our project as we needed to change how we captured the data, especially when automating the graphing. 

We wrote code to automate the generation of graphs for the different mmult algorithms we implemented. It was a task in our Trello board that we had marked with the color green to indicate that it was extra credit.
The proportion of time and tasks dedicated to writing variations of mmult and testing/reporting the multiplication was about even. As we added more variations of mmult, we realized that we needed to change how we tested/reported the information and how to make it so that a user could choose any variation after compiling main.c and it would output the information from that variation, rather than making a user compile a file per variation. Therefore, there was a large amount of time dedicated to both aspects of the project.

# Usage
This project has three different programs:  
* main
* main_mpi (flawed)
* mmult_mpi

main_mpi handles matrix multiplication through 1000 iterations of increasing matrix size using MPI while main utilizes every other method (OMP, SIMD) for matrix multiplication and mmult_mpi showcases matrix multiplication using MPI of a single user entered matrix size

Syntax:

	make main
	make main_mpi
	make mmult_mpi
	
	main:
	From [2,1000]
	./main O3
	./main O3 MPI
	./main O3 MPI OMP
	./main O3 MPI OMP SIMD
	./main O3 MPI OMP SIMD UNOPTIMIZED
	
	With Two Files: ./main [matrix1] [matrix2] [matrix_size]
	./main a.txt b.txt 5
	
	main_mpi:
	mpiexec -np [number of nodes/processes you want to use] ./main_mpi [max_matrix_size]
	mpiexec -np 4 ./main_mpi 1000
	
	mmult_mpi:
	mpiexec -np [number of nodes/processes you want to use] ./main_mpi [matrix_size]
	mpiexec -np 4 ./mmult_mpi 10

# Comparison Graph
![comparison_graph](comparison_graph.png)
