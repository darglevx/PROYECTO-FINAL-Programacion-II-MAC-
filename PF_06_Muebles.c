
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
#include <stdlib.h>
//#include <math.h>
//#include <ctype.h>
//#include <winbgim.h>
#include <locale.h>
#include <string.h>


// -- Estructura Producto -- //
typedef struct
{
    char clave[4]; //AQUI SON TRES USABLES + \0
    char descripcion_producto[51]; //50 char + \0
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

//////////////////////////////////

void Listado(void) //SEGUNDA OPCION
{
    system("cls");

    FILE *archivo;
    Producto *lista = NULL;
    int n = 0, activos = 0, i;

    archivo = fopen("productos.dat", "rb");
    if (archivo == NULL)
    {
        printf("\n\tNo hay productos registrados aún.\n");
        pausa();
        return;
    }

    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1)
        ++n;
    rewind(archivo);

    if (n == 0)
    {
        fclose(archivo);
        printf("\n\tEl archivo está vacío.\n");
        pausa();
        return;
    }

    lista = (Producto *)malloc(n * sizeof(Producto));
    if (lista == NULL)
    {
        printf("\tError: no se pudo reservar memoria.\n");
        fclose(archivo);
        pausa();
        return;
    }

    fread(lista, sizeof(Producto), n, archivo);
    fclose(archivo);
    
						// T A B L A 
    // Encabezado (sin \t para no desplazar)
    printf("\n %-5s %-30s %12s %13s %15s %12s\n",
           "Clave", "Descripcion", "Costo prod.", "Precio venta",
           "Unid.vendidas", "Ganancia");
    printf(" ");
    for (i = 0; i < 92; i++) printf("-");
    printf("\n");

    float ganancia_total = 0.0f;

    for (i = 0; i < n; i++)
    {
        if (lista[i].borrado) continue;

        float ganancia = (lista[i].precio_venta - lista[i].costo_produccion)
                         * lista[i].unidades_vendidas;
        ganancia_total += ganancia;
        activos++;

        printf(" %-5s %-30s %11.2f %12.2f %15.0f %11.2f\n",
               lista[i].clave,
               lista[i].descripcion_producto,
               lista[i].costo_produccion,
               lista[i].precio_venta,
               lista[i].unidades_vendidas,
               ganancia);
    }

    printf(" ");
    for (i = 0; i < 92; i++) printf("-");
    printf("\n");

    if (activos == 0)
        printf("\n No hay productos activos.\n");
    else
        printf("%-57s %11.2f\n", "GANANCIA TOTAL:", ganancia_total);
        
    /* 
	El 57 imprime el texto "GANANCIA TOTAL:" justificado a la izquierda ocupando 57 caracteres 
	Ese espacio en blanco es lo que empuja el número hasta alinearlo con la columna de Ganancia.
	el 11 imprime ganancia_total con 2 decimales en un espacio de 11 caracteres.
	*/

    printf("\n Total de productos activos: %d\n", activos);

    free(lista);
    pausa();
}

