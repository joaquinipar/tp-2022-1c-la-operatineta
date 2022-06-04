#include "../include/monitor_colas_procesos.h"

// Todos los procesos admitidos al kernel
static t_list *procesos;

static t_list *cola_nuevos;
static t_list *cola_listos;
static t_list *cola_ejecucion;
static t_list *cola_bloqueados;
static t_list *cola_bloqueados_suspendidos;
static t_list *cola_suspendidos_listos;
static t_list *cola_terminados;

pthread_mutex_t procesos_mutex;
pthread_mutex_t procesos_nuevos_mutex;
pthread_mutex_t procesos_listos_mutex;
pthread_mutex_t procesos_ejecutando_mutex;
pthread_mutex_t procesos_bloqueados_mutex;
pthread_mutex_t procesos_bloqueados_suspendidos_mutex;
pthread_mutex_t procesos_suspendidos_listos_mutex;
pthread_mutex_t procesos_terminados_mutex;

int cantidad_procesos_suspendidos_listos();

void iniciar_monitor_colas_procesos() {
  debug_log("monitor_colas_procesos.c@init",
            "Inicializando variables del monitor de colas de procesos");

  procesos = list_create();
  cola_nuevos = list_create();
  cola_listos = list_create();
  cola_ejecucion = list_create();
  cola_bloqueados = list_create();
  cola_bloqueados_suspendidos = list_create();
  cola_suspendidos_listos = list_create();
  cola_terminados = list_create();

  pthread_mutex_init(&procesos_mutex, NULL);
  pthread_mutex_init(&procesos_nuevos_mutex, NULL);
  pthread_mutex_init(&procesos_listos_mutex, NULL);
  pthread_mutex_init(&procesos_ejecutando_mutex, NULL);
  pthread_mutex_init(&procesos_bloqueados_mutex, NULL);
  pthread_mutex_init(&procesos_bloqueados_suspendidos_mutex, NULL);
  pthread_mutex_init(&procesos_suspendidos_listos_mutex, NULL);
  pthread_mutex_init(&procesos_terminados_mutex, NULL);

  sem_init(&sem_proceso_nuevo, 0, 0);
  sem_init(&sem_proceso_listo, 0, 0);
  sem_init(&sem_grado_multiprogramacion_completo, 0, 0);
  sem_init(&sem_proceso_suspendido, 0, 0);
  sem_init(&sem_bin_procesar_listo, 0, 1);

  debug_log("monitor_colas_procesos.c@init",
            "Variables del monitor de colas de procesos inicializadas");
}

