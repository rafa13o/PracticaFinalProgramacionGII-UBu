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

int main()
{

	int opcion; // Opcion que ha escogido el usuario del menú
	bool repetir = true;
	
	while(repetir){
		opcion = solicitarOpcionMenu();
		seleccionarOpcion(opcion);
		if(opcion==0){
			repetir=false;
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
	} while (!opcionCorrecta);

	opcion = charOpcion - '0';
	limpiarBuffer();

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
	int contador = 1; // Comienzo a contar en 1

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



void limpiarBuffer(){
	while(getchar() != '\n'){}
}