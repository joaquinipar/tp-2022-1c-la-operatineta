#include "../../include/utils/logger.h"
#define TRUE 1

char *modulo = "DEFAULT_MODULE(cambiar con logger_set_module())";
char *logs_path = "./logs.log";

void logger_set_module(char *nombre_modulo) {
    modulo = nombre_modulo;
}

void info_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, LOG_LEVEL_DEBUG);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_info(logger, log);
  free(log);

  log_destroy(logger);
}

void error_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, LOG_LEVEL_DEBUG);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_error(logger, log);
  free(log);

  log_destroy(logger);
}

void debug_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, LOG_LEVEL_DEBUG);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_debug(logger, log);
  free(log);

  log_destroy(logger);
}

void warning_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, LOG_LEVEL_DEBUG);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_warning(logger, log);
  free(log);

  log_destroy(logger);
}

void trace_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, LOG_LEVEL_DEBUG);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_trace(logger, log);
  free(log);

  log_destroy(logger);
}
