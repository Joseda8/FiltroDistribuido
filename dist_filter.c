#include <mpi.h> 
#include "filters.h"


void apply_median(char* img_name, char* out_name){
    make_img_conversion(img_name);
    median_filter_main("img.ppm", out_name);
}

void make_img_conversion(char *filename) {
    char convert_cmd[26] = "convert ";
    char name_img_out[26] = " img.ppm";
    strcat(convert_cmd, filename);
    strcat(convert_cmd, name_img_out);
    system(convert_cmd);
}

  
int main(int argc, char* argv[]) { 

    int nparams = argc - 1;
    if(nparams < 1) {
        printf( "Wrong number of arguments\n" );
        return -1;
    }
  
    int pid, np;

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    // np -> no. of processes 
    // pid -> process id 
  
    MPI_Status status; 
  
    // Creation of parallel processes 
    MPI_Init(&argc, &argv); 
    MPI_Get_processor_name(processor_name, &name_len);
  
    // find out process ID, 
    // and how many processes were started 
    MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
    MPI_Comm_size(MPI_COMM_WORLD, &np); 
  
    // master process 
    if (pid == 0) { 
        char* img_name = argv[1];
        char* img_format = &img_name[strlen(img_name)-4];

        int START_FLAG = 1;

        char crop_img_cmd[50] = "convert -crop 50%x100% ";
        char crop_img_out[50] = " out.jpg";
        strcat(crop_img_cmd, img_name);
        strcat(crop_img_cmd, crop_img_out);
        system(crop_img_cmd);

        //Se envía mensaje al nodo 1 para que inicie el trabajo
        MPI_Send(&START_FLAG, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 

        //Se inicia el trabajo en el nodo 0
        printf("Applying Median Filter in %s. Core %d out of %d processors\n", processor_name, pid, np);
        apply_median("out-0.jpg", "out0.ppm");

        //Cuando el nodo 1 termine su parte, envía un mensaje
        MPI_Recv(&START_FLAG, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status); 

        printf("Joining images in %s. Core %d out of %d processors\n", processor_name, pid, np);
        char join_img_cmd[75] = "convert out0.ppm out1.ppm +append result";
        strcat(join_img_cmd, img_format);
        system(join_img_cmd);

        system("rm img.ppm out0.ppm out1.ppm out-0.jpg out-1.jpg");
  
    } 
    // slave processes 
    else { 

        int START_FLAG;

        //Recibe mensaje de inicio
        MPI_Recv(&START_FLAG, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 

        //Realiza el trabajo y envía el aviso al nodo master
        printf("Applying Median Filter in %s. Core %d out of %d processors\n", processor_name, pid, np);
        apply_median("out-1.jpg", "out1.ppm");

        MPI_Send(&START_FLAG, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    } 
  
    MPI_Finalize(); 
  
    return 0; 
} 
