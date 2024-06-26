#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	//logger = log_create("tp0.log","cliente.logger",true, LOG_LEVEL_INFO);
	//agregado dentro de la funcion iniciar logger para poder indicar si hay error al crearlo
	log_info(logger,"Soy un log");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	//	OK, corregido

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	//config = config_create("../cliente.config"); 
	//config_create agregado al final del archivo para detectar errores al crearlo
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config,"CLAVE");
	// Loggeamos el valor de config
	log_info(logger,"Valor leido de config: %s",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	//OK, corregido

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete (dentro de la funcion paquete)
	paquete(conexion);
	printf(">> CLIENTE CERRADO");

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("cliente.log","log_create_cliente",true,LOG_LEVEL_INFO);
	if (nuevo_logger==NULL){
		perror("No se pudo crear el log.");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config==NULL){
		perror("No se pudo crear el config");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger,">> %s", leido); 

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"")!=0){
		free(leido);
		leido = readline("> ");
		log_info(logger,">> %s",leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();
	while(strcmp(leido,"")!=0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1); //string length + 1 (caract vacio)
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	//enviado de PAQUETE		
	enviar_paquete(paquete,conexion); 
	//el paquete se debe enviar dentro de la funcion

	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
