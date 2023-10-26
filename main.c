#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    int id;
    char nombre[20];
    char apellido[20];
    char dni[10];
    int edad;
    char genero;
} stPersona;

typedef struct nodo
{
    stPersona dato;
    struct nodo * siguiente;
    struct nodo * anterior;
} nodo;

typedef struct {
 int * arreglo;
 int posTope;
 }Pila;

nodo * iniclista();
nodo * crearNodo(stPersona dato);
nodo * agregarPrincipio(nodo * lista, nodo * nuevo);
nodo * agregarOrdenado(nodo * lista, nodo * nuevo);
nodo * cargarNodos(nodo * lista, char nombreAr[]);
nodo * buscarUltimo(nodo * lista);
nodo * pasarJovenes(nodo * lista, nodo ** menores);

int main()
{
    char nombreAr[] = "archivoPersonas.bin";
    nodo * lista = iniclista();
    lista = cargarNodos(lista, nombreAr);
    printf("\n           TODOS:               \n");
    mostrarLista(lista);

    nodo * menores = iniclista();
    lista = pasarJovenes(lista, &menores);
    printf("\n           MENORES:               \n");
    mostrarLista(menores);
    printf("\n           MAYORES:               \n");
    mostrarLista(lista);

    Pila p;
    inicPila(&p);
    apilarPrimeros(lista, &p);
    mostrarPila(p);
    return 0;
}

nodo * iniclista()
{
    return NULL;
}

nodo * crearNodo(stPersona dato)
{
    nodo * aux = (nodo*) malloc(sizeof(nodo));
    aux->dato = dato;
    aux->siguiente = NULL;
    aux->anterior = NULL;

    return aux;
}

nodo * agregarPrincipio(nodo * lista, nodo * nuevo)
{
    if(lista == NULL)
    {
        lista = nuevo;
    }
    else
    {
        nuevo->siguiente = lista;
        lista->anterior = nuevo;
        lista = nuevo;
    }
    return lista;
}

nodo * agregarOrdenado(nodo * lista, nodo * nuevo)
{
    if(lista == NULL)
    {
        lista = nuevo;
    }
    else
    {
        if(nuevo->dato.edad < lista->dato.edad)
        {
            lista = agregarPrincipio(lista, nuevo);
        }
        else
        {
            nodo * seg = lista->siguiente;
            nodo * ante = lista;
            while(seg != NULL && nuevo->dato.edad > seg->dato.edad)
            {
                ante = seg;
                seg = seg->siguiente;
            }
            ante->siguiente = nuevo;
            nuevo->anterior = ante;
            nuevo->siguiente = seg;
            if(seg != NULL)
            {
                seg->anterior = nuevo;
            }
        }
    }
    return lista;
}

nodo * cargarNodos(nodo * lista, char nombreAr[])
{
    stPersona aux;
    FILE * buffer = fopen(nombreAr, "rb");
    if(buffer)
    {
        while(fread(&aux,sizeof(stPersona),1, buffer) > 0)
        {
            nodo * nuevo = crearNodo(aux);
            lista = agregarOrdenado(lista, nuevo);
        }
    }
    fclose(buffer);
    return lista;
}

void mostrarLista(nodo * lista)
{
    nodo * seg = buscarUltimo(lista);
    while(seg->anterior != NULL)
    {
        escribir(seg->dato);
        seg = seg->anterior;
    }
}

void escribir(stPersona p)
{
    printf("\nID...................................: %d", p.id);
    printf("\nNombre y Apellido..........: %s %s",p.nombre,p.apellido);
    printf("\nDNI.................................: %s",p.dni);
    printf("\nEdad...............................: %d",p.edad);
    printf("\nGenero............................: %s \n",(p.genero=='f')?"Femenino":"Masculino");
}

nodo * buscarUltimo(nodo * lista)
{
    nodo * seg = lista;
    while(seg->siguiente != NULL)
    {
        seg = buscarUltimo(seg->siguiente);
    }
    return seg;
}

nodo * pasarJovenes(nodo * lista, nodo ** menores)
{
    nodo * pasar;
    while(lista != NULL&& lista->dato.edad < 30)
    {
        pasar = lista;
        lista = lista->siguiente;
        pasar->siguiente = NULL;
        pasar->anterior = NULL;
        *menores = agregarPrincipio(*menores, pasar);
    }
    return lista;
}

void inicPila(Pila * p){
    p->arreglo = (int*) malloc(sizeof(int)*10);
    p->posTope = 0;
}

void apilar(Pila * p, int dato){
    p->arreglo[p->posTope] = dato;
    (p->posTope)++;
}

void mostrarPila(Pila p){
    int i = 0;
    printf("\nBASE.....................................TOPE\n");
    while(i < p.posTope){
        printf(" %d " , p.arreglo[i]);
        i++;
    }
    printf("\nBASE.....................................TOPE\n");
}

void apilarPrimeros(nodo * lista, Pila * p){
    nodo * seg = lista;
    while(seg != NULL && p->posTope < 10){
            apilar(p, seg->dato.id);
            seg = seg->siguiente;
    }
}