void destruir_monitor_colas_procesos() {
  debug_log("monitor_colas_procesos.c@destroy",
            "Destruyendo el monitor de colas de procesos");

  list_destroy_and_destroy_elements(cola_nuevos, (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_listos, (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_ejecucion, (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_bloqueados, (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_bloqueados_suspendidos,
                                    (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_suspendidos_listos,
                                    (void *)proceso_destruir);
  list_destroy_and_destroy_elements(cola_terminados, (void *)proceso_destruir);

  pthread_mutex_destroy(&procesos_mutex);
  pthread_mutex_destroy(&procesos_nuevos_mutex);
  pthread_mutex_destroy(&procesos_listos_mutex);
  pthread_mutex_destroy(&procesos_ejecutando_mutex);
  pthread_mutex_destroy(&procesos_bloqueados_mutex);
  pthread_mutex_destroy(&procesos_bloqueados_suspendidos_mutex);
  pthread_mutex_destroy(&procesos_suspendidos_listos_mutex);
  pthread_mutex_destroy(&procesos_terminados_mutex);

  debug_log("monitor_colas_procesos.c@destroy",
            "Monitor de colas de procesos destruido");
}

void agregar_proceso_a_lista_de_procesos(pcb_t *proceso) {
  pthread_mutex_lock(&procesos_mutex);
  list_add(procesos, proceso);
  pthread_mutex_unlock(&procesos_mutex);
}

pcb_t *buscar_proceso(uint32_t pid) {
  bool buscar_pid(pcb_t * proceso) { return proceso->pid == pid; }

  return list_find(procesos, (void *)buscar_pid);
}

int cantidad_procesos_suspendidos_listos() {
  int cantidad;
  pthread_mutex_lock(&procesos_suspendidos_listos_mutex);
  cantidad = list_size(cola_suspendidos_listos);
  pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

  format_debug_log("monitor_colas_procesos@cantidad_procesos_suspendidos_listos", "Cantidad de procesos suspendidos listos: %d", cantidad);
  return cantidad;
}

/*  --------------------------- Funciones Cola Nuevos
 * ---------------------------  */

bool encolar_proceso_en_nuevos(pcb_t *proceso) {
  debug_log("monitor_colas_procesos.c@encolar_proceso_nuevo", "Encolando un nuevo proceso");

  pthread_mutex_lock(&procesos_nuevos_mutex);
  list_add(cola_nuevos, proceso);
  proceso->estado = ESTADO_PROCESO_NEW;
  proceso->duracion_ultima_rafaga = 0; // agregado
  proceso->estimacion = kernel_config->estimacion_inicial; // agregado
  pthread_mutex_unlock(&procesos_nuevos_mutex);

  format_debug_log("monitor_cols_procesos@encolar_proceso_en_nuevos", "Proceso pid: %d encolado en nuevos", proceso->pid);

  sem_post(&sem_proceso_nuevo);

  return true;
}

pcb_t *desencolar_proceso_nuevo() {
  debug_log("monitor_colas_procesos.c@obtener_proceso_en_cola_nuevos",
            "Obteniendo un proceso de la cola de nuevos");

  pthread_mutex_lock(&procesos_nuevos_mutex);

  pcb_t *proceso = list_remove(cola_nuevos, 0);

  pthread_mutex_unlock(&procesos_nuevos_mutex);

  debug_log("monitor_colas_procesos.c@obtener_proceso_en_cola_nuevos",
            "Proceso de la cola de nuevos obtenido");

  return proceso;
}

pcb_t *buscar_proceso_en_nuevos(int pid) {

  bool pid_iguales(pcb_t *proceso, int pid)
  {
		return proceso->pid == pid;
  }

  pcb_t *proceso_nuevo;

  pthread_mutex_lock(&procesos_nuevos_mutex);

  proceso_nuevo = list_find(cola_nuevos, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_nuevos_mutex);

  if (proceso_nuevo != NULL) {
    pthread_mutex_lock(&procesos_nuevos_mutex);

    list_remove_by_condition(cola_nuevos, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_nuevos_mutex);

    return proceso_nuevo;
  }

  char *mensaje = string_from_format(
      "El proceso pid: %d no se encuentra en cola de nuevos", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_nuevos", mensaje);
  free(mensaje);

  return proceso_nuevo;
}

bool lista_de_nuevos_vacia()
{
	bool resultado;

    pthread_mutex_lock(&procesos_nuevos_mutex);

    resultado = list_is_empty(cola_nuevos);

    pthread_mutex_unlock(&procesos_nuevos_mutex);

    return resultado;
}

/*  --------------------------- Funciones Cola Listos
 * ---------------------------  */

void encolar_proceso_en_listos(pcb_t *proceso) {
  char *msg = string_from_format("El proceso con pid: %d sera movido a cola de listos", proceso->pid);
  debug_log("monitor_colas_procesos.c@encolar_proceso_en_listos", msg);
  free(msg);
  
  pthread_mutex_lock(&procesos_listos_mutex);

  list_add(cola_listos, proceso);
  proceso->estado = ESTADO_PROCESO_READY;
  //proceso_iniciar_espera(proceso);

  pthread_mutex_unlock(&procesos_listos_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d encolado en listos", proceso->pid);
  info_log("monitor_colas_procesos.c@encolar_proceso_en_listos", mensaje);
  free(mensaje);

  sem_post(&sem_proceso_listo);
}

pcb_t *desencolar_proceso_listo() {

  pthread_mutex_lock(&procesos_listos_mutex);

  pcb_t *proceso = list_remove(cola_listos, 0);
  //proceso_finalizar_espera(proceso);

  pthread_mutex_unlock(&procesos_listos_mutex);

  return proceso;
}

void ordenar_cola_listos() {

	//printf("\n\nCola listos previo a ordenarse: \n");

	void iterator(pcb_t* proceso)
	{
    format_info_log("monitor_colas_procesos.c@ordenar_cola_listos", "Proceso pid: %d", proceso->pid);
    format_info_log("monitor_colas_procesos.c@ordenar_cola_listos", "Ultima rafaga: %d", proceso->duracion_ultima_rafaga);
    format_info_log("monitor_colas_procesos.c@ordenar_cola_listos", "Proxima ráfaga estimada: %d", proxima_rafaga_estimada(proceso));
    format_info_log("monitor_colas_procesos.c@ordenar_cola_listos", "Estimación anterior: %d", proceso->estimacion);
	}

	void actualizar_estimacion_anterior(pcb_t* proceso)
	{
		proceso->estimacion = proxima_rafaga_estimada(proceso);
	}

	//list_iterate(cola_listos, (void*) iterator);

  pthread_mutex_lock(&procesos_listos_mutex);

  list_sort(cola_listos, (void *)mayor_prioridad);

  //list_iterate(cola_listos, (void*) actualizar_estimacion_anterior);

  info_log("monitor_colas_procesos.c@ordenar_cola_listos", "Cola listos ordenada: ");
  list_iterate(cola_listos, (void*) iterator);

  pthread_mutex_unlock(&procesos_listos_mutex);
}

pcb_t *buscar_proceso_en_listos(int pid) {

	bool pid_iguales(pcb_t *proceso, int pid)
    {
		return proceso->pid == pid;
	}

  pcb_t *proceso_listo;

  pthread_mutex_lock(&procesos_listos_mutex);

  proceso_listo = list_find(cola_listos, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_listos_mutex);

  if (proceso_listo != NULL) {
    pthread_mutex_lock(&procesos_listos_mutex);

    list_remove_by_condition(cola_listos, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_listos_mutex);

    return proceso_listo;
  }

  char *mensaje = string_from_format(
      "El proceso pid: %d no se encuentra en cola de listos", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_listos", mensaje);
  free(mensaje);

  return proceso_listo;
}

bool lista_de_listos_vacia()
{
	bool resultado;

    pthread_mutex_lock(&procesos_listos_mutex);

    resultado = list_is_empty(cola_listos);

    pthread_mutex_unlock(&procesos_listos_mutex);

    return resultado;
}

/*  --------------------------- Funciones Cola Ejecucion
 * ---------------------------  */

void encolar_proceso_en_ejecucion(pcb_t *proceso) {

  pthread_mutex_lock(&procesos_ejecutando_mutex);

  list_add(cola_ejecucion, proceso);

  pthread_mutex_unlock(&procesos_ejecutando_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d encolado en ejecucion", proceso->pid);
  info_log("monitor_colas_procesos.c@encolar_proceso_en_ejecucion", mensaje);
  free(mensaje);
}

pcb_t *desencolar_proceso_en_ejecucion() {

  pthread_mutex_lock(&procesos_ejecutando_mutex);
  pcb_t *proceso_en_ejecucion = list_remove(cola_ejecucion, 0);
  pthread_mutex_unlock(&procesos_ejecutando_mutex);

  format_debug_log("monitor_colas_procesos.c@desencolar_proceso_en_ejecucion", "Proceso pid: %d desencolado de ejecucion", proceso_en_ejecucion->pid);

  return proceso_en_ejecucion;
}

pcb_t *buscar_proceso_en_ejecucion(int pid) {

	bool pid_iguales(pcb_t * proceso)
	{
		return proceso->pid == pid;
	}

  pcb_t *proceso_en_ejecucion;

  pthread_mutex_lock(&procesos_ejecutando_mutex);

  proceso_en_ejecucion = list_find(cola_ejecucion, (void *)pid_iguales);

  char *mensaje1 = string_from_format(
      "Proceso pid: %d encontrado en ejecucion", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_ejecucion", mensaje1);
  free(mensaje1);

  pthread_mutex_unlock(&procesos_ejecutando_mutex);

  if (proceso_en_ejecucion != NULL) {
    pthread_mutex_lock(&procesos_ejecutando_mutex);

    list_remove_by_condition(cola_ejecucion, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_ejecutando_mutex);

    return proceso_en_ejecucion;
  }

  char *mensaje2 = string_from_format(
      "El proceso pid: %d no se encuentra en ejecucion", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_ejecucion", mensaje2);
  free(mensaje2);

  return proceso_en_ejecucion;
}

/*  --------------------------- Funciones Cola Bloqueados
 * ---------------------------  */

void encolar_proceso_en_bloqueados(pcb_t *proceso, int retardo) {
  pthread_mutex_lock(&procesos_bloqueados_mutex);
  
  proceso->estado = ESTADO_PROCESO_BLOCKED;
  bloqueo_proceso_t *proceso_bloqueado = malloc(sizeof(bloqueo_proceso_t));
  proceso_bloqueado->retardo = retardo;
  proceso_bloqueado->proceso = proceso;
  list_add(cola_bloqueados, proceso_bloqueado);
  format_debug_log("monitor_colas_procesos.c@encolar_proceso_en_bloqueados", "Proceso pid: %d encolado en bloqueados", proceso->pid);

  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  //TODO: hacer signal de sem_cont_procesos_bloqueados (en lo posible llamar a funcion en dispositivo_io.c que lo haga)
}

pcb_t *desencolar_proceso_bloqueado() {

  pthread_mutex_lock(&procesos_bloqueados_mutex);

  pcb_t *proceso = list_remove(cola_bloqueados, 0);

  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d desencolado de bloqueados", proceso->pid);
  debug_log("monitor_colas_procesos.c@desencolar_proceso_en_bloqueados", mensaje);
  free(mensaje);

  return proceso;
}

pcb_t *obtener_ultimo_proceso_bloqueado() {

  pcb_t *proceso;
  int tam_lista;

  pthread_mutex_lock(&procesos_bloqueados_mutex);
  tam_lista = list_size(cola_bloqueados);
  proceso = list_remove(cola_bloqueados, tam_lista - 1);
  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  char *mensaje = string_from_format(
      "Último proceso pid: %d desencolado de bloqueados", proceso->pid);
  debug_log("monitor_colas_procesos.c@obtener_ultimo_proceso_bloqueado", mensaje);
  free(mensaje);

  return proceso;
}

pcb_t *buscar_y_sacar_proceso_en_bloqueados(int pid) {

  bool pid_iguales(pcb_t *proceso)
  {
	  return proceso->pid == pid;
  }

  pcb_t *proceso_bloqueado;

  pthread_mutex_lock(&procesos_bloqueados_mutex);

  proceso_bloqueado = list_find(cola_bloqueados, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  if (proceso_bloqueado != NULL) {
    pthread_mutex_lock(&procesos_bloqueados_mutex);

    list_remove_by_condition(cola_bloqueados, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_bloqueados_mutex);

    return proceso_bloqueado;
  }

  char *mensaje = string_from_format(
      "El proceso pid: %d no se encuentra en cola de bloqueados", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_bloqueados", mensaje);
  free(mensaje);

  return proceso_bloqueado;
}

bool lista_de_bloqueados_vacia()
{
	bool resultado;

    pthread_mutex_lock(&procesos_bloqueados_mutex);

    resultado = list_is_empty(cola_bloqueados);

    pthread_mutex_unlock(&procesos_bloqueados_mutex);

    return resultado;
}

/*  --------------------------- Funciones Cola Bloqueados Suspendidos
 * ---------------------------  */

void encolar_proceso_en_bloqueados_suspendidos(pcb_t *proceso) {

  pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);

  list_add(cola_bloqueados_suspendidos, proceso);
  proceso->estado = ESTADO_PROCESO_BLOCKED_SUSPENDED;

  pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d encolado en bloqueados_suspendidos", proceso->pid);
  info_log("monitor_colas_procesos.c@encolar_proceso_en_bloqueados_suspendidos", mensaje);
  free(mensaje);
}

pcb_t *desencolar_proceso_bloqueado_suspendido() {

  pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);
  pcb_t *proceso = list_remove(cola_bloqueados_suspendidos, 0);
  pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

  return proceso;
}

pcb_t *buscar_y_sacar_proceso_en_bloqueados_suspendidos(int pid) {

  bool pid_iguales(pcb_t *proceso)
  {
	  return proceso->pid == pid;
  }

  pcb_t *proceso_bloqueado_suspendido;

  pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);

  proceso_bloqueado_suspendido =
      list_find(cola_bloqueados_suspendidos, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

  if (proceso_bloqueado_suspendido != NULL) {
    pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);

    list_remove_by_condition(cola_bloqueados_suspendidos, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

    return proceso_bloqueado_suspendido;
  }

  char *mensaje = string_from_format(
      "El proceso pid: %d no se encuentra en cola de bloqueados suspendidos",
      pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_bloquedos suspendidos",
            mensaje);
  free(mensaje);

  return proceso_bloqueado_suspendido;
}

/*  --------------------------- Funciones Cola Suspendidos Listos
 * ---------------------------  */

void encolar_proceso_en_suspendidos_listos(pcb_t *proceso) {

  pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

  list_add(cola_suspendidos_listos, proceso);
  proceso->estado = ESTADO_PROCESO_SUSPENDED_READY;

  pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d encolado en suspendidos_listos", proceso->pid);
  info_log("monitor_colas_procesos.c@encolar_proceso_suspendido_listo", mensaje);
  free(mensaje);

}

pcb_t *desencolar_proceso_suspendido_listo() {

  debug_log("monitor_colas_procesos.c@desencolar_proceso_suspendido_listo",
	        "Obteniendo un proceso de la cola suspendidos_listos");

  pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

  pcb_t *proceso = list_remove(cola_suspendidos_listos, 0);

  pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d desencolado de suspendidos_listos", proceso->pid);
  debug_log("monitor_colas_procesos.c@desencolar_proceso_suspendido_listo", mensaje);
  free(mensaje);

  return proceso;
}

pcb_t *buscar_proceso_en_suspendidos_listos(int pid) {

  bool pid_iguales(pcb_t *proceso, int pid)
  {
	  return proceso->pid == pid;
  }

  pcb_t *proceso_suspendido_listo;

  pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

  proceso_suspendido_listo =
      list_find(cola_suspendidos_listos, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

  if (proceso_suspendido_listo != NULL) {
    pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

    list_remove_by_condition(cola_suspendidos_listos, (void *)pid_iguales);

    pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

    return proceso_suspendido_listo;
  }

  char *mensaje = string_from_format(
      "El proceso pid: %d no se encuentra en cola de suspendidos listos", pid);
  debug_log("monitor_colas_procesos.c@buscar_proceso_en_suspendidos listos",
            mensaje);
  free(mensaje);

  return proceso_suspendido_listo;
}

bool lista_de_suspendidos_listos_vacia()
{
	bool resultado;

    pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

    resultado = list_is_empty(cola_suspendidos_listos);

    pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

    return resultado;
}

/*  --------------------------- Funciones Cola Terminados
 * ---------------------------  */

void encolar_proceso_en_terminados(pcb_t *proceso) {

  debug_log("monitor_colas_procesos.c@encolar_proceso_en_terminados",
            "Encolando proceso terminado");

  pthread_mutex_lock(&procesos_terminados_mutex);

  list_add(cola_terminados, proceso);
  proceso->estado = ESTADO_PROCESO_EXIT;

  pthread_mutex_unlock(&procesos_terminados_mutex);

  debug_log("monitor_colas_procesos.c@encolar_proceso_nuevo", "Proceso terminado encolado");
}

/*  --------------------------- Otras funciones ---------------------------  */

void mover_proceso_a_listo() {
  pcb_t *proceso;

  if (cantidad_procesos_suspendidos_listos() > 0) {

    proceso = desencolar_proceso_suspendido_listo();
    encolar_proceso_en_listos(proceso);

  } else {

    proceso = desencolar_proceso_nuevo();
    encolar_proceso_en_listos(proceso);

  }
}

void mover_proceso_nuevo_a_suspendido_listo() {
  pcb_t *proceso;

  proceso = desencolar_proceso_nuevo();
  encolar_proceso_en_suspendidos_listos(proceso);

  format_debug_log("monitor_colas_procesos.c@mover_proceso_nuevo_a_suspendido_listo", "Proceso: %d movido a suspendido-listo", proceso->pid);
  cantidad_procesos_suspendidos_listos();
}

pcb_t *mover_proceso_listo_a_ejecucion() {
  pcb_t *proceso;

  if(kernel_config -> algoritmo_planificacion == SRT){
    ordenar_cola_listos();
  }
  proceso = desencolar_proceso_listo();
  encolar_proceso_en_ejecucion(proceso);
  proceso_ejecutar(proceso);

  return proceso;
}

void mover_ultimo_proceso_bloqueado_a_suspendido() {
  pcb_t *proceso;

  proceso = obtener_ultimo_proceso_bloqueado();
  encolar_proceso_en_bloqueados_suspendidos(proceso);
}

void mover_proceso_ejecutando_a_terminados() {
  pcb_t *proceso;

  proceso = desencolar_proceso_en_ejecucion();
  encolar_proceso_en_terminados(proceso);
}

void mover_proceso_en_ejecucion_a_terminados(uint32_t pid) {

  bool pid_iguales(pcb_t * proceso) {
		return proceso->pid == pid;
	}

  pthread_mutex_lock(&procesos_ejecutando_mutex);
  pthread_mutex_lock(&procesos_terminados_mutex);

  pcb_t *proceso = list_remove_by_condition(cola_ejecucion, (void *)pid_iguales);

  if (proceso == NULL) {
    error_log("monitor_colas_procesos@mover_proceso_en_ejecucion_a_terminados", "Error al buscar proceso para terminar (no esta en cola de ejecucion)");
    pthread_mutex_unlock(&procesos_terminados_mutex);
    pthread_mutex_unlock(&procesos_ejecutando_mutex);
    return;
  }

  encolar_proceso_en_terminados(proceso);

  pthread_mutex_unlock(&procesos_terminados_mutex);
  pthread_mutex_unlock(&procesos_ejecutando_mutex);

}

void mover_proceso_de_bloqueados_a_terminados(uint32_t pid) {

  char *msg = string_from_format("Moviendo proceso de bloqueado a terminados, pid: %d", pid);
  info_log("monitor_colas_procesos.c@mover_proceso_de_bloqueados_a_terminados", msg);
  free(msg);

  bool pid_iguales(pcb_t * proceso) {
		return proceso->pid == pid;
	}

  pthread_mutex_lock(&procesos_bloqueados_mutex);
  pcb_t *proceso = list_remove_by_condition(cola_bloqueados, (void *)pid_iguales);
  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  if (proceso == NULL) {
    error_log("monitor_colas_procesos@mover_proceso_de_bloqueados_a_terminados", "Error al buscar proceso para terminar (no esta en cola de bloqueados)");
    return;
  }

  encolar_proceso_en_terminados(proceso);
}

int cantidad_procesos_en_multiprogramacion() {

  int cantidad_listos, cantidad_ejecutando, cantidad_bloqueados, cantidad_total;

  pthread_mutex_lock(&procesos_listos_mutex);
  pthread_mutex_lock(&procesos_ejecutando_mutex);
  pthread_mutex_lock(&procesos_bloqueados_mutex);

  cantidad_listos = list_size(cola_listos);
  cantidad_ejecutando = list_size(cola_ejecucion);
  cantidad_bloqueados = list_size(cola_bloqueados);

  pthread_mutex_unlock(&procesos_listos_mutex);
  pthread_mutex_unlock(&procesos_ejecutando_mutex);
  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  cantidad_total = cantidad_listos + cantidad_ejecutando + cantidad_bloqueados;

  char *mensaje1 = string_from_format(
      "Cantidad procesos en listos: %d", cantidad_listos);
  debug_log("monitor_colas_procesos.c@cantidad_procesos_en_multiprogramacion", mensaje1);
  free(mensaje1);

  char *mensaje2 = string_from_format(
      "Cantidad procesos ejecutando: %d", cantidad_ejecutando);
  debug_log("monitor_colas_procesos.c@cantidad_procesos_en_multiprogramacion", mensaje2);
  free(mensaje2);

  char *mensaje3 = string_from_format(
      "Cantidad procesos bloqueados: %d", cantidad_bloqueados);
  debug_log("monitor_colas_procesos.c@cantidad_procesos_en_multiprogramacion", mensaje3);
  free(mensaje3);

  char *mensaje = string_from_format(
      "Cantidad procesos en sistema: %d", cantidad_total);
  debug_log("monitor_colas_procesos.c@cantidad_procesos_en_multiprogramacion", mensaje);
  free(mensaje);

  return cantidad_total;
}

bool grado_multiprogramacion_completo()
{
	bool resultado;

	resultado = cantidad_procesos_en_multiprogramacion() >= kernel_config->grado_multiprogramacion;

	if(resultado == true)
	{
    info_log("monitor_colas_procesos.c@grado_de_multiprogramacion", "Grado de multiprogramacion completo");
		/*debug_log("kernel/grado_de_multiprogramacion_completo.c",
				  "Grado de multiprogramación completo");*/
	}

	return resultado;
}

/**
 *  Esta funcion solo debe usarse para matar un proceso
 *  busca un proceso por pid en las distintas colas, lo saca y lo devuelve
 * */
pcb_t *buscar_y_sacar_proceso_de_cualquier_cola(uint32_t pid) {
  char *msg = string_from_format("Buscando el proceso con pid: %d en las colas", pid);
  debug_log("kernel/monitor_colas_procesos.c@buscar_y_sacar_proceso_de_cualquier_cola", msg);
  free(msg);

  pcb_t *proceso_encontrado = NULL;

  proceso_encontrado = buscar_proceso_en_nuevos(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  proceso_encontrado = buscar_proceso_en_listos(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  proceso_encontrado = buscar_proceso_en_ejecucion(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  proceso_encontrado = buscar_y_sacar_proceso_en_bloqueados(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  proceso_encontrado = buscar_y_sacar_proceso_en_bloqueados_suspendidos(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  proceso_encontrado = buscar_proceso_en_suspendidos_listos(pid);
  if(proceso_encontrado != NULL)
  {
	  return proceso_encontrado;
  }

  char *msg3 =
      string_from_format("El proceso con pid: %d no fue encontrado", pid);
  debug_log("kernel/"
            "monitor_colas_procesos.c@buscar_y_sacar_proceso_de_cualquier_cola",
            msg3);
  free(msg3);

  return proceso_encontrado;
}

bool mayor_prioridad(pcb_t *proceso1, pcb_t *proceso2) {

  bool resultado;

  switch (kernel_config->algoritmo_planificacion) {
  case SRT:

    resultado =
        proxima_rafaga_estimada(proceso1) <= proxima_rafaga_estimada(proceso2);

    	//printf("\n\nProx ráfaga 1: %d\n\n", proxima_rafaga_estimada(proceso1));
    	//printf("Prox ráfaga 2: %d\n\n", proxima_rafaga_estimada(proceso2));

    break;

  case FIFO:

    //(Sebas)esto no deberia hacer nada

    //resultado = (proceso1->estimacion - proceso2->estimacion) > 0;
    
    break;
  }

  return resultado;
}

int proxima_rafaga_estimada(pcb_t *proceso) {

  // Fórmula SJF	pr = ur * alpha + (1 - alpha) * t estimado ur

  return proceso->duracion_ultima_rafaga * kernel_config->alfa +
         ((1 - kernel_config->alfa) * proceso->estimacion);
}

// TODO: implementar
void actualizar_espera_listos() {

  //int cantidad_listos;
  //pcb_t *proceso;

  pthread_mutex_lock(&procesos_listos_mutex);

  /* cantidad_listos = list_size(cola_listos);
  for (int i = 0; i < cantidad_listos; i++) {
    proceso = list_get(cola_listos, i);
    //proceso_finalizar_espera(proceso);
  } */

  pthread_mutex_unlock(&procesos_listos_mutex);
}

int cantidad_procesos_nuevos() {

  int cantidad;

  pthread_mutex_lock(&procesos_nuevos_mutex);
  cantidad = list_size(cola_nuevos);
  pthread_mutex_unlock(&procesos_nuevos_mutex);

  return cantidad;
}

int cantidad_procesos_listos() {

  int cantidad;

  pthread_mutex_lock(&procesos_listos_mutex);
  cantidad = list_size(cola_listos);
  pthread_mutex_unlock(&procesos_listos_mutex);

  return cantidad;
}

int cantidad_procesos_ejecutando() {

  int cantidad;

  pthread_mutex_lock(&procesos_ejecutando_mutex);
  cantidad = list_size(cola_ejecucion);
  pthread_mutex_unlock(&procesos_ejecutando_mutex);

  return cantidad;
}

int cantidad_procesos_bloqueados() {

  int cantidad;

  pthread_mutex_lock(&procesos_bloqueados_mutex);
  cantidad = list_size(cola_bloqueados);
  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  return cantidad;
}

int cantidad_procesos_bloqueados_suspendidos() {

  int cantidad;

  pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);
  cantidad = list_size(cola_bloqueados_suspendidos);
  pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

  return cantidad;
}

int cantidad_procesos_terminados() {

  int cantidad;

  pthread_mutex_lock(&procesos_terminados_mutex);
  cantidad = list_size(cola_terminados);
  pthread_mutex_unlock(&procesos_terminados_mutex);

  return cantidad;
}

/* bool mover_proceso_en_ejecucion_a_bloqueado(int pid) {

	bool pid_iguales(pcb_t * proceso){
		return proceso->pid == pid;
	}

  pthread_mutex_lock(&procesos_ejecutando_mutex);

  pcb_t *proceso_en_ejecucion = list_find(cola_ejecucion, (void *)pid_iguales);

  pthread_mutex_unlock(&procesos_ejecutando_mutex);

  desencolar_proceso_en_ejecucion(proceso_en_ejecucion);

  if (proceso_en_ejecucion == NULL) {
    error_log("monitor_colas_procesos.c@mover_proceso_en_ejecucion_a_bloqueado", "Error al sacar proceso de cola de ejecucion");
    return false;
  }

  encolar_proceso_en_bloqueados(proceso_en_ejecucion);
  
  return true;
} */

void procesos_lock() {
  pthread_mutex_lock(&procesos_mutex);
}

void procesos_unlock() {
  pthread_mutex_unlock(&procesos_mutex);
}

int get_cantidad_procesos() {
  return procesos->elements_count;
}

t_list_iterator *procesos_get_iterator() {
  return list_iterator_create(procesos);
}

t_list *get_procesos() {
  return procesos;
}

t_list *get_procesos_bloqueados() {
  bool esta_bloqueado(pcb_t *proc) {
    return proc->estado == ESTADO_PROCESO_BLOCKED;
  }

  if (list_size(procesos) < 1) {
    return NULL;
  }
  
  return list_filter(procesos, (void *)esta_bloqueado);
}

pcb_t *desencolar_proceso_bloqueado_IO(pcb_t* proceso) {

  bool buscar_por_pid(pcb_t * process) {
      return process->pid == proceso->pid;
  }

  pthread_mutex_lock(&procesos_bloqueados_mutex);

  pcb_t *proceso_desencolado = list_remove_by_condition(cola_bloqueados, (void*)buscar_por_pid);

  pthread_mutex_unlock(&procesos_bloqueados_mutex);

  char *mensaje = string_from_format(
      "Proceso pid: %d desencolado de bloqueados", proceso_desencolado->pid);
  debug_log("monitor_colas_procesos.c@desencolar_proceso_en_bloqueados", mensaje);
  free(mensaje);

  return proceso_desencolado;
}

pcb_t *desencolar_proceso_bloqueado_suspendido_IO(pcb_t* proceso) {

  debug_log("monitor_colas_procesos.c@desencolar_proceso_bloqueado_suspendido_IO",
	        "Obteniendo un proceso de la cola bloqueados_suspendidos");

  bool buscar_por_pid(pcb_t * process) {
      return process->pid == proceso->pid;
  }

  pthread_mutex_lock(&procesos_bloqueados_suspendidos_mutex);
  pcb_t *proceso_desencolado = list_remove_by_condition(cola_bloqueados_suspendidos, (void*)buscar_por_pid);
  pthread_mutex_unlock(&procesos_bloqueados_suspendidos_mutex);

  return proceso_desencolado;
}
