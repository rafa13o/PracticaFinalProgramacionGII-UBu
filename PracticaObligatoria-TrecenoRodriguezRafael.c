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
 * cd VM/COMPARTIDA/PROGRA/PRACTICA
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
bool validaReal(float, int, float, float, char);
int menuTerritorios();
int menuEstaciones();
int menuMes();
bool clonarArchivo();

//--- Listas constantes ---
const char listadoComunidadesAutonomas[3][21] = {"Comunidad Valenciana", "Castilla y León", "Comunidad de Madrid"};
const char listadoEstaciones[20][22] = {"Valencia", "Elche aeropuerto", "Castellón", "Ávila", "Burgos aeropuerto", "León",
										"Ponferrada", "Salamanca aeropuerto", "Segovia", "Soria", "Valladolid", "Valladolid aeropuerto", "Zamora",
										"Colmenar Viejo", "Getafe", "Madrid Cuatro Vientos", "Madrid Retiro", "Madrid Aeropuerto", "Puerto de Navacerrada",
										"Torrejón de Ardoz"};
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
		ficheroTemperaturas = fopen((char*)nombreFicheroInicial, "r");
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
		ficheroTemperaturas = fopen((char*)nombreFicheroInicial, "r");
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
		ficheroTemperaturas = fopen((char*)nombreFicheroInicial, "r");
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
	int datoActual = 0; // Dato que tengo que pedir y comprobar
	int opcionTerritorio, opcionEstacion, opcionMes, cantidadDatosLeidos, datoEnteroLeido;
	float datoRealLeido;
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

	//printf("Ha seleccionado usted:\n\tCA: %d %s\n\tEstacion: %d %s\n\tMes: %d %s\n\n", opcionTerritorio, comunidadAutonoma, opcionEstacion, estacion, opcionMes, mes);

	while (datoActual < 13) // para comprobar todo
	{
		/** Lo que voy haciendo es recoger los datos en datoRealLeido (para los float) 
		 * y datoEnteroLeido (para los int)
		 * El dato leído lo guardo en la variable correspondiente y compruebo 
		 * que el dato sea correcto. Si no es correcto, volverá a pedirlo y
		 * se repetirá el proceso. Si es correcto, sumará uno a datoActual para
		 * pedir el siguiente dato y repetir el mismo proceso.
		 */
		switch (datoActual)
		{
		case 0: //tempMedia
			printf("Por favor, dame un valor para la TEMPERATURA MEDIA: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			tempMedia = datoRealLeido;
			break;
		case 1: // mediaTempMax
			printf("Por favor, dame un valor para la MEDIA DE LAS TEMPERATURAS MÁXIMAS: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			mediaTempMax = datoRealLeido;
			break;
		case 2: // mediaTempMin
			printf("Por favor, dame un valor para la MEDIA DE LAS TEMPERATURAS MÍNIMAS: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			mediaTempMin = datoRealLeido;
			break;
		case 3: // precipitacionMensualMedia
			printf("Por favor, dame un valor para la MEDIA DE LAS PRECIPITACIONES: --> ");
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			precipitacionMensualMedia = datoEnteroLeido;
			break;
		case 4: // humedadMedia
			printf("Por favor, dame un valor para la MEDIA DE LA HUMEDAD RELATIVA: --> ");
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			humedadMedia = datoEnteroLeido;
			break;
		case 5: // diasLluvia
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE LLUVIA: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasLluvia = datoRealLeido;
			break;
		case 6: // diasNieve
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE NIEVE: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasNieve = datoRealLeido;
			break;
		case 7: // diasTempestad
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE TEMPESTAD: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasTempestad = datoRealLeido;
			break;
		case 8: // diasNiebla
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE NIEBLA: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasNiebla = datoRealLeido;
			break;
		case 9: // diasHelada
			printf("Por favor, dame un valor para la MEDIA DE DÍAS DE HELADA: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasHelada = datoRealLeido;
			break;
		case 10: // diasVacios
			printf("Por favor, dame un valor para la MEDIA DE DÍAS SIN DATOS: --> ");
			cantidadDatosLeidos = scanf("%f%c", &datoRealLeido, &enter);
			diasVacios = datoRealLeido;
			break;
		case 11: // horasDeSol
			printf("Por favor, dame un valor para la MEDIA DE HORAS DE SOL: --> ");
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			horasDeSol = datoEnteroLeido;
			break;
		case 12: // altura
			printf("Por favor, dame un valor para la ALTURA: --> ");
			cantidadDatosLeidos = scanf("%d%c", &datoEnteroLeido, &enter);
			altura = datoEnteroLeido;
			break;
		default:
			break;
		}

		if (datoActual == 3 || datoActual == 4 || datoActual == 11 || datoActual == 12)
		{
			datoCorrecto = validarEntero(datoEnteroLeido, listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual], cantidadDatosLeidos, enter);
		}
		else
		{
			datoCorrecto = validaReal(datoRealLeido, listaTamanosMinimos[datoActual], (float) listaTamanosMaximos[datoActual], (float) cantidadDatosLeidos, enter);
		}

		if (datoCorrecto)
			datoActual++;
		else
			printf("--- ¡¡El dato es incorrecto!!. Recuerde que el dato debe estar entre %d y %d.\n", listaTamanosMinimos[datoActual], listaTamanosMaximos[datoActual]);
	}

	if(clonarArchivo){ //Si el archivo inicial se ha abierto, clonado y cerrado correctamente, escribo la nuyeva línea
		
	}

	//-- DEBUG --
	printf("\nCA: %s", comunidadAutonoma);
	printf("\nEstación: %s", estacion);
	printf("\nAltura: %d", altura);
	printf("\nMes: %s", mes);
	printf("\nTemperatura Media: %.1f", tempMedia);
	printf("\nMedia temp. Maxima: %.1f", mediaTempMax);
	printf("\nMedia temp. Mínima: %.1f", mediaTempMin);
	printf("\nMedia precipitaciones: %d", precipitacionMensualMedia);
	printf("\nMedia humedad: %d", humedadMedia);
	printf("\nMedia lluvia: %.1f", diasLluvia);
	printf("\nMedia nieve: %.1f", diasNieve);
	printf("\nMedia tempestad: %.1f", diasTempestad);
	printf("\nMedia niebla: %.1f", diasNiebla);
	printf("\nMedia helada: %.1f", diasHelada);
	printf("\nMedia vacíos: %.1f", diasVacios);
	printf("\nMedia horas de sol: %d", horasDeSol);
}


bool clonarArchivo(){
	FILE *archivoACopiar = fopen((char*)nombreFicheroInicial, "r");
	FILE *ficheroClonado = fopen((char*)nombreFicheroFinal, "a+");

	if (archivoACopiar == NULL){
		printf("Hubo un problema al abrir el fichero de entrada");
		return false;
	}

	if(archivoACopiar == NULL){
		printf("Hubo un problema al abrir el fuchero de salida");
		return false;
	}

	while(!feof(archivoACopiar)){
		fscanf(archivoACopiar, ) // AQUI ME QUEDO
	}

	return true;
}


/**
 * @param numeroLeido el número que ha introducido el usuario
 * @param tamanoMinimo el rango mínimo de ese dato
 * @param tamanoMaximo el rango máximo de ese dato
 * @param parametrosLeidos el número de parámetros que ha leído la función scanf()
 * @param enter saber si el usuario ha introducido un enter al final
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
 * @param numeroLeido el número que ha introducido el usuario
 * @param tamanoMinimo el rango mínimo de ese dato
 * @param tamanoMaximo el rango máximo de ese dato
 * @param parametrosLeidos el número de parámetros que ha leído la función scanf()
 * @param enter saber si el usuario ha introducido un enter al final
 */
bool validaReal(float numeroLeido, int tamanoMinimo, float tamanoMaximo, float parametrosLeidos, char enter)
{

	bool datoCorrecto = false;

	if (parametrosLeidos == 2.0f)
		if (enter == '\n')
			if (numeroLeido >= tamanoMinimo && numeroLeido <= tamanoMaximo)
				datoCorrecto = true;

	printf("     Pulse intro para continuar");
	limpiarBuffer();
	return datoCorrecto;
}

/**
 * 
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

/**
 * 
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

	while (!opcionCorrecta)
	{
		printf("\t ---> ");
		scanf("%d%*[^\n]", &opcionEstacion);
		limpiarBuffer();
		if (opcionEstacion >= 1 && opcionEstacion <= opcionMaxima)
		{
			opcionCorrecta = true;
		}
	}

	opcionEstacion += sumar;
	opcionEstacion--;

	return opcionEstacion;
}

/**
 * 
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
