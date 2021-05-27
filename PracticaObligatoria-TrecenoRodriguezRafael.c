/**
 *
 * Programa en C que realiza (dependiendo de lo que indique el usuario):
 * 	1) Indica el número total de caracteres
 * 	2) Indica el número total de filas
 * 	3) Indica Cuál es la fila más larga
 * 	4) Clona el archivo y crea una nueva línea al final
 *
 * @author TRECEÑO RODRIGUEZ, RAFAEL (rafa13o)
 * @date 13-05-2021 Tuesday
 * @version 2.0v
 */

/**
 * FOR ME: (notas personales)
 * cd VM/COMPARTIDA/PROGRA/PRACTICA
 * gcc -Wall -o PracticaObligatoria-TrecenoRodriguezRafael PracticaObligatoria-TrecenoRodriguezRafael.c
 * ./PracticaObligatoria-TrecenoRodriguezRafael
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LONG 254 // Posible longitud máxima de un string en este programa

/* Definición de estructuras*/
struct datosArchivo
{
	char comunidadAutonoma[23];
	char estacion[24];
	char mes[13];
	float tempMedia, mediaTempMax, mediaTempMin, diasLluvia, diasNieve, diasTempestad, diasNiebla, diasHelada, diasVacios;
	int precipitacionMensualMedia, humedadMedia, horasDeSol, altura;
};

/*Definición de funciones*/
int solicitarOpcionMenu();
void seleccionarOpcion(int);
int contarCaracteres(FILE *);
int numeroTotalFilas(FILE *);
int filaMasLarga(FILE *);
void limpiarBuffer();
void solicitarDatos();
bool validarEntero(int, int, int, int, char);
bool validaReal(float, int, float, int, char);
int menuTerritorios();
int menuEstaciones();
int menuMes();
bool clonarArchivo();
float temperaturaMedia(FILE *, char *, char *);
void temperaturaPrecipitacion(FILE *, int);
void crearEstructura(FILE *, struct datosArchivo *);

//--- Listas constantes ---
const char listadoComunidadesAutonomas[3][21] = {"Comunidad Valenciana", "Castilla y Leon", "Comunidad de Madrid"};
const char listadoEstaciones[20][22] = {"Valencia", "Elche aeropuerto", "Castellón", "Avila", "Burgos aeropuerto", "Leon",
										"Ponferrada", "Salamanca aeropuerto", "Segovia", "Soria", "Valladolid", "Valladolid aeropuerto", "Zamora",
										"Colmenar Viejo", "Getafe", "Madrid Cuatro Vientos", "Madrid Retiro", "Madrid Aeropuerto", "Puerto de Navacerrada",
										"Torrejon de Ardoz"};