///////////////////////////////////
void Ver_menu(void)
{
	system("cls"); //agregar al principio de cada funcion principal, de preferencia
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

void Alta_producto(void)
{
    Producto nuevo;
    FILE *archivo;
    char clave_temp[10]; // validar longitud, entonces usamos una mas grande

    printf("\n\t	Alta de producto 	\n");

    // 1. CLAVE
    // ver si hay duplicados
    int n = 0;
    Producto *lista = NULL;

    archivo = fopen("productos.dat", "rb");
    if (archivo != NULL)
    {
        // Contar existentes
        Producto temp;
        while (fread(&temp, sizeof(Producto), 1, archivo) == 1)
            n++;
        rewind(archivo);

        // Cargar en memoria dinámica
        lista = (Producto *)malloc(n * sizeof(Producto));
        if (lista == NULL)
        {
            printf("Error: no se pudo reservar memoria.\n");
            fclose(archivo);
            pausa();
            return;
        }
        fread(lista, sizeof(Producto), n, archivo);
        fclose(archivo);
    }

    // Pedir y validar clave
    do
    {
        printf("\tClave del producto (3 caracteres): ");
        fgets(clave_temp, sizeof(clave_temp), stdin);
        Limpiar_salto_linea(clave_temp);

        if (strlen(clave_temp) != 3)
        {
            printf("\tError: la clave debe tener exactamente 3 caracteres.\n");
            continue;
        }

        // Verificar si hay duplicado
        if (n > 0 && Buscar_posicion(clave_temp, n, lista) != -1)
        {
            printf("\tError: ya existe un producto activo con esa clave.\n");
            continue;
        }

        break; // Clave válida y única

    } while (1);

    strncpy(nuevo.clave, clave_temp, 4); // Copia los 3 chars + \0

    // 2. DESCRIPCIÓN DEL PROD.
    printf("\tDescripción del producto (máx. 50 caracteres): ");
    fgets(nuevo.descripcion_producto, sizeof(nuevo.descripcion_producto), stdin);
    Limpiar_salto_linea(nuevo.descripcion_producto);

    // 3. COSTO DE PRODUCCIÓN
    do
    {
        printf("\tCosto de producción: $");
        if (scanf("%f", &nuevo.costo_produccion) != 1 || nuevo.costo_produccion < 0)
        {
            printf("\tError: ingrese un valor numérico positivo.\n");
            Limpiar_buffer();
            nuevo.costo_produccion = -1; // reintento
            continue;
        }
        Limpiar_buffer();
        break;
    } while (1);

    // 4. PRECIO DE VENTA
    do
    {
        printf("\tPrecio de venta:     $");
        if (scanf("%f", &nuevo.precio_venta) != 1 || nuevo.precio_venta < 0)
        {
            printf("\tError: ingrese un valor numérico positivo.\n");
            Limpiar_buffer();
            nuevo.precio_venta = -1;
            continue;
        }
        Limpiar_buffer();
        break;
    } while (1);

    // 5. UNIDADES VENDIDAS
    do
    {
        printf("\tUnidades vendidas:   ");
        if (scanf("%f", &nuevo.unidades_vendidas) != 1 || nuevo.unidades_vendidas < 0)
        {
            printf("\tError: ingrese un valor numérico positivo.\n");
            Limpiar_buffer();
            nuevo.unidades_vendidas = -1;
            continue;
        }
        Limpiar_buffer();
        break;
    } while (1);

    // 6. BORRADO lógico: siempre inicia en 0 (activo)
    nuevo.borrado = 0;

    // 7. CALCULA LA GANANCIA
    float ganancia = (nuevo.precio_venta - nuevo.costo_produccion) * nuevo.unidades_vendidas;

    // 8. CONFIRMACIÓN
    printf("\n\t	CONFIRMAR DATOS 	\n");
    printf("\tClave           : %s\n",  nuevo.clave);
    printf("\tDescripción     : %s\n",  nuevo.descripcion_producto);
    printf("\tCosto producción: $%.2f\n", nuevo.costo_produccion);
    printf("\tPrecio de venta : $%.2f\n", nuevo.precio_venta);
    printf("\tUnidades vendidas: %.0f\n", nuevo.unidades_vendidas);
    printf("\tGanancia estimada: $%.2f\n", ganancia);
    printf("\n\t¿Guardar este producto? (s/n): ");

    char confirmacion;
    confirmacion = getchar();
    Limpiar_buffer();

    if (confirmacion != 's' && confirmacion != 'S')
    {
        printf("\tAlta cancelada.\n");
        free(lista);
        pausa();
        return;
    }

    //  9. ESCRITURA EN ARCHIVO BINARIO
    archivo = fopen("productos.dat", "ab");
    if (archivo == NULL)
    {
        printf("\tError: no se pudo abrir el archivo para escritura.\n");
        free(lista);
        pausa();
        return;
    }

    fwrite(&nuevo, sizeof(Producto), 1, archivo);
    fclose(archivo);

    printf("\n\tProducto '%s' guardado correctamente.\n", nuevo.clave);

    // 10. LIBERAR MEMORIA DINÁMICA
    free(lista);

    pausa(); //hacer que vea el mensaje, sino pasa de largo
}

////////////////////////////////////////////////////////////////////////////////////////
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
    //conserva el punto de los decimales y no reemplaza por comas.
    setlocale(LC_NUMERIC, "C");
    //Producto product;
    
	Seleccion_menu(opcion);
    return 0;
}

