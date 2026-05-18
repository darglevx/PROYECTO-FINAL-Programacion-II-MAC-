
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

void pausa();
void Limpiar_buffer();
void Limpiar_salto_linea(char *cadena);
int  Buscar_posicion(char *clave, int n, Producto *p);
void Alta_producto();
void Listado();
void Editar_producto();
void Eliminar_producto();
void Ver_menu();
int  Seleccion_menu(int opcion);

// ---------------------------- FUNCIONES AUXILIARES ------------------------------//


//Detiene la ejecución hasta que se pulsa ENTER
void pausa()
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


void Ver_menu()
{
	system("cls"); //agregar al principio de cada funcion principal, de preferencia
    printf("\n\t	     MENÚ 		\n\n");
    printf("\t 1. Dar de alta un producto\n");
    printf("\t 2. Mostrar todos los productos\n");
    printf("\t 3. Editar un producto\n");
    printf("\t 4. Eliminar un producto\n");
    printf("\t 5. Crear gráfica de columnas\n");
    printf("\t 6. Crear gráfica circular\n");  
    printf("\t 7. Salir\n\n\n");                   
}


// ---------------------------- FUNCIONES PRINCIPALES ------------------------------//


//////////////////////////////////////////////////////////////////////////////////////
void Listado() //SEGUNDA OPCION
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

//////////////////////////////////////////////////////////////////////////////////////////////

void Alta_producto()
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
        printf("\tPrecio de venta: $");
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
        printf("\tUnidades vendidas: ");
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
    printf("\tClave: %s\n",  nuevo.clave);
    printf("\tDescripción: %s\n",  nuevo.descripcion_producto);
    printf("\tCosto producción: $%.2f\n", nuevo.costo_produccion);
    printf("\tPrecio de venta: $%.2f\n", nuevo.precio_venta);
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

/////////////////////////////////////////////////////////////////////////////////////////

void Editar_producto()
{
    system("cls");

    FILE *archivo;
    Producto *lista = NULL;
    int n = 0, i, pos;
    char clave_buscar[10];
    int opcion_campo;

    // Abrimos el archivo en modo lectura binaria para cargar los productos existentes
    archivo = fopen("productos.dat", "rb");
    if (archivo == NULL)
    {
        printf("\n\tNo hay productos registrados aún.\n");
        pausa();
        return;
    }

    // Contamos cuántos registros hay en el archivo recorriéndolo completo
    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1)
        n++;
    rewind(archivo); // Regresamos el cursor al inicio para poder leer de nuevo

    if (n == 0)
    {
        fclose(archivo);
        printf("\n\tEl archivo está vacío.\n");
        pausa();
        return;
    }

    // Reservamos memoria dinámica para guardar todos los registros del archivo
    lista = (Producto *)malloc(n * sizeof(Producto));
    if (lista == NULL)
    {
        printf("\tError: no se pudo reservar memoria.\n");
        fclose(archivo);
        pausa();
        return;
    }

    // Cargamos todos los registros (incluyendo borrados lógicos) en el arreglo dinámico
    fread(lista, sizeof(Producto), n, archivo);
    fclose(archivo);

    // PEDIR CLAVE A BUSCAR
    printf("\n\tEditar producto\n");
    printf("\tIngrese la clave del producto a editar: ");
    fgets(clave_buscar, sizeof(clave_buscar), stdin);
    Limpiar_salto_linea(clave_buscar); // Quitamos el \n que deja fgets

    // Buscamos la posición del producto en el arreglo; retorna -1 si no existe o está borrado
    pos = Buscar_posicion(clave_buscar, n, lista);
    if (pos == -1)
    {
        printf("\n\tProducto con clave '%s' no encontrado o está eliminado.\n", clave_buscar);
        free(lista);
        pausa();
        return;
    }

    // MOSTRAR DATOS ACTUALES del producto encontrado antes de editar
    printf("\n\tDatos actuales del producto:\n");
    printf("\t  Clave: %s  (no editable)\n", lista[pos].clave);
    printf("\t  1. Descripción: %s\n",   lista[pos].descripcion_producto);
    printf("\t  2. Costo producto: $%.2f\n", lista[pos].costo_produccion);
    printf("\t  3. Precio venta: $%.2f\n", lista[pos].precio_venta);
    printf("\t  4. Unid. vendidas: %.0f\n",  lista[pos].unidades_vendidas);
    printf("\t  5. Editar todos los campos\n");
    printf("\t  0. Cancelar\n");

