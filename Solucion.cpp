/*********************************
Mauricio Iván Cortés García
A00816689
ITESM
Análisis y Diseño de Algoritmos
Prof. Román Martinez
Tarea 5
*********************************/

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <windows.h>
using namespace std;
int minimo=0;
double NUM_MAXIMO = 999999999999;

/*
Nombre: Shortest_Points
Atributos: AIRPLANE_1 (string), AIRPLANE_2 (string), Distancia_Media (double), Constructor NULL.
*/
struct Shortest_Points
{
    string AIRPLANE_1;
    string AIRPLANE_2;
    double Distancia_Media;
    Shortest_Points()
    {
        AIRPLANE_1 = "NULL";
        AIRPLANE_2 = "NULL";
        Distancia_Media = NUM_MAXIMO;
    }
}Resultado;

/*
Nombre: Airplane
Atributos: ID (string), Coord_X (int), Coord_Y (int), Distancia_Final (double)
*/
struct Airplane
{
    string ID;
    int Coord_X;
    int Coord_Y;
    double Distancia_Final;
}Aux1, Aux2, Reporte[9907];

/*
Nombre: get_Min_Points
Entrada: Point_1 (Shortest_Points), Point_2 (Shortest_Points)
Salida: Minimo entre dos puntos (Shortest_Points)
Funcion: Calcular la distancia minima entre dos aviones.
*/
Shortest_Points get_Min_Points(Shortest_Points Point_1, Shortest_Points Point_2)
{
    return(Point_1.Distancia_Media<=Point_2.Distancia_Media) ? Point_1 : Point_2;
}

/*
Nombre: get_Distance_Airplanes
Entrada: Plane_1 (Airplane), Plane_2 (Airplane)
Salida: Distancia entre dos puntos con coordenada X (double)
Funcion: Calcular matemáticamente la distancia entre dos aviones con coordenadas (x,y).
*/
double get_Distance_Airplanes(Airplane Plane_1, Airplane Plane_2)
{
    return sqrt((Plane_1.Coord_X-Plane_2.Coord_X)*(Plane_1.Coord_X-Plane_2.Coord_X)+
                (Plane_1.Coord_Y-Plane_2.Coord_Y)*(Plane_1.Coord_Y-Plane_2.Coord_Y));
}

/*
Nombre: Particion
Entrada: Reporte[] (Airplane), Inicio (int), Final (int), &minimo (int)
Salida: void
Funcion: Ordenar ascendentemente los datos del reporte de aviones.
*/
void Particion(Airplane Reporte[], int Inicio, int Final, int &minimo)
{
    int Aux=Inicio;
    int Intercambio=Reporte[Inicio].Coord_X;
    for (int i=Inicio+1; i<=Final; i++)
        if (Reporte[i].Coord_X < Intercambio)
        {
            Aux++;
            Aux1 = Reporte[i];
            Reporte[i] = Reporte[Aux];
            Reporte[Aux] = Aux1;
        }
    minimo = Aux;
    Aux2 = Reporte[Inicio];
    Reporte[Inicio] = Reporte[minimo];
    Reporte[minimo] = Aux2;
}

/*
Nombre: Quick_Sort
Entrada: Reporte[] (Airplane), Inicio (int), Final (int)
Salida: void
Funcion: Formula recursiva para mandar ordenar datos del reporte de aviones.
*/
void Quick_Sort(Airplane Reporte[], int Inicio, int Final)
{
    if(Inicio < Final)
    {
        Particion(Reporte, Inicio, Final, minimo);
        Quick_Sort(Reporte, Inicio, minimo-1);
        Quick_Sort(Reporte, minimo+1, Final);
    }
}

/*
Nombre: Busqueda_Tres_Puntos
Entrada: Reporte[] (Airplane), Inicio (int), Final (int)
Salida: Punto con menor distancia (Shortest_Points)
Funcion: Calcular de manera tradicional (fuerza bruta) la distancia entre 3 puntos o menos y decidir cual es el menor.
*/
Shortest_Points Busqueda_Tres_Puntos(Airplane Reporte[], int Inicio, int Final)
{
    double Begin = NUM_MAXIMO;
    Shortest_Points Final_Point;
    for(int i=Inicio; i<Final; i++)//Algoritmo de fuerza bruta
        for(int j=i+1; j<=Final; j++)
            //Comparar los datos dentro de la mediana de rango con el punto medio y definir el punto menor.
            if(get_Distance_Airplanes(Reporte[i], Reporte[j]) <= Begin)
            {
                Final_Point.Distancia_Media = get_Distance_Airplanes(Reporte[i], Reporte[j]);
                Final_Point.AIRPLANE_1 = Reporte[i].ID;
                Final_Point.AIRPLANE_2 = Reporte[j].ID;
                Begin = get_Distance_Airplanes(Reporte[i], Reporte[j]);
            }
    return Final_Point;
}

/*
Nombre: Resuelve_Mediana
Entrada: Mediana (vector<Airplane>), Punto_Min (Shortest_Points)
Salida: Punto menor obtenido solamente en el rango de la mediana. (Shortest_Points)
Funcion: Calcular el punto menor de un vector con aviones.
*/
Shortest_Points Resuelve_Mediana(vector<Airplane> Mediana, Shortest_Points Punto_Min)
{
    for (int i = 0; i<Mediana.size()-1; i++)
        for (int j = i+1; j<Mediana.size() && (Mediana[j].Coord_X - Mediana[i].Coord_X) < Punto_Min.Distancia_Media; j++)
            if (get_Distance_Airplanes(Mediana[i],Mediana[j]) < Punto_Min.Distancia_Media)
            {
                Punto_Min.AIRPLANE_1 = Mediana[i].ID;
                Punto_Min.AIRPLANE_2 = Mediana[j].ID;
                Punto_Min.Distancia_Media = get_Distance_Airplanes(Mediana[i], Mediana[j]);
            }
    return Punto_Min;
}

