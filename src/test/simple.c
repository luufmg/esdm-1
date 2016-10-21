#include <stdio.h>
#include <memvol.h>
#include <time.h>

void benchmark(FILE *f, int mode) {
    puts("Benchmark started...");
    /* setup */
    struct timespec begin;
    struct timespec end;
    unsigned long delta_create;
    unsigned long delta_dataset_create;
    unsigned long delta_write;
    unsigned long delta_read;
    unsigned long delta_close;
    unsigned long delta_dataset_f_create;
    unsigned long delta_f_write;
    unsigned long delta_f_read;
    unsigned long delta_f_close;
    hid_t fid, fprop, space, space_f, did_int, did_float;
    herr_t status;

    fprop = H5Pcreate(H5P_FILE_ACCESS);

    if (mode == 1) {
        hid_t vol_id = H5VL_memvol_init();
        H5Pset_vol(fprop, vol_id, &fprop);
    }

 	const int nloops = 10;
    
    
    for (int c = 0; c < nloops; c++) {
        char name[12];
        sprintf(name, "test%d.h5", c);

        hsize_t dims[2] = {10, 10};	
        int data[10][10];
        int data_float[10][10];
        int data_read[10][10];
        float data_f_read[10][10];

        int counter = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                data[i][j] = 1000 + ++counter;
                data_float[i][j] = 1000.25 + 0.25*(++counter);
            }
        }

        space = H5Screate_simple(2, dims, NULL);
        space_f = H5Screate_simple(2, dims, NULL);

        /* H5Fcreate */
        clock_gettime(CLOCK_MONOTONIC, &begin);

        fid = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, fprop);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_create += (end.tv_nsec - begin.tv_nsec);

        /* H5Dcreate int */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    did_int = H5Dcreate2(fid, "/test", H5T_NATIVE_INT, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);	

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_dataset_create += (end.tv_nsec - begin.tv_nsec);

        /* H5Dwrite int */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    status = H5Dwrite(did_int, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_write += (end.tv_nsec - begin.tv_nsec);

        /* H5Dread int */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    status = H5Dread(did_int, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_read);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_read += (end.tv_nsec - begin.tv_nsec);


	    status = H5Dclose(did_int);
        status = H5Sclose(space);

        /* H5Dcreate float */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    did_float = H5Dcreate2(fid, "/test_float", H5T_NATIVE_FLOAT, space_f, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);	

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_dataset_f_create += (end.tv_nsec - begin.tv_nsec);
       
        /* H5Dwrite float */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    status = H5Dwrite(did_float, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_float);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_f_write += (end.tv_nsec - begin.tv_nsec); 

        /* H5Dread float */
        clock_gettime(CLOCK_MONOTONIC, &begin);

	    status = H5Dread(did_float, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_f_read);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_f_read += (end.tv_nsec - begin.tv_nsec);
    
        status = H5Dclose(did_float);
        status = H5Sclose(space_f);


        /* H5Fclose */
        clock_gettime(CLOCK_MONOTONIC, &begin);

        status = H5Fclose(fid);

        clock_gettime(CLOCK_MONOTONIC, &end);
        delta_close += (end.tv_nsec - begin.tv_nsec);

        const char* version;
        if (mode == 1) {
            version = "memvol";
        } else {
            version = "standard";
        }

        char dsize[12];
        sprintf(dsize, "%dx%d", 10, 10);

        /* Dataframe Outline:
         * "Hardware" "Memory" "DataSize" "HDFversion" "H5Fcreate" "H5Dcreate int" "H5Dwrite int" "H5Dread int" "H5Dcreate float" "H5Dwrite float" "H5Dread float" "H5Fclose"
         */
        fprintf(f, "\"%s\" \"%s\" %lu %lu %lu %lu %lu %lu %lu %lu\n", dsize, version, delta_create, delta_dataset_create, delta_write, delta_read, delta_dataset_f_create, delta_f_write, delta_f_read, delta_close);
    }
}


int main(int argc, char** argv) {

    FILE *f = fopen("benchmark.log", "w");
    if (f == NULL) {
        puts("Error opening file!");
        return 1;
    }
    fprintf(f, "\"Hardware\" \"Memory\" \"DataSize\" \"HDFversion\" \"H5Fcreate\" \"H5Dcreate int\" \"H5Dwrite int\" \"H5Dread int\" \"H5Dcreate float\" \"H5Dwrite float\" \"H5Dread float\" \"H5Fclose\"\n");
  
    /* HDF standard */
    benchmark(f, 0);

    /* HDF memvol */
    benchmark(f, 1);

    fclose(f);

	H5VL_memvol_finalize();
}