const char listadoMeses[12][11] = {"Enero", "Febero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
const int listaTamanosMinimos[] = {-20, -20, -20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int listaTamanosMaximos[] = {50, 50, 50, 300, 100, 366, 366, 20, 30, 50, 50, 250, 2000};
const char nombreFicheroInicial[] = "ValoresClimatologicosFINAL.csv";
const char nombreFicheroFinal[] = "ValoresClimatologicosFINAL_V2.csv";

int main()
{

	int opcion; // Opcion que ha escogido el usuario del menú
	bool repetir = true;

	while (repetir)
	{
		opcion = solicitarOpcionMenu();
		seleccionarOpcion(opcion);
		if (opcion == 0)
		{
			repetir = false;
		}
	}

	return 0;
}

/**
 * Muestra el menú y solicita al usuario una de las opciones.
 * El menú se repetirá hasta que el usuario introduzca una opción válida.
 * 
 * NOTA: recogo un char porque, al recoger un int, si el usuario introducía
 * una letra, se producía un bucle infinito.
 * 
 * @return la opción que haya escogido el usuario del menú
 */
int solicitarOpcionMenu()
{

	int opcion;
	char charOpcion;
	bool opcionCorrecta = false;

	do
	{
		printf("\n\n************************************************************************");
		printf("\n\t1.- Número total de caracteres (contando espacios y NO saltos de línea)");
		printf("\n\t2.- Número total de filas");
		printf("\n\t3.- Fila más larga");
		printf("\n\t4.- Añadir una nueva fila");
		printf("\n\t5.- Temperatura media del mes de mayo en Castilla y León");
		printf("\n\t6.- Crear archivos con temper. [15,30] y con precip. [20,40]");
		printf("\n\t0.- Salir");
		printf("\n**************************************************************************");
		printf("\n ---> ");
		scanf("%c%*[^\n]", &charOpcion); // Para ignorar el \n

		if (charOpcion >= 48 && charOpcion <= 54) // 48(Ascii)=0 y 54(Ascii)=6
		{
			opcionCorrecta = true;
		}
		else
		{
			printf("¡¡¡ Esa opción no está en el menú !!!");
		}
		limpiarBuffer();
	} while (!opcionCorrecta);

	opcion = charOpcion - '0';

	return opcion;
}

/**
 * Llama a las funciones que ejecutan cada una de las opciones
 * del menú mediante un switch.
 * Además, esta función abre y cierra el fichero y lo pasa a las
 * demás funciones ya abierto.
 * 
 * @param opcion la opción que eligió el usuario del menú
 */
void seleccionarOpcion(int opcion)
{
	FILE *ficheroTemperaturas; // El fichero que leo
	int numeroFilas, numeroDeFilaMasLarga, numeroDeCaracteresTotales, estadoFicheroCerrado;

	switch (opcion)
	{
	case 1: // Total caracteres del fichero
		ficheroTemperaturas = fopen((char *)nombreFicheroInicial, "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroDeCaracteresTotales = contarCaracteres(ficheroTemperaturas);
		printf("Número total de caracteres: %d\n\n", numeroDeCaracteresTotales);
		estadoFicheroCerrado = fclose(ficheroTemperaturas);
		if (estadoFicheroCerrado == 0) // El fichero se ha cerrado de forma correcta
		{
			printf("\n*****\nEl fichero se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		else
		{
			printf("\n*****\nEl fichero no se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}

		break;
	case 2: // Número total de filas que hay en el fichero
		ficheroTemperaturas = fopen((char *)nombreFicheroInicial, "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroFilas = numeroTotalFilas(ficheroTemperaturas);
		printf("Número total de filas: %d\n\n", numeroFilas);
		estadoFicheroCerrado = fclose(ficheroTemperaturas);
		if (estadoFicheroCerrado == 0) // El fichero se ha cerrado de forma correcta
		{
			printf("\n*****\nEl fichero se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		else
		{
			printf("\n*****\nEl fichero no se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}

		break;
	case 3: // Fila más larga del fichero
		ficheroTemperaturas = fopen((char *)nombreFicheroInicial, "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroDeFilaMasLarga = filaMasLarga(ficheroTemperaturas);
		printf("La fila más larga es la fila %d\n\n", numeroDeFilaMasLarga);
		estadoFicheroCerrado = fclose(ficheroTemperaturas);
		if (estadoFicheroCerrado == 0) // El fichero se ha cerrado de forma correcta
		{
			printf("\n*****\nEl fichero se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		else
		{
			printf("\n*****\nEl fichero no se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}

		break;
	case 4:
		solicitarDatos();
		break;
	case 5:
		ficheroTemperaturas = fopen((char *)nombreFicheroInicial, "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		// Castilla y león es la comunidad 1 del listado. Mayo es el mes 4 del listado (posición)
		float media = temperaturaMedia(ficheroTemperaturas, (char *)listadoComunidadesAutonomas[1], (char *)listadoMeses[4]);
		printf("Temperatura media de las estaciones meteorológicas de CASTILLA Y LEON en el mes de MAYO es: %.1f", media);
		estadoFicheroCerrado = fclose(ficheroTemperaturas);
		if (estadoFicheroCerrado == 0) // El fichero se ha cerrado de forma correcta
		{
			printf("\n*****\nEl fichero se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		else
		{
			printf("\n*****\nEl fichero no se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		break;
	case 6:
		ficheroTemperaturas = fopen((char *)nombreFicheroInicial, "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		temperaturaPrecipitacion(ficheroTemperaturas, 253);
		estadoFicheroCerrado = fclose(ficheroTemperaturas);
		if (estadoFicheroCerrado == 0) // El fichero se ha cerrado de forma correcta
		{
			printf("\n*****\nEl fichero se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		else
		{
			printf("\n*****\nEl fichero no se ha cerrado de forma correcta tras su lectura.\n*****\n");
		}
		break;
	default:
		printf("Saliendo...\n\n\n");
	}
}

/**
 * Cuenta los caracteres del fichero que le llega por parámetro
 * Por cada caracter que cuenta va comprobando si es un salto de línea o si es
 * el final del archivo. Si no es ninguna de ellas, incrementa el contador en 
 * 1. Si el caracter es alguna de ellas, continúa y recoge el siguiente caracter.
 * 
 * @param fichero el fichero del que se quieren contar los caracteres
 * @return el número de caracteres del fichero (sin contar saltos de línea)
 */
int contarCaracteres(FILE *fichero)
{
	char caracterLeido; // Caracter leído en cada iteración
	int contador = 0;

	while (!feof(fichero)) // Hasta el final del bucle
	{
		caracterLeido = fgetc(fichero);
		if (caracterLeido != '\n' && caracterLeido != '\r' && caracterLeido != EOF)
		{
			contador++;
		}
	}

	return contador;
}

/**
 * Cuenta el número total de filas del fichero.
 * Lo que hace es recorrer el fichero e ir contando el caracter de salto de 
 * línea '\n' hasta que llega al final del fichero.
 * Al final, le resta uno ya que la primera fila (la de las cabeceras) no
 * se contabiliza.
 * 
 * @param fichero el fichero del que se quiere saber el número de líneas
 * @return el número de filas que hay menos la fila de cabeceras (total-1)
 */
int numeroTotalFilas(FILE *fichero)
{
	char caracterLeido; // El caracter que se lee en cada iteración del while
	int contador = 1;	// Comienzo a contar en 1

	while (!feof(fichero)) // Hasta el final del bucle
	{
		caracterLeido = fgetc(fichero);

		if (caracterLeido == '\n')
		{
			contador++;
		}
	}

	contador--; // Le quito las cabeceras

	return contador;
}

/**
 * Función que devuelve el nñumero de la fila más larga.
 * Va línea por línea contabilizando los caracteres y, una vez que llega a un 
 * salto de línea, comprueba si el número de caracteres que acaba de leer es 
 * mayor que el número de caracteres que tenía la fila que antes era la mayor.
 * Si es mayor, actualiza los datos. Si es menor, continúa.
 * 
 * @param fichero archivo del que se quiere saber cuál es la fila mayor
 * @return el número de la fila más larga
 */
int filaMasLarga(FILE *fichero)
{
	/**
	 * numeroDeFila: número de fila actual
	 * numeroDeFilaMaximo: número de fila que más caracteres tiene.
	 * numeroMaximoDeCaracteres: número de caracteres que tiene la fila con más 
	 * 	caracteres.
	 * numeroDeCaracteres: número de caracteres que tiene la fila que se acaba de leer.
	 */
	int numeroDeFila, numeroDeFilaMaximo, numeroMaximoDeCaracteres, numeroDeCaracteres;
	char caracterLeido;

	numeroDeFilaMaximo = 0;
	numeroMaximoDeCaracteres = 0;

	numeroDeFila = 1;
	numeroDeCaracteres = 0;

	while (!feof(fichero))
	{ // Comienzo a contar las filas
		caracterLeido = fgetc(fichero);
		if (caracterLeido != '\n' || caracterLeido == EOF) // No es final de línea
			numeroDeCaracteres++;
		else
		{
			if (numeroMaximoDeCaracteres < numeroDeCaracteres)
			{
				numeroMaximoDeCaracteres = numeroDeCaracteres;
				numeroDeFilaMaximo = numeroDeFila;
			}
			numeroDeCaracteres = 0;
			numeroDeFila++; // Cambio de fila
		}
	}

	return numeroDeFilaMaximo;
}

/**
 * Función que va pidiendo al usuario los datos de la nueva línea.
 * Primero pregunta por la comunidad, estación y mes a través de un menú.
 * Luego, pregunta por cada dato y el usuario los introduce por teclado.
 * Llama a las funciones validarEntero() y validaReal() para ver si el dato 
 * 		introducido es correcto. Si es correcto, pide el siguiente dato.
 * 		Si no lo es, vuelve a pedir el mismo dato.
 * Al final, llama a clonarArchivo() para clonar el archivo y, si se ha 
 * 		realizado bien, se añade la nueva línea al final del archivo.
 */
void solicitarDatos()
{
	bool datoCorrecto = false; // Si el dato introducido por el usuario es correcto, cambiará su valor a true
	int datoActual = 0;		   // Dato que tengo que pedir y comprobar

	/**
	 * opcionTerritorio = número del territorio que ha seleccionado el usuario (el índice en el array)
	 * opcionEstacion = número de la estación que ha seleccionado el usuario (el índice en el array)
	 * opcionMes = número del mes que ha seleccionado el usuario (el índice en el array)
	 * cantidadDatosLeidos = cantidad de datos que lee la función scanf()
	 * datoEnteroLeido = dato entero que ha introducido el usuario
	 */
	int opcionTerritorio, opcionEstacion, opcionMes, cantidadDatosLeidos, datoEnteroLeido;
	float datoRealLeido;					  // Dato real que ha introducido el usuario
	char *comunidadAutonoma, *estacion, *mes; // Strings
	char enter;								  // Para comprobar si el usuario ha introducido un enter

	/**
	 * tempMedia = temperatura media mensual/anual (0)
	 * mediaTempMax = media mensual/anual de las temperaturas máximas diarias (1)
	 * mediaTempMin = media mensual/anual de las temperaturas mínimas diarias (2)
	 * diasLluvia = media mensual/anual de días de precipitación superior o igual a 1 mm (5)
	 * diasNieve = media mensual/anual de días de nieve (6)
	 * diasTempestad = media mensual/anual de días de tempestad (7)
	 * diasNiebla = media mensual/anual de días de niebla (8)
	 * diasHelada = media mensual/anual de días de helada (9)
	 * diasVacios = media mensual/anual de días con datos "vacíos" (10)
	 */
	float tempMedia, mediaTempMax, mediaTempMin, diasLluvia, diasNieve, diasTempestad, diasNiebla, diasHelada, diasVacios;

	/**
	 * precipitacionMensualMedia = precipitación mensual/anual media (mm) (3)
	 * humedadMedia = humedad relativa media (%) (4)
	 * horasDeSol = media mensual/anual de horas de sol (11)
	 * altura = altura (en m) de la estación
	 */
	int precipitacionMensualMedia, humedadMedia, horasDeSol, altura;

	// Elección de comunidad autónoma
	opcionTerritorio = menuTerritorios();
	comunidadAutonoma = (char *)listadoComunidadesAutonomas[opcionTerritorio];

	// Elección de estación
	opcionEstacion = menuEstaciones(opcionTerritorio);
	estacion = (char *)listadoEstaciones[opcionEstacion];

	//Elección del mes
	opcionMes = menuMes();
	mes = (char *)listadoMeses[opcionMes];

	while (datoActual < 13) // Para comprobar todo
	{
		/** 
		 * Lo que voy haciendo es recoger los datos en datoRealLeido (para los float) 
		 * y datoEnteroLeido (para los int)
		 * El dato leído lo guardo en la variable correspondiente y compruebo 
		 * que el dato sea correcto. Si no es correcto, volverá a pedirlo y
		 * se repetirá el proceso. Si es correcto, sumará uno a datoActual para
		 * pedir el siguiente dato y repetir el mismo proceso.
		 */
		switch (datoActual)
		{
		case 0: //tempMedia
			printf("Por favor, dame un valor para la TEMPERATURA MEDIA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			tempMedia = datoRealLeido;
			break;
		case 1: // mediaTempMax
			printf("Por favor, dame un valor para la MEDIA DE LAS TEMPERATURAS MÁXIMAS [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			mediaTempMax = datoRealLeido;
			break;
		case 2: // mediaTempMin
			printf("Por favor, dame un valor para la MEDIA DE LAS TEMPERATURAS MÍNIMAS [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			mediaTempMin = datoRealLeido;
			break;
		case 3: // precipitacionMensualMedia
			printf("Por favor, dame un valor para la MEDIA DE LAS PRECIPITACIONES [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			precipitacionMensualMedia = datoEnteroLeido;
			break;
		case 4: // humedadMedia
			printf("Por favor, dame un valor para la MEDIA DE LA HUMEDAD RELATIVA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			humedadMedia = datoEnteroLeido;
			break;
		case 5: // diasLluvia
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE LLUVIA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasLluvia = datoRealLeido;
			break;
		case 6: // diasNieve
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE NIEVE [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasNieve = datoRealLeido;
			break;
		case 7: // diasTempestad
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE TEMPESTAD [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasTempestad = datoRealLeido;
			break;
		case 8: // diasNiebla
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE NIEBLA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasNiebla = datoRealLeido;
			break;
		case 9: // diasHelada
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE HELADA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasHelada = datoRealLeido;
			break;
		case 10: // diasVacios
			printf("Por favor, dame un valor para la MEDIA DE DÍAS SIN DATOS [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasVacios = datoRealLeido;
			break;
		case 11: // horasDeSol
			printf("Por favor, dame un valor para la MEDIA DE HORAS DE SOL [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			horasDeSol = datoEnteroLeido;
			break;
		case 12: // altura
			printf("Por favor, dame un valor para la ALTURA [%d,%d]: --> ", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			altura = datoEnteroLeido;
			break;
		default:
			break;
		}

		if (datoActual == 3 || datoActual == 4 || datoActual == 11 || datoActual == 12) // Datos de tipo entero
		{
			datoCorrecto = validarEntero(datoEnteroLeido, listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual], cantidadDatosLeidos, enter);
		}
		else // Datos de tipo float
		{
			datoCorrecto = validaReal(datoRealLeido, listaTamanosMinimos[datoActual], (float)listaTamanosMaximos[datoActual], cantidadDatosLeidos, enter);
		}

		if (datoCorrecto) // El dato introducido por el usuario es correcto
			datoActual++;
		else // El dato introducido por el usuario es incorrecto
			printf("--- ¡¡El dato es incorrecto!!. Recuerde que el dato debe estar entre %d y %d.\n", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
	}

	if (clonarArchivo()) //Si el archivo inicial se ha abierto, clonado y cerrado correctamente, escribo la nueva línea
	{
		FILE *archivo = fopen((char *)nombreFicheroFinal, "a+"); // Abro el fichero en modo de agregar al final

		if (archivo == NULL) // Compruebo que el archivo se ha abierto correctamente
		{
			printf("Hubo un problema al abrir el fuchero de salida");
		}
		else
		{
			fprintf(archivo, "%s,%s,%d,%s,%.1f,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%d\n", comunidadAutonoma, estacion, altura, mes, tempMedia, mediaTempMax, mediaTempMin, precipitacionMensualMedia, humedadMedia, diasLluvia, diasNieve, diasTempestad, diasNiebla, diasHelada, diasVacios, horasDeSol);
		}

		if (fclose(archivo) != 0) // Compruebo si el archivo no se ha cerrado correctamente
		{
			printf("Hubo un problema al cerrar el fichero de salida");
		}
	}
}

/**
 * Realiza la clonación del archivo de entrada al archivo de salida
 * 
 * @return true si lo ha podido hacer correctamente (apertura, clonación, cierre). False en caso contrario.
 */
bool clonarArchivo()
{
	char caracterLeido;
	int estadoFicheroEntrada, estadoFicheroSalida; // Almacenan el retorno del estado en que se han cerrado. Si los archivos se cerraron bien serán 0.

	FILE *archivoACopiar = fopen((char *)nombreFicheroInicial, "r");
	FILE *ficheroClonado = fopen((char *)nombreFicheroFinal, "a+");

	if (archivoACopiar == NULL) // Compruebo que el archivo se ha abierto correctamente
	{
		printf("Hubo un problema al abrir el fichero de entrada");
		return false;
	}

	if (archivoACopiar == NULL) // Compruebo que el archivo se ha abierto correctamente
	{
		printf("Hubo un problema al abrir el fuchero de salida");
		return false;
	}

	while ((caracterLeido = fgetc(archivoACopiar)) != EOF) // Copio todos los valores menos el EOF
	{
		fputc(caracterLeido, ficheroClonado);
	}

	estadoFicheroEntrada = fclose(archivoACopiar);
	estadoFicheroSalida = fclose(ficheroClonado);

	if (estadoFicheroEntrada != 0) // Compruebo si ha ocurrido algún error (resultado distinto de 0)
	{
		printf("Hubo un problema al cerrar el fichero de entrada");
		return false;
	}

	if (estadoFicheroSalida != 0) // Compruebo si ha ocurrido algún error (resultado distinto de 0)
	{
		printf("Hubo un problema al cerrar el fichero de salida");
		return false;
	}

	return true;
}

/**
 * Función que comprueba si el valor entero introducido por el usuario es correcto.
 * El valor deberá estar entre los rangos tamanoMinimo y tamanoMaximo.
 * Devuelve true si el valor es correcto (cumple las condiciones). false en caso contrario.
 * 
 * @param numeroLeido el número que ha introducido el usuario
 * @param tamanoMinimo el rango mínimo de ese dato
 * @param tamanoMaximo el rango máximo de ese dato
 * @param parametrosLeidos el número de parámetros que ha leído la función scanf()
 * @param enter saber si el usuario ha introducido un enter al final
 * @return true si el valor cumple las condiciones. False en caso contrario.
 */
bool validarEntero(int numeroLeido, int tamanoMinimo, int tamanoMaximo, int parametrosLeidos, char enter)
{
	bool datoCorrecto = false;

	// Primero compruebo que el scanf() solo devuelva dos valores (el valor del dato y el intro)
	if (parametrosLeidos == 2)
	{
		// Compruebo que el enter sea verdaderamente un enter
		if (enter == '\n')
			if (numeroLeido >= tamanoMinimo && numeroLeido <= tamanoMaximo)
			{
				datoCorrecto = true;
			}
	}

	printf("     Pulse intro para continuar");
	limpiarBuffer();
	return datoCorrecto;
}

/**
 * Función que comprueba si el valor float introducido por el usuario es correcto.
 * El valor deberá estar entre los rangos tamanoMinimo y tamanoMaximo.
 * Devuelve true si el valor es correcto (cumple las condiciones). false en caso contrario.
 * 
 * @param numeroLeido el número que ha introducido el usuario
 * @param tamanoMinimo el rango mínimo de ese dato
 * @param tamanoMaximo el rango máximo de ese dato
 * @param parametrosLeidos el número de parámetros que ha leído la función scanf()
 * @param enter saber si el usuario ha introducido un enter al final
 * @return true si el valor cumple las condiciones. False en caso contrario.
 */
bool validaReal(float numeroLeido, int tamanoMinimo, float tamanoMaximo, int parametrosLeidos, char enter)
{

	bool datoCorrecto = false;

	// Primero compruebo que el scanf() solo devuelva dos valores (el valor del dato y el intro)
	if (parametrosLeidos == 2.0f)
		// Compruebo que el enter sea verdaderamente un enter
		if (enter == '\n')
			if (numeroLeido >= tamanoMinimo && numeroLeido <= tamanoMaximo)
				datoCorrecto = true;

	printf("     Pulse intro para continuar");
	limpiarBuffer();
	return datoCorrecto;
}

/**
 * Imprime un menú con las comunidades autónomas y comprueba que el dato introducido sea correcto.
 *
 * @return el índice en el que se encuentra la comunidad en el array.
 */
int menuTerritorios()
{
	int opcionTerritorio, contador;
	bool opcionCorrecta = false;

	printf("Por favor, seleccione uno de la lista:\n");
	for (int i = 0; i < 3; i++)
	{
		contador = i + 1;
		printf("\t%d.- %s\n", contador, listadoComunidadesAutonomas[i]);
	}

	while (!opcionCorrecta) // Comprueba que la opción introducida por el usuario sea correcta
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionTerritorio);
		limpiarBuffer();
		if (opcionTerritorio >= 1 && opcionTerritorio <= 3)
		{
			opcionCorrecta = true;
		}
	}

	opcionTerritorio--; // Le resto uno porque el usuario cuenta de 1 a 3, pero el array va de 0 a 2

	return opcionTerritorio;
}

/**
 * Imprime un menú con las estaciones y comprueba que el dato introducido sea correcto.
 * 
 * @param territorioSeleccionado el territorio que seleccionó el usuario (ya que las 
 * 		estaciones deben ir acordes al territorio). Así, solo imprimo las estaciones
 * 		del territorio seleccionado.
 * @return el índice en el que se encuentra la estación en el array
 */
int menuEstaciones(int territorioSeleccionado)
{
	/**
	 * sumar = cantidad a sumar para que de el resultado correcto (posición del array)
	 */
	int opcionEstacion, contador = 1, sumar;
	bool opcionCorrecta = false;
	int opcionMaxima; // Opción máxima que el usuario puede seleccionar de la lista que se le muestra en cada caso

	printf("Por favor, seleccione uno de la lista:\n");

	if (territorioSeleccionado == 0) // Comunidad Valenciana
	{
		sumar = 0;
		opcionMaxima = 3;
		for (int i = 0; i < 3; i++)
		{
			printf("\t%d.- %s\n", contador, listadoEstaciones[i]);
			contador++;
		}
	}
	else if (territorioSeleccionado == 1) // Castilla y León
	{
		sumar = 3;
		opcionMaxima = 10;
		for (int i = 3; i < 13; i++)
		{
			printf("\t%d.- %s\n", contador, listadoEstaciones[i]);
			contador++;
		}
	}
	else // Comunidad de Madrid
	{
		sumar = 13;
		opcionMaxima = 7;
		for (int i = 13; i < 20; i++)
		{
			printf("\t%d.- %s\n", contador, listadoEstaciones[i]);
			contador++;
		}
	}

	while (!opcionCorrecta) // Comprueba que la opción introducida por el usuario sea correcta
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionEstacion);
		limpiarBuffer();
		if (opcionEstacion >= 1 && opcionEstacion <= opcionMaxima)
		{
			opcionCorrecta = true;
		}
	}

	opcionEstacion += sumar; // Cantidad que hay que sumar al número para que dé el índice
	opcionEstacion--;		 // Para que dé el índice

	return opcionEstacion;
}

/**
 * Imprime un menú con los meses y comprueba que el dato introducido sea correcto.
 * 
 * @return el índice en el que se encuentra la estación en el array
 */
int menuMes()
{
	int opcionMes, contador;
	bool opcionCorrecta = false;

	printf("Por favor, seleccione uno de la lista:\n");
	for (int i = 0; i < 12; i++)
	{
		contador = i + 1;
		printf("\t%d.- %s\n", contador, listadoMeses[i]);
	}

	while (!opcionCorrecta) // Comprueba que la opción introducida por el usuario sea correcta
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionMes);
		limpiarBuffer();
		if (opcionMes >= 1 && opcionMes <= 12)
		{
			opcionCorrecta = true;
		}
	}

	opcionMes--; // Para que dé el índice

	return opcionMes;
}

/**
 * Lee el fihcero que se pasa por parámetro, crea una estructura de tipo datosArchivo 
 * con los datos que hay en él y luego los guarda en un array de estructuras de tipo datoArchivo
 * Luego, recorre el array de estructuras y va cogiendo los valores de comunidadAutonoma y del mes
 * y comprueba con strcmp(). Si la comparación es correcta (resultado=0), hace la suma del valor
 * al general y suma al contador de datos recogidos.
 * Cuando finaliza, realiza la media y la devuelve para ser imprimida por pantalla
 * 
 * @param fichero fichero desde el que se va a filtrar
 * @param comunidadAutonoma comunidad autónoma por la que se quiere filtrar
 * @param mes mes por el que se va a filtrar
 * @return la media de las temperaturas de las estaciones de CASTILLA Y LEON en el mes de MAYO
 */
float temperaturaMedia(FILE *fichero, char *comunidadAutonoma, char *mes)
{
	int filas = 253;
	struct datosArchivo listadoDatos[filas];

	crearEstructura(fichero, listadoDatos); // Función para crear la estructura

	//Calcular medias
	float sumaTemperaturas = 0;
	int contadorDatos = 0;
	for (int i = 0; i < 252; i++)
	{
		struct datosArchivo datosRecogidos = listadoDatos[i];
		if (strcmp(datosRecogidos.comunidadAutonoma, comunidadAutonoma) == 0 && strcmp(datosRecogidos.mes, mes) == 0)
		{
			contadorDatos++;
			sumaTemperaturas += datosRecogidos.tempMedia;
		}
	}

	float media = sumaTemperaturas / contadorDatos;

	return media;
}

/**
 * Función que recibe un fichero abierto por parámetro, crea una
 * estructura de tipo datosArchivo con él 
 * 
 * @param fichero el fichero del que tiene que recoger los datos.
 * @param numeroLineas el número de lineas que tiene el fichero original.
 */
void temperaturaPrecipitacion(FILE *fichero, int numeroLineas)
{
	struct datosArchivo datosRecogidos;
	struct datosArchivo nuevosDatos[numeroLineas]; // Datos para el nuevo archivo
	struct datosArchivo listadoDatos[numeroLineas];

	crearEstructura(fichero, listadoDatos); // Función para crear la estructura


	int contadorDatosGuardados = 0;
	for (int i = 0; i < numeroLineas; i++)
	{
		datosRecogidos = listadoDatos[i];

		if ((datosRecogidos.tempMedia >= 15 && datosRecogidos.tempMedia <= 30) && (datosRecogidos.precipitacionMensualMedia >= 20 && datosRecogidos.precipitacionMensualMedia <= 40))
		{
			nuevosDatos[contadorDatosGuardados] = datosRecogidos;
			contadorDatosGuardados++;
		}
	}
}

/**
 * Función que recibe por parámetro un archivo y un array de tipo struct datosArchivo.
 * Lee el archivo, separa los datos, los almacena cada uno en la estructura y, a su vez,
 * los almacena en el array de estructuras.
 * 
 * @param fichero fichero del que lee los datos para crear la estructura.
 * @param listadoDatos array donde se guardarán los datos.
 */
void crearEstructura(FILE *fichero, struct datosArchivo *listadoDatos){

	char filaTitulos[70];

	char enter;
	int numeroDeFila = 0, posicion;

	while (!feof(fichero))
	{
		posicion = numeroDeFila - 1;
		if (numeroDeFila == 0)
			fgets(filaTitulos, 70, fichero);
		else
		{
			fscanf(fichero, "%c%[^,],%[^,],%d,%[^,],%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f,%d%c",
				   &enter, listadoDatos[posicion].comunidadAutonoma, listadoDatos[posicion].estacion, &listadoDatos[posicion].altura, listadoDatos[posicion].mes, &listadoDatos[posicion].tempMedia,
				   &listadoDatos[posicion].mediaTempMax, &listadoDatos[posicion].mediaTempMin, &listadoDatos[posicion].precipitacionMensualMedia, &listadoDatos[posicion].humedadMedia,
				   &listadoDatos[posicion].diasLluvia, &listadoDatos[posicion].diasNieve, &listadoDatos[posicion].diasTempestad, &listadoDatos[posicion].diasNiebla,
				   &listadoDatos[posicion].diasHelada, &listadoDatos[posicion].diasVacios, &listadoDatos[posicion].horasDeSol, &enter);
		}
		numeroDeFila++;
	}
}

/**
 * Función para limpiar el buffer
 */
void limpiarBuffer()
{
	while (getchar() != '\n')
	{
	}
}

//------ BIBLIOGRAFÍA ------
// https://www.microchip.com/forums/m955412.aspx
// https://www.microchip.com/forums/m719672.aspx