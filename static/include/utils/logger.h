#ifndef __OPERATINETA_LOGGER_H
#define __OPERATINETA_LOGGER_H

#include <commons/log.h>
#include <commons/string.h>
#include <stdlib.h>

void logger_set_module(char *modulo);
void logger_set_loglevel(t_log_level level);
void destruir_logger();
void debug_log(char *ubicacion, char *mensaje);
void info_log(char *ubicacion, char *mensaje);
void error_log(char *ubicacion, char *mensaje);
void warning_log(char *ubicacion, char *mensaje);
void trace_log(char *ubicacion, char *mensaje);
void format_info_log(char* ubicacion, char* format, ...);
void format_debug_log(char* ubicacion, char* format, ...);
void format_warning_log(char* ubicacion, char* format, ...);
void format_trace_log(char* ubicacion, char* format, ...);
void format_error_log(char* ubicacion, char* format, ...);

#endif