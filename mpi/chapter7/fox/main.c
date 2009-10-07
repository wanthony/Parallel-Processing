#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

typedef struct {
	int p; /* num processors */
	MPI_Comm comm; /* grid communicator */
	MPI_Comm comm_row; /* row communicator */
	MPI_Comm comm_col; /* col communicator */
	int q; /* order of grid */
	int my_row; /* my row number */
	int my_col; /* my col number */
	int my_rank; /* my process rank */
} GRID_INFO_T;

void Setup_grid(GRID_INFO_T *grid);
void Fox(int, GRID_INFO_T*, int *, int *, int *);
int *Set_to_zero(int *A);
void Local_matrix_multiply(int *A, int *B, int *C);

int main(int argc, char *argv[]) {
	GRID_INFO_T *grid;
	int *A, int *B, int *C;
	int n = 4;
	int i, j;
	int my_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	A = (int*)malloc(sizeof(int) * n);
	B = (int*)malloc(sizeof(int) * n);
	C = (int*)malloc(sizeof(int) * n);

	grid = (grid*)malloc(sizeof(grid));

	Setup_grid(grid);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			A[i * n + j] = 1;
			B[i * n + j] = 1;
			C[i * n + j] = 0;
		}
	}

	Fox(n, grid, A, B, C);

	if (my_rank == 0) {
		printf("A * B = \n");
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf(" %d ", C[i * n + j]);
			}

			printf("\n");
		}
	}

	MPI_Finalize();

	return 0;
}

void Setup_grid(GRID_INFO_T *grid) {
	int old_rank;
	int dimensions[2];
	int wrap_around[2];
	int coordinates[2];
	int free_coords[2];

	MPI_Comm_size(MPI_COMM_WORLD, &(grid->p));
	MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);

	grid->q = (int)sqrt((double) grid->p);
	dimensions[0] = dimensions[1] = grid->q;

	wrap_around[0] = wrap_around[1] = 1;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->comm));
	MPI_Comm_rank(grid->comm, &(grid->my_rank));
	MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
	grid->my_row = coordinates[0];
	grid->my_col = coordinates[1];

	free_coords[0] = 0;
	free_coords[1] = 1;
	MPI_Cart_sub(grid->comm, free_coords, &(grid->col_comm));
}

void Fox(int n, GRID_INFO_T *grid, int *local_A, int *local_B, int *local_C) {
	int *temp_A;
	int stage;
	int bcast_root;
	int n_bar;
	int source;
	int dest;
	MPI_Status status;

	n_bar = n / grid->q;

	Set_to_zero(local_C);

	source = (grid->my_row + 1) % grid->q;
	dest = (grid->my_row + grid->q - 1) % grid->q;

	temp_A = (int*)malloc(sizeof(int) * n_bar * n_bar);

	for (stage = 0; stage < grid->q; stage++) {
		bcast_root = (grid->my_row + stage) % grid->q;
		if (bcast_root == grid->my_col) {
			MPI_Bcast(local_A, 1, MPI_INT, bcast_root, grid->row_comm);
			Local_matrix_multiply(local_A, local_B, local_C);
		} else {
			MPI_Bcast(temp_A, 1, MPI_INT, bcast_root, grid->row_comm);
			Local_matrix_multiply(local_A, local_B, local_C);
		}

		MPI_Sendrecv_replace(local_B, 1, MPI_INT, dest, 0, source, 0, grid->col_comm, &status);
	}
}

int *Set_to_zero(int *A) {
	int *temp;
	int i;

	temp = (int*)malloc(sizeof(A));

	for (i = 0; i < sizeof(A) / sizeof(int); i++) {
		temp[i] = 0;
	}

	return temp;
}

void Local_matrix_multiply(int *A, int *B, int *C) {
	int i, j, k;

	int n = sizeof(A) / sizeof(int);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; i++) {
			C[i * n + j] = 0;
			for (k = 0; k < n; k++) {
				C[i * n + j] = A[i * n + k] * B[k * n + j];
			}
		}
	}
}
