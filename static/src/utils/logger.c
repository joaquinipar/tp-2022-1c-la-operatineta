#include "../../include/utils/logger.h"
#define TRUE 1

char *modulo = "DEFAULT_MODULE(cambiar con logger_set_module())";
char *logs_path = "./logs.log";
t_log_level loglevel = LOG_LEVEL_DEBUG;

void logger_set_module(char *nombre_modulo) {
    modulo = nombre_modulo;
}

void logger_set_loglevel(t_log_level level) {
  loglevel = level;
};

void info_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, loglevel);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_info(logger, log);
  free(log);

  log_destroy(logger);
}

void error_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, loglevel);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_error(logger, log);
  free(log);

  log_destroy(logger);
}

void debug_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, loglevel);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_debug(logger, log);
  free(log);

  log_destroy(logger);
}

void warning_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, loglevel);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_warning(logger, log);
  free(log);

  log_destroy(logger);
}

void trace_log(char *ubicacion, char *mensaje) {
  t_log *logger = log_create(logs_path, modulo, TRUE, loglevel);

  char *log = string_from_format("%s: %s", ubicacion, mensaje);
  log_trace(logger, log);
  free(log);

  log_destroy(logger);
}

void format_info_log(char* ubicacion, char* format, ...) {
    char* formatted_string;
    va_list arguments;
    va_start(arguments, format);
    formatted_string = string_from_vformat(format, arguments);
    va_end(arguments);
    info_log(ubicacion, formatted_string);
    free(formatted_string);
}

void format_debug_log(char* ubicacion, char* format, ...) {
    char* formatted_string;
    va_list arguments;
    va_start(arguments, format);
    formatted_string = string_from_vformat(format, arguments);
    va_end(arguments);
    debug_log(ubicacion, formatted_string);
    free(formatted_string);
}

void format_warning_log(char* ubicacion, char* format, ...) {
    char* formatted_string;
    va_list arguments;
    va_start(arguments, format);
    formatted_string = string_from_vformat(format, arguments);
    va_end(arguments);
    warning_log(ubicacion, formatted_string);
    free(formatted_string);
}

void format_trace_log(char* ubicacion, char* format, ...) {
    char* formatted_string;
    va_list arguments;
    va_start(arguments, format);
    formatted_string = string_from_vformat(format, arguments);
    va_end(arguments);
    trace_log(ubicacion, formatted_string);
    free(formatted_string);
}

void format_error_log(char* ubicacion, char* format, ...) {
    char* formatted_string;
    va_list arguments;
    va_start(arguments, format);
    formatted_string = string_from_vformat(format, arguments);
    va_end(arguments);
    error_log(ubicacion, formatted_string);
    free(formatted_string);
}

