
/* 

		EQUIPO 06:
	
	ANAYA DIAZ CRISTIAN
	ESTRADA SALAZAR JORDI
	GARCIA MARTINEZ MICHELLE YARED
	RUEDA GRANADOS DANIEL LEVI
	
*/

/* Supóngase que la empresa IA-MUEBLES fabrica diferentes productos y desea crear gráficas 
que le permitan hacer un diagnóstico de cómo se está realizando el trabajo. 

Los datos que actualmente se guardan de cada producto son:

    Clave (3 caracteres)
    Descripción del producto (50 caracteres)
    Costo de producción (flotante)
    Precio de venta (flotante)
    Unidades vendidas (flotante)

El programa desarrollado debe contener un menú con las siguientes opciones:

    1. Dar de alta un producto.
    2. Mostrar todos los productos.
    3. Editar un producto.
    4. Eliminar un producto.
    5. Crear gráfica de columnas.
    6. Crear gráfica circular.
    7. Salir.

REQUISITOS:

    Usar:
	estructuras
	archivos
	memoria dinámica.
	
    Los archivos deben ser binarios.
    El listado que debe mostrar la opción 2 del menú será en forma tabular. 
	El cálculo de la ganancia se hace con la siguiente fórmula: 
	Ganancia = (Precio-Costo)*Unidades vendidas.()


    La opción 3, debe permitir cambiar cualquier dato del producto a excepción de la clave.
	La opción para eliminar solo debe marcar al registro y no borrarlo físicamente.
	La gráfica de columnas se hará con el campo "Unidades vendidas". 
	
	Nótese que el fondo es blanco. Los colores pueden ser aleatorios o consecutivos.


La gráfica circular se hará con el porcentaje de ganancias de cada producto RESPECTO A LA GANANCIA TOTAL 
									(TODOS LOS PRODUCTOS).

Entregar el archivo binario de productos con al menos 5 registros capturados
					diferentes a los de este ejemplo.

Entregar TODA LA CARPETA DEL PROYECTO DE CODEBLOCKS en un archivo ZIP. 
		El nombre del archivo deben ser PF_06_Muebles.zip
*/


/* 				BIBLIOTECAS QUE PUEDE QUE USEMOS 
(las vamos desmarcando conforme las usemos, para no saturar el programa): */

#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <ctype.h>
//#include <winbgim.h>
#include <locale.h>
#include <string.h>


// -- Estructura Producto -- //
typedef struct
{
    char clave[3]; //AQUI SON DOS USABLES + \0
    char descripcion_producto[50]; //49 char + \0
    float costo_produccion;
    float precio_venta;
    float unidades_vendidas;
    int borrado; // 0 = Activo, 1 = Borrado logico, NO fisico.

}Producto;

// Agregué prototipos para no tener que preocuparnos por el orden al momento de añadir las funciones. 
//Cuando se agregue una función favor de agregarla también a los prototipos.

void pausa(void);
void Limpiar_buffer(void);
void Limpiar_salto_linea(char *cadena);
int  Buscar_posicion(char *clave, int n, Producto *p);
void Alta_producto(void);
void Listado(void);
void Ver_menu(void);
int  Seleccion_menu(int opcion);

// ---------------------------- FUNCIONES AUXILIARES ------------------------------//


//Detiene la ejecución hasta que se pulsa ENTER
void pausa(void)
{
	printf("\nPresione Enter para continuar...");
	fflush(stdout); //Vacia el buffer de salida para que se alcance a leer el mensaje.
	getchar();
}


/*
 		Evita que posteriormente se lea basura. 
 		DESCARTA: caracteres pendientes en bufer stdin (el de entrada)
 		IMPORTANTE: tambien quita el \n que deja scanf

*/
void Limpiar_buffer()
{
    int c;
    while((c=getchar()) != '\n' && c != EOF);
}

/*

 		Recibe cadena y reemplaza el \n que deja fgets
		 al final por un \0.
*/
void Limpiar_salto_linea(char *cadena){
	int indiceSalto;
	indiceSalto = strcspn(cadena, "\n"); //busca posicion de \n
	cadena[indiceSalto] = '\0'; //lo reemplaza por \0
}

/*
			esta funcion implica ARREGLO DINAMICO. 
			Explicacion de parametros: 
			clave es la cadena, n el total de registros, p es puntero al arreglo (mem. dinamica)
			returna: indice del producto o -1 si no existe
			NOTA: Solo considera los no eliminados (borrado == 0)

*/
int Buscar_posicion(char *clave, int n, Producto *p)
{
    int i, encontrado = -1;
    for (i = 0; i < n; i++)
    {
        /* strcmp compara dos cadenas; retorna 0 si son iguales */
        if (strcmp(clave, p[i].clave) == 0 && !p[i].borrado)
        {
            encontrado = i;
            break;
        }
    }
    return encontrado;
}

void Alta_producto()
{
	
}

void Listado()
{

}

void Ver_menu(void)
{
    printf("\n\t------------ MENÚ ------------\n");
    printf("\t 1. Dar de alta un producto\n");
    printf("\t 2. Mostrar todos los productos\n");
    printf("\t 3. Editar un producto\n");
    printf("\t 4. Eliminar un producto\n");
    printf("\t 5. Crear gráfica de columnas\n");
    printf("\t 6. Crear gráfica circular\n");  
    printf("\t 7. Salir\n\n\n");                   
}


// ---------------------------- FUNCIONES PRINCIPALES ------------------------------//
int Seleccion_menu(int opcion)
{

    do
    {
    	Ver_menu(); //para que lo muestre en cada repeticion
        printf("\tSeleccionar opcion: ");
        scanf("%d", &opcion);
        Limpiar_buffer(); //por si acaso, limpiamos el \n

            switch(opcion)
            {
                case 1:
                	Alta_producto();
                	break;

                case 2:
                	Listado(); //Muestra la tabla con los productos
                	break;

                case 3:
                	
                	break;

                case 4:
                	
                	break;

                case 5:
                	
                	break;

                case 6:
                    
                    break;
                
                case 7:
                	printf("\nGracias por usar el programa, vuelva pronto.\n \t CREDITOS: EQUIPO 06");
                	break;
                
                default:
                	printf("\nOpción inválida. Ingrese un número del 1 al 7.\n");
                	pausa(); //para que alcance a ver el mensaje
                	break;
                	
            }
    }while(opcion != 7); //REPITE HASTA QUE ELIJAN SALIR
    
    return opcion;
}




int main()
{
	
	int opcion=0;
	
	//caracteres en español
    setlocale(LC_ALL, "spanish");
    setlocale(LC_ALL, "es_ES");
    //Producto product;
    
	Seleccion_menu(opcion);
    return 0;
}

