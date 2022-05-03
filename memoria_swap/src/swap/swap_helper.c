#include "../../include/swap/swap_helper.h"

char* obtener_ruta_swap_proceso(int pid){
    char* pid_string = string_itoa(pid);
    puts(mem_swap_config->path_swap);
    int ultimo_caracter_path = mem_swap_config->path_swap[strlen(mem_swap_config->path_swap)]; // Sin incluir '\n'
    char* final_string;
    if(ultimo_caracter_path == '/'){
        // Caso /home/utnso/swap/

        // Concadeno directamente.
        final_string = string_from_format("%s%s%s",mem_swap_config->path_swap, pid_string,".swap");
    }
    else{
        // Caso /home/utnso/swap

        // Le agrego '/' antes del nombre del archivo
        final_string = string_from_format("%s/%s%s",mem_swap_config->path_swap, pid_string,".swap");
    }

    free(pid_string);
    return final_string;
}

int crear_archivo_swap(archivo_pid_t* archivo) {

    debug_log("swap_helper.c@crear_archivo_swap", "Archivo iterado: ");
    debug_log("swap_helper.c@crear_archivo_swap", archivo->path_archivo);

    int fd_archivo_swap = open(archivo->path_archivo, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd_archivo_swap == -1) {
        error_log("swap_helper.c@crear_archivo_swap", "No se pudo crear el area de swap");
        return -1;
    }

    ftruncate(fd_archivo_swap, archivo->tam_proceso);

    archivo->area_archivo_swap = mmap(NULL, archivo->tam_proceso, PROT_READ | PROT_WRITE, MAP_SHARED, fd_archivo_swap,
                                      0);

    if (errno != 0) {
        char *error;
        error = string_from_format("Error al crear area de swap: errno %i", errno);
        error_log("swap_helper.c@crear_archivo_swap", error);
        free(error);
    } else {
        debug_log("swap_helper.c@crear_archivo_swap", "Area de swap mapeada correctamente!");
    }

    memset(archivo->area_archivo_swap, '\0', archivo->tam_proceso);

    close(fd_archivo_swap);

    return 0; // Exito!
}