// Validamos que la opción ingresada esté en el rango permitido (0 a 5)
//---------------------------DUDA-------------------------------------------------------
    do
    {
        printf("\n\t¿Qué desea editar? (0-5): ");
        if (scanf("%d", &opcion_campo) != 1) // scanf retorna 0 si no pudo leer un entero (ej: letra)
        {
            Limpiar_buffer(); // Descartamos el carácter inválido que quedó en el buffer
            printf("\tError: ingrese solo números del 0 al 5.\n");
            opcion_campo = -1; // Forzamos reintento con valor fuera del rango válido
        }
        else if (opcion_campo < 0 || opcion_campo > 5)
        {
            Limpiar_buffer();
            printf("\tError: opción fuera de rango, ingrese un número del 0 al 5.\n");
        }
        else
        {
            Limpiar_buffer(); // Limpiamos el \n que dejó el scanf válido
        }
    } while (opcion_campo < 0 || opcion_campo > 5);
 //--------------------------DUDA--------------------------------------------------------
    
    if (opcion_campo == 0)
    {
        printf("\n\tEdición cancelada.\n");
        free(lista);
        pausa();
        return;
    }

    printf("\n");

    // EDITAR DESCRIPCIÓN: se ejecuta si eligió 1 o editar todo (5)
    if (opcion_campo == 1 || opcion_campo == 5)
    {
        printf("\tNueva descripción (máx. 50 caracteres): ");
        // fgets escribe directamente sobre el campo del registro en memoria dinámica
        fgets(lista[pos].descripcion_producto, sizeof(lista[pos].descripcion_producto), stdin);
        Limpiar_salto_linea(lista[pos].descripcion_producto);
    }

    // EDITAR COSTO: se ejecuta si eligió 2 o editar todo (5)
    if (opcion_campo == 2 || opcion_campo == 5)
    {
        do
        {
            printf("\tNuevo costo de producción: $");
            // Validamos que sea un número y que no sea negativo
            if (scanf("%f", &lista[pos].costo_produccion) != 1 || lista[pos].costo_produccion < 0)
            {
                printf("\tError: ingrese un valor numérico positivo.\n");
                Limpiar_buffer();
                lista[pos].costo_produccion = -1; // Forzamos reintento con valor inválido
                continue;
            }
            Limpiar_buffer();
            break;
        } while (1);
    }

    // EDITAR PRECIO: se ejecuta si eligió 3 o editar todo (5)
    if (opcion_campo == 3 || opcion_campo == 5)
    {
        do
        {
            printf("\tNuevo precio de venta: $");
            if (scanf("%f", &lista[pos].precio_venta) != 1 || lista[pos].precio_venta < 0)
            {
                printf("\tError: ingrese un valor numérico positivo.\n");
                Limpiar_buffer();
                lista[pos].precio_venta = -1;
                continue;
            }
            Limpiar_buffer();
            break;
        } while (1);
    }

    // EDITAR UNIDADES: se ejecuta si eligió 4 o editar todo (5)
    if (opcion_campo == 4 || opcion_campo == 5)
    {
        do
        {
            printf("\tNuevas unidades vendidas: ");
            if (scanf("%f", &lista[pos].unidades_vendidas) != 1 || lista[pos].unidades_vendidas < 0)
            {
                printf("\tError: ingrese un valor numérico positivo.\n");
                Limpiar_buffer();
                lista[pos].unidades_vendidas = -1;
                continue;
            }
            Limpiar_buffer();
            break;
        } while (1);
    }

    // Calculamos la ganancia con los datos ya modificados en memoria para mostrarla en la confirmación
    float ganancia = (lista[pos].precio_venta - lista[pos].costo_produccion)
                     * lista[pos].unidades_vendidas;

    // Mostramos resumen final antes de confirmar la escritura en disco
    printf("\n\tDatos actualizados:\n");
    printf("\t  Clave: %s\n",   lista[pos].clave);
    printf("\t  Descripción: %s\n",   lista[pos].descripcion_producto);
    printf("\t  Costo producto: $%.2f\n", lista[pos].costo_produccion);
    printf("\t  Precio venta: $%.2f\n", lista[pos].precio_venta);
    printf("\t  Unid. vendidas: %.0f\n",  lista[pos].unidades_vendidas);
    printf("\t  Ganancia estimada: $%.2f\n", ganancia);
    printf("\n\t¿Guardar cambios? (s/n): ");

    char confirmacion = getchar();
    Limpiar_buffer();

    // Si el usuario no confirma, descartamos los cambios
    if (confirmacion != 's' && confirmacion != 'S')
    {
        printf("\n\tCambios descartados.\n");
        free(lista);
        pausa();
        return;
    }

    // Abrimos el archivo en modo escritura binaria "wb": borra el contenido anterior
    // y lo reescribimos completo con el arreglo ya modificado en memoria
    archivo = fopen("productos.dat", "wb");
    if (archivo == NULL)
    {
        printf("\n\tError: no se pudo abrir el archivo para escritura.\n");
        free(lista);
        pausa();
        return;
    }

    // Escribimos los n registros de golpe; esto incluye borrados lógicos y el registro editado
    fwrite(lista, sizeof(Producto), n, archivo);
    fclose(archivo);

    printf("\n\tProducto '%s' actualizado correctamente.\n", lista[pos].clave);

    free(lista); // Liberamos la memoria dinámica que reservamos con malloc
    pausa();
}
/////////////////////////////////////////////////////////////////////////////////////////

