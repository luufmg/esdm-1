#include <stdio.h>
#include <time.h>
#include <memvol.h>

void benchmark(FILE *f, int mode) {
    /* setup */
    clock_t begin, end;
    double delta_create = 0.0;
    double delta_dataset_create = 0.0;
    double delta_write = 0.0;
    double delta_read= 0.0;
    double delta_close = 0.0;
    hid_t fid, fprop, space, did;
    herr_t status;

    fprop = H5Pcreate(H5P_FILE_ACCESS);

    if (mode == 1) {
        hid_t vol_id = H5VL_memvol_init();
        H5Pset_vol(fprop, vol_id, &fprop);
    }

    int data_read[3][4];
 	const int nloops = 10000;
    
    
    for (int i = 0; i < nloops; i++) {
        char name[12];
        sprintf(name, "test%d.h5", i);

	hsize_t dims[2] = {3, 4};	
	int data[3][4];
	int counter = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			data[i][j] = 1000 + ++counter;
		}
	}

	space = H5Screate_simple(2, dims, NULL);

        begin = clock();
        fid = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, fprop);
        end = clock();
        delta_create += (double)(end-begin) / CLOCKS_PER_SEC;

        begin = clock();
	did = H5Dcreate2(fid, "/test", H5T_NATIVE_INT, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);	
        end = clock();
        delta_dataset_create += (double)(end-begin) / CLOCKS_PER_SEC;

        begin = clock();
	status = H5Dwrite(did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
        end = clock();
        delta_write += (double)(end-begin) / CLOCKS_PER_SEC;


        begin = clock();
	status = H5Dread(did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_read);
        end = clock();
        delta_read += (double)(end-begin) / CLOCKS_PER_SEC;

     

        begin = clock();
	status = H5Dclose(did);
        end = clock();
        delta_close += (double)(end-begin) / CLOCKS_PER_SEC;

         status = H5Sclose(space);
         status = H5Fclose(fid);

    }

    /* calculate means */

        printf("\n BUFFER: ");
	
       for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d,", data_read[i][j]);
		}
	}
    
    const char* header;
    if (mode == 1) {
        header = "HDF5 memvol benchmark ##########";
    } else {
        header = "HDF5 standard benchmark ##########";
    }

    fprintf(f, "%s\n", header);

	printf("\n");
    double avg_create = (double)delta_create / nloops;
    fprintf(f, "\nAverage Execution Time for H5Fcreate: %.10f\n", avg_create);

    double avg_dataset_create = (double)delta_dataset_create / nloops;
    fprintf(f, "Average Execution Time for H5Dcreate: %.10f\n", avg_dataset_create); 

    double avg_write = (double)delta_write / nloops;
    fprintf(f, "Average Execution Time for H5Dwrite: %.10f\n", avg_write);

    double avg_read = (double)delta_read / nloops;
    fprintf(f, "Average Execution Time for H5read: %.10f\n", avg_read);

    double avg_close = (double)delta_close / nloops;
    fprintf(f, "Average Execution Time for H5Fclose: %.10f\n", avg_close);
}


int main(int argc, char** argv) {

    FILE *f = fopen("benchmark.log", "w");
    if (f == NULL) {
        puts("Error opening file!");
        return 1;
    }
  
    /* HDF standard */
    benchmark(f, 0);

    fprintf(f, "\n");

    /* HDF memvol */
    benchmark(f, 1);

	H5VL_memvol_finalize();
}
