/**
 *
 * Programa en C que indica (dependiendo de lo que indique el usuario):
 * 	1) número total de caracteres
 * 	2) Número total de filas
 * 	3) Cuál es la fila más larga
 *
 * @author TRECEÑO RODRIGUEZ, RAFAEL (rafa13o)
 * @date 27-04-2021 Tuesday
 * @version 1.1
 */

/**
 * FOR ME: (notas personales)
 * cd cd VM/COMPARTIDA/PROGRA/PRACTICA
 * gcc -Wall -o PracticaObligatoria-TrecenoRodriguezRafael PracticaObligatoria-TrecenoRodriguezRafael.c
 * ./PracticaObligatoria-TrecenoRodriguezRafael
 */
#include <stdio.h>
#include <stdbool.h>

/*Definición de funciones*/
int solicitarOpcionMenu();
void seleccionarOpcion(int);
int contarCaracteres(FILE *);
int numeroTotalFilas(FILE *);
int filaMasLarga(FILE *);
void limpiarBuffer();
void solicitarDatos();
bool validarEntero(int, int, int, int, char);
//bool validaReal(float, int, float, float, char);
int menuTerritorios();
int menuEstaciones();
int menuMes();

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
		printf("\n\t0.- Salir");
		printf("\n**************************************************************************");
		printf("\n ---> ");
		scanf("%c%*[^\n]", &charOpcion); // Para ignorar el \n

		if (charOpcion >= 48 && charOpcion <= 52) // 48(Ascii)=0 y 52(Ascii)=4
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
	int numeroFilas, numeroDeFilaMasLarga, numeroDeCaracteresTotales;

	switch (opcion)
	{
	case 1: // Total caracteres del fichero
		ficheroTemperaturas = fopen("ValoresClimatologicosFINAL.csv", "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroDeCaracteresTotales = contarCaracteres(ficheroTemperaturas);
		printf("Número total de caracteres: %d\n\n", numeroDeCaracteresTotales);
		fclose(ficheroTemperaturas);
		break;
	case 2: // Número total de filas que hay en el fichero
		ficheroTemperaturas = fopen("ValoresClimatologicosFINAL.csv", "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroFilas = numeroTotalFilas(ficheroTemperaturas);
		printf("Número total de filas: %d\n\n", numeroFilas);
		fclose(ficheroTemperaturas);
		break;
	case 3: // Fila más larga del fichero
		ficheroTemperaturas = fopen("ValoresClimatologicosFINAL.csv", "r");
		if (ficheroTemperaturas == NULL) // Compruebo que el fichero se ha abierto correctamente
		{
			printf("Error al abrir el fichero");
			break; // Finalizo ejecución
		}
		numeroDeFilaMasLarga = filaMasLarga(ficheroTemperaturas);
		printf("La fila más larga es la fila %d\n\n", numeroDeFilaMasLarga);
		fclose(ficheroTemperaturas);
		break;
	case 4:
		solicitarDatos();
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

void solicitarDatos()
{

	bool datoCorrecto = false;
	int datoAPedir = 0; // Dato que tengo que pedir y comprobar

	int opcionTerritorio, opcionEstacion, opcionMes;

	char *comunidadAutonoma, *estacion, *mes; // Strings
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

	char listadoComunidadesAutonomas[3][21] = {"Comunidad Valenciana", "Castilla y León", "Comunidad de Madrid"};
	char listadoEstaciones[20][22] = {"Valencia", "Elche aeropuerto", "Castellón", "Ávila", "Burgos aeropuerto", "León",
									  "Ponferrada", "Salamanca aeropuerto", "Segovia", "Soria", "Valladolid", "Valladolid aeropuerto", "Zamora",
									  "Colmenar Viejo", "Getafe", "Madrid Cuatro Vientos", "Madrid Retiro", "Madrid Aeropuerto", "Puerto de Navacerrada",
									  "Torrejón de Ardoz"};
	char listadoMeses[12][11] = {"Enero", "Febero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

	// Elección de comunidad autónoma
	opcionTerritorio = menuTerritorios();
	comunidadAutonoma = listadoComunidadesAutonomas[opcionTerritorio];

	// Elección de estación
	opcionEstacion = menuEstaciones();
	estacion = listadoEstaciones[opcionEstacion];

	//Elección del mes
	opcionMes = menuMes();
	mes = listadoMeses[opcionMes];

	printf("Ha seleccionado usted:\n\tCA: %d %s\n\tEstacion: %d %s\n\tMes: %d %s", opcionTerritorio, comunidadAutonoma, opcionEstacion, estacion, opcionMes, mes);

	return;

	while (datoAPedir < 12) // para comprobar todo
	{
		switch (datoAPedir)
		{
		case 0: //tempMedia
			break;
		case 1: // mediaTempMax
			break;
		default:
			break;
		}
		scanf("%d", &precipitacionMensualMedia);
		datoCorrecto = validarEntero(precipitacionMensualMedia, 0, 300, 0, '\n');
	}
}

int menuTerritorios()
{
	int opcionTerritorio, contador;
	bool opcionCorrecta = false;

	char listadoComunidadesAutonomas[3][21] = {"Comunidad Valenciana", "Castilla y León", "Comunidad de Madrid"};

	printf("Por favor, seleccione uno de la lista:\n");
	for (int i = 0; i < 3; i++)
	{
		contador = i + 1;
		printf("\t%d.- %s\n", contador, listadoComunidadesAutonomas[i]);
	}

	while (!opcionCorrecta)
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

int menuEstaciones()
{
	int opcionEstacion, contador;
	bool opcionCorrecta = false;

	char listadoEstaciones[20][22] = {"Valencia", "Elche aeropuerto", "Castellón", "Ávila", "Burgos aeropuerto", "León",
									  "Ponferrada", "Salamanca aeropuerto", "Segovia", "Soria", "Valladolid", "Valladolid aeropuerto", "Zamora",
									  "Colmenar Viejo", "Getafe", "Madrid Cuatro Vientos", "Madrid Retiro", "Madrid Aeropuerto", "Puerto de Navacerrada",
									  "Torrejón de Ardoz"};

	printf("Por favor, seleccione uno de la lista:\n");
	for (int i = 0; i < 20; i++)
	{
		contador = i + 1;
		printf("\t%d.- %s\n", contador, listadoEstaciones[i]);
	}

	while (!opcionCorrecta)
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionEstacion);
		limpiarBuffer();
		if (opcionEstacion >= 1 && opcionEstacion <= 20)
		{
			opcionCorrecta = true;
		}
	}

	opcionEstacion--; // Le resto uno porque el usuario cuenta de 1 a 3, pero el array va de 0 a 2

	return opcionEstacion;
}

int menuMes()
{
	int opcionMes, contador;
	bool opcionCorrecta = false;

	char listadoMeses[12][11] = {"Enero", "Febero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

	printf("Por favor, seleccione uno de la lista:\n");
	for (int i = 0; i < 12; i++)
	{
		contador = i + 1;
		printf("\t%d.- %s\n", contador, listadoMeses[i]);
	}

	while (!opcionCorrecta)
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionMes);
		limpiarBuffer();
		if (opcionMes >= 1 && opcionMes <= 12)
		{
			opcionCorrecta = true;
		}
	}

	opcionMes--; // Le resto uno porque el usuario cuenta de 1 a 3, pero el array va de 0 a 2

	return opcionMes;
}

bool validarEntero(int numeroLeido, int tamanoMinimo, int tamanoMaximo, int parametrosLeidos, char enter)
{
	return false;
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