void Eliminar_producto()
{
    system("cls");

    FILE *archivo;
    Producto *lista = NULL;
    int n = 0, pos;
    char clave_buscar[10];

    // Abrimos el archivo en modo lectura binaria para cargar los productos existentes
    archivo = fopen("productos.dat", "rb");
    if (archivo == NULL)
    {
        printf("\n\tNo hay productos registrados aún.\n");
        pausa();
        return;
    }

    // Contamos cuántos registros hay recorriendo el archivo completo
    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1)
        n++;
    rewind(archivo); // Regresamos el cursor al inicio para poder leer de nuevo

    if (n == 0)
    {
        fclose(archivo);
        printf("\n\tEl archivo está vacío.\n");
        pausa();
        return;
    }

    // Reservamos memoria dinámica para todos los registros
    lista = (Producto *)malloc(n * sizeof(Producto));
    if (lista == NULL)
    {
        printf("\tError: no se pudo reservar memoria.\n");
        fclose(archivo);
        pausa();
        return;
    }

    // Cargamos todos los registros (incluyendo borrados lógicos) en el arreglo dinámico
    fread(lista, sizeof(Producto), n, archivo);
    fclose(archivo);

    // Pedimos la clave del producto a eliminar
    printf("\n\tEliminar producto\n");
    printf("\tIngrese la clave del producto a eliminar: ");
    fgets(clave_buscar, sizeof(clave_buscar), stdin);
    Limpiar_salto_linea(clave_buscar); // Quitamos el \n que deja fgets

    // Buscamos el producto; retorna -1 si no existe o ya está borrado
    pos = Buscar_posicion(clave_buscar, n, lista);
    if (pos == -1)
    {
        printf("\n\tProducto con clave '%s' no encontrado o ya está eliminado.\n", clave_buscar);
        free(lista);
        pausa();
        return;
    }

    // Mostramos los datos del producto antes de confirmar
    float ganancia = (lista[pos].precio_venta - lista[pos].costo_produccion)
                     * lista[pos].unidades_vendidas;

    printf("\n\tDatos del producto a eliminar:\n");
    printf("\t  Clave: %s\n",   lista[pos].clave);
    printf("\t  Descripción: %s\n",   lista[pos].descripcion_producto);
    printf("\t  Costo producto: $%.2f\n", lista[pos].costo_produccion);
    printf("\t  Precio venta: $%.2f\n", lista[pos].precio_venta);
    printf("\t  Unid. vendidas: %.0f\n",  lista[pos].unidades_vendidas);
    printf("\t  Ganancia: $%.2f\n", ganancia);

    // Confirmación antes de marcar como eliminado
    printf("\n\t¿Está seguro de eliminar este producto? (s/n): ");
    char confirmacion = getchar();
    Limpiar_buffer();

    if (confirmacion != 's' && confirmacion != 'S')
    {
        printf("\n\tEliminación cancelada.\n");
        free(lista);
        pausa();
        return;
    }

    // Marcamos el registro como borrado lógicamente (borrado = 1)
    // El registro PERMANECE en el archivo; solo cambia este campo
    lista[pos].borrado = 1;

    // Abrimos en "wb" para reescribir el archivo completo con el cambio aplicado
    archivo = fopen("productos.dat", "wb");
    if (archivo == NULL)
    {
        printf("\n\tError: no se pudo abrir el archivo para escritura.\n");
        free(lista);
        pausa();
        return;
    }

    // Escribimos todos los registros de golpe, incluido el recién marcado como borrado
    fwrite(lista, sizeof(Producto), n, archivo);
    fclose(archivo);

    printf("\n\tProducto '%s' eliminado correctamente.\n", lista[pos].clave);

    free(lista); // Liberamos la memoria dinámica que reservamos con malloc
    pausa();
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
                	Editar_producto();
                	break;

                case 4:
                	Eliminar_producto();
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

///////////////////////////// MAIN //////////////////////////////////////////////////
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

