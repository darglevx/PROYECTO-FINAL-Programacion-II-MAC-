
/* 

		EQUIPO 06:
	
	ANAYA DIAZ CRISTIAN
	ESTRADA SALAZAR JORDI
	GARCIA MARTINEZ MICHELLE YARED
	RUEDA GRANADOS DANIEL LEVI
	
*/

/* Sup�ngase que la empresa IA-MUEBLES fabrica diferentes productos y desea crear gr�ficas 
que le permitan hacer un diagn�stico de c�mo se est� realizando el trabajo. 

Los datos que actualmente se guardan de cada producto son:

    Clave (3 caracteres)
    Descripci�n del producto (50 caracteres)
    Costo de producci�n (flotante)
    Precio de venta (flotante)
    Unidades vendidas (flotante)

El programa desarrollado debe contener un men� con las siguientes opciones:

    1. Dar de alta un producto.
    2. Mostrar todos los productos.
    3. Editar un producto.
    4. Eliminar un producto.
    5. Crear gr�fica de columnas.
    6. Crear gr�fica circular.
    7. Salir.

REQUISITOS:

    Usar:
	estructuras
	archivos
	memoria din�mica.
	
    Los archivos deben ser binarios.
    El listado que debe mostrar la opci�n 2 del men� ser� en forma tabular. 
	El c�lculo de la ganancia se hace con la siguiente f�rmula: 
	Ganancia = (Precio-Costo)*Unidades vendidas.()


    La opci�n 3, debe permitir cambiar cualquier dato del producto a excepci�n de la clave.
	La opci�n para eliminar solo debe marcar al registro y no borrarlo f�sicamente.
	La gr�fica de columnas se har� con el campo "Unidades vendidas". 
	
	N�tese que el fondo es blanco. Los colores pueden ser aleatorios o consecutivos.


La gr�fica circular se har� con el porcentaje de ganancias de cada producto RESPECTO A LA GANANCIA TOTAL 
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
    char clave[3]; //AQUI SON DOS USABLES + \0
    char descripcion_producto[50]; //49 char + \0
    float costo_produccion;
    float precio_venta;
    float unidades_vendidas;
    int borrado; // 0 = Activo, 1 = Borrado logico, NO fisico.

}Producto;

// Agregu� prototipos para no tener que preocuparnos por el orden al momento de a�adir las funciones. 
//Cuando se agregue una funci�n favor de agregarla tambi�n a los prototipos.

void pausa(void); //completado
void Limpiar_buffer(void); //completado
void Limpiar_salto_linea(char *cadena); //completado
int  Buscar_posicion(char *clave, int n, Producto *p); //completado
void Alta_producto(void); //completado
void Listado(void); //incompleto: por completar
void Ver_menu(void); //completado
int  Seleccion_menu(int opcion); //completado
void borrar(); //haciendo: por completar

// ---------------------------- FUNCIONES AUXILIARES ------------------------------//


//Detiene la ejecuci�n hasta que se pulsa ENTER
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
void Listado()
{
    FILE *archivo;
    Producto temp;
    int i = 1;

	system("cls");

    printf("\n\t\tLISTADO DE PRODUCTOS\n");

    archivo = fopen("productos.dat", "rb");

    if (archivo == NULL)    {
        printf("\tError: no se pudo abrir el archivo para lectura.\n");
        system("pause");
        return;
    }
    
    fread(&temp, sizeof(Producto), 1, archivo);

    while(!feof(archivo))
    {
        if(temp.borrado == 0) // Solo muestra los no eliminados
        {
            printf("clave: %s\n", temp.clave);
            printf("descripci�n: %s\n", temp.descripcion_producto);
            printf("costo de producci�n: %.2f\n", temp.costo_produccion);
            printf("precio de venta: %.2f\n", temp.precio_venta);
            printf("unidades vendidas: %.0f\n", temp.unidades_vendidas);
        }
        printf("-----------------------------------\n");
        fread(&temp, sizeof(Producto), 1, archivo);
    }

    pausa();

    fclose(archivo);
}

///////////////////////////////////
void Ver_menu(void)
{
	system("cls"); //agregar al principio de cada funcion principal, de preferencia
    printf("\n\t------------ MEN� ------------\n");
    printf("\t 1. Dar de alta un producto\n");
    printf("\t 2. Mostrar todos los productos\n");
    printf("\t 3. Editar un producto\n");
    printf("\t 4. Eliminar un producto\n");
    printf("\t 5. Crear gr�fica de columnas\n");
    printf("\t 6. Crear gr�fica circular\n");  
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

        // Cargar en memoria din�mica
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
        printf("\tClave del producto (2 caracteres): ");
        fgets(clave_temp, sizeof(clave_temp), stdin);
        Limpiar_salto_linea(clave_temp);

        if (strlen(clave_temp) != 2)
        {
            printf("\tError: la clave debe tener exactamente 2 caracteres.\n");
            continue;
        }

        // Verificar si hay duplicado
        if (n > 0 && Buscar_posicion(clave_temp, n, lista) != -1)
        {
            printf("\tError: ya existe un producto activo con esa clave.\n");
            continue;
        }

        break; // Clave v�lida y �nica

    } while (1);

    strncpy(nuevo.clave, clave_temp, 3); // Copia los 2 chars + \0

    // 2. DESCRIPCI�N DEL PROD.
    printf("\tDescripci�n del producto (m�x. 49 caracteres): ");
    fgets(nuevo.descripcion_producto, sizeof(nuevo.descripcion_producto), stdin);
    Limpiar_salto_linea(nuevo.descripcion_producto);

    // 3. COSTO DE PRODUCCI�N
    do
    {
        printf("\tCosto de producci�n: $");
        if (scanf("%f", &nuevo.costo_produccion) != 1 || nuevo.costo_produccion < 0)
        {
            printf("\tError: ingrese un valor num�rico positivo.\n");
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
            printf("\tError: ingrese un valor num�rico positivo.\n");
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
            printf("\tError: ingrese un valor num�rico positivo.\n");
            Limpiar_buffer();
            nuevo.unidades_vendidas = -1;
            continue;
        }
        Limpiar_buffer();
        break;
    } while (1);

    // 6. BORRADO l�gico: siempre inicia en 0 (activo)
    nuevo.borrado = 0;

    // 7. CALCULA LA GANANCIA
    float ganancia = (nuevo.precio_venta - nuevo.costo_produccion) * nuevo.unidades_vendidas;

    // 8. CONFIRMACI�N
    printf("\n\t	CONFIRMAR DATOS 	\n");
    printf("\tClave           : %s\n",  nuevo.clave);
    printf("\tDescripci�n     : %s\n",  nuevo.descripcion_producto);
    printf("\tCosto producci�n: $%.2f\n", nuevo.costo_produccion);
    printf("\tPrecio de venta : $%.2f\n", nuevo.precio_venta);
    printf("\tUnidades vendidas: %.0f\n", nuevo.unidades_vendidas);
    printf("\tGanancia estimada: $%.2f\n", ganancia);
    printf("\n\t�Guardar este producto? (s/n): ");

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

    // 10. LIBERAR MEMORIA DIN�MICA
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
                	//borrar();
                	break;

                case 5:
                	
                	break;

                case 6:
                    
                    break;
                
                case 7:
                	printf("\nGracias por usar el programa, vuelva pronto.\n \t CREDITOS: EQUIPO 06");
                	break;
                
                default:
                	printf("\nOpci�n inv�lida. Ingrese un n�mero del 1 al 7.\n");
                	pausa(); //para que alcance a ver el mensaje
                	break;
                	
            }
    }while(opcion != 7); //REPITE HASTA QUE ELIJAN SALIR
    
    return opcion;
}
/*/
void Borrar(){
	char clavebuscada[10];
	FILE *archivo;
	int posicion, respuesta;
	
	system("cls");
	
	printf("Borrado de producto\n");
	
	printf("Clave del producto: ");
	LeerCadena(clavebuscada, 3);
	
	archivo=fopen("productos.dat", "r+b");
	
	posicion=buscarPosicion(clavebuscada, archivo);
	
	if(posicion == -1){
		printf("Producto no encontrado\n");
		system("pause");
		return;
	}
	
	//Nos posicionamos en el producto
	fseek(archivo, posicion*sizeof(Producto) ,SEEK_SET);
	
	//Leemos los datos
	fread(&nuevo, sizeof(Producto), 1, archivo);
	
	EscribirProducto(nuevo);
	
	printf("Confirmación de borrado (1=SI / 0=NO): ");
	scanf("%d", &respuesta);
	
	if(respuesta==0)
		printf("Cancelado\n");
	else{
		fseek(archivo, posicion*sizeof(Producto), SEEK_SET);
		nuevo.borrado=1;
		fwrite(&nuevo, sizeof(Producto), 1, archivo);
		printf("Producto borrado\n");
	}
	
	fclose(archivo);
	
	system("pause");
}
*/
int main()
{
	
	int opcion=0;
	
	//caracteres en espa�ol
    setlocale(LC_ALL, "spanish");
    //conserva el punto de los decimales y no reemplaza por comas.
    setlocale(LC_NUMERIC, "C");
    //Producto product;
    
	Seleccion_menu(opcion);
    return 0;
}