/*
Nombre: Resuelve
Entrada: Reporte[] (Airplane), Inicio (int), Final (int), Num_Datos (int)
Salida: Punto minimo final de todo el reporte de aviones. (Shortest_Points)
Funcion: Aplica la tecnica de divide y venceras para calcular los puntos minimos de las particiones de 3 o menos puntos juntos
        se trata diferente el problema dependiendo si es par o impar, para manipular la Mediana con los datos sobrantes, se
        utiliza un vector y se utiliza la funcion Resuelve_Mediana, finalmente compara los datos obtenidos de las particiones
        y los de la Mediana y decide cual es el menor.
*/
Shortest_Points Resuelve(Airplane Reporte[], int Inicio, int Final, int Num_Datos)
{
    Shortest_Points Punto_Comp, Punto_Left, Punto_Right, Punto_Min;
    int Mitad = Num_Datos/2;
    Airplane Punto_Medio_Plano = Reporte[Mitad];
    if(Num_Datos<=3)//Realizar la busqueda de fuerza bruta cuando hay 3 o menos puntos a comparar.
        return Busqueda_Tres_Puntos(Reporte, Inicio, Final);
    if(Num_Datos%2==0)//Definir que funciones utilizar si la cantidad de datos es par o impar.
    {
        Punto_Left = Resuelve(Reporte, Inicio, (Inicio+Mitad)-1, Mitad);
        Punto_Right = Resuelve(Reporte, Inicio+Mitad, Final, Num_Datos-Mitad);
    }
    else
    {
        Punto_Left = Resuelve(Reporte, Inicio, Inicio+Mitad, Mitad+1);
        Punto_Right = Resuelve(Reporte, (Inicio+Mitad)+1, Final, Num_Datos-(Num_Datos-Mitad));
    }
    Punto_Comp=get_Min_Points(Punto_Left,Punto_Right);//Obtener el menor de los puntos izq y derecha de la particion.
    int Cont=0;
    vector<Airplane> Mediana;
    for(int i=Inicio; i<=Final; i++)//Introducir los datos al vector mediana.
        if(abs(Reporte[i].Coord_X-Punto_Medio_Plano.Coord_X)<=Punto_Comp.Distancia_Media)
        {
            Mediana.push_back(Reporte[i]);
            Cont++;
        }
    if(!Mediana.empty())//Comparar el punto menor del vector mediana contra el punto menor de las 2 particiones.
        Punto_Min=get_Min_Points(Punto_Comp, Resuelve_Mediana(Mediana, Punto_Comp));
    else
        Punto_Min=Punto_Comp;
    return Punto_Min;
}

int main()
{
    int Cont_Ren=0;
    ifstream Archivo_Entrada;
    string Nombre_Archivo;
    //Se le pide el nombre del archivo al usuario.
    cout << endl <<"Introduce el nombre del archivo con los datos de vuelos: " << endl;
    cin >> Nombre_Archivo;
    //Se le agrega la extension .txt
    Nombre_Archivo+=".txt";
    //Se abre el archivo.
    Archivo_Entrada.open(Nombre_Archivo.c_str());
    //Se leen los datos del archivo y se acumulan en el reporte.
    while(!Archivo_Entrada.eof())
    {
        Archivo_Entrada >> Reporte[Cont_Ren].ID;
        Archivo_Entrada >> Reporte[Cont_Ren].Coord_X;
        Archivo_Entrada >> Reporte[Cont_Ren].Coord_Y;
        Cont_Ren++;
    }
    //Se mandan ordenar las coordenadas en X con los datos.
    Quick_Sort(Reporte, 0, (Cont_Ren-1)-1);
    //Se manda llamar la funcion general.
    Resultado = Resuelve(Reporte, 0, (Cont_Ren-1)-1, Cont_Ren-1);
    //Se despliegan los datos de la distancia y los puntos finales mas cercanos.
    cout << endl << endl << "Puntos mas cercanos en el plano " << endl;
    cout << "Punto A: " << Resultado.AIRPLANE_1 << endl;
    cout << "Punto B: " << Resultado.AIRPLANE_2 << endl;
    cout << "Distancia: " << Resultado.Distancia_Media << endl << endl;
    //Se cierra el archivo.
    Archivo_Entrada.close();
    system("PAUSE");
    return 0;
}

/**********************************  REFERENCIAS  *****************************************
https://en.wikipedia.org/wiki/Closest_pair_of_points_problem
http://www.mat.uson.mx/omrodriguez/algoritmos/Comparativa/index.html
http://www.geeksforgeeks.org/closest-pair-of-points/
http://www.algoritmia.net/articles.php?id=34
https://rosettacode.org/wiki/Closest-pair_problem
http://www.sanfoundry.com/cpp-program-find-closest-pair-points-array/
http://gamedev.stackexchange.com/questions/14373/in-2d-how-do-i-efficiently-find-the-nearest-object-to-a-point
https://tfetimes.com/c-closest-pair-problem/

En estos links se muestran diferentes ejemplos de como resolver total o parcialmente el reto
de los puntos más cercanos, pero inicialmente me apoyé principalmente en lograr entender el
concepto del problema gráficamente para captar mejor la idea de lo que se tenía que hacer.
Existen muchas maneras de resolver éste problema y cuando surgían problemas de programación,
utilizaba métodologías realizadas en otros ejemplos para complementar mi código. Me fue de
mucha ayuda principalmente los primeros cinco links ya que con ellos logré realizar la mayor
parte del problema y con los demás complementé mis ideas.
*******************************************************************************************/
