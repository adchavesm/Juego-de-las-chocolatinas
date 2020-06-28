#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <limits.h>

#define Q_LIMIT 1000
#define BUSY 1
#define IDLE 0
#define MAX_TELLERS 20

using namespace std;

//Funcion Factorial
int fact(int n)
{
    if (n == 0)
        return 1;
    return n * fact(n - 1);
}

//Funcion que nos permite simular una aleatoriedad al abrir una chocolatina
int obtener_lamina(double prob_laminas[], int size)
{
    double r = rand();
    int n = 0;
    for (int i = 0; i < size; ++i)
    {
        n += INT_MAX * prob_laminas[i];
        if (r <= n)
        {
            return i;
        }
    }
    return -1;
}

//Funcion que permite saber cual es la siguiente persona en abrir una chocolatina
int why_next_time(double matriz[], int n)
{
    int next;
    int menor = 99999;
    for (int i = 0; i < n; i++)
    {
        if (matriz[i] >= 0 && matriz[i] < menor)
        {
            menor = matriz[i];
            next = i;
        }
    }
    return next;
}

//Funcion iterativa en cada ciclo que valida si alguna persona ya lleno su album por completo
void validar_ganador(int matriz[][500], int n, int m, int *persona)
{
    bool res = true;
    for (int i = 0; i < n; i++)
    {
        res = true;
        for (int j = 0; j < m; j++)
        {

            if (matriz[i][j] != 0)
            {
            }
            else
            {
                res = false;
            }
        }
        if (res == true)
        {
            *persona = i;
            break;
        }
    }
}

bool el_agente_lleno_album(int matriz[][500], int n, int m)
{
    bool lleno_el_album = true;
    for (int z = 0; z < m; z++)
    {
        if (matriz[n][z] == 0)
        {
            lleno_el_album = false;
            break;
        }
    }
    if (lleno_el_album == true)
    {
        return true;
    }
    return false;
}

//valida cuando todos han ganado
bool validar_final_simulation(int matriz[][500], int n, int m)
{
    bool res = true;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {

            if (matriz[i][j] == 0)
            {
                res = false;
                break;
            }
        }
    }
    return res;
}

//validar numero de laminas promedio
int numero_laminas_promedio(int matriz[][500], int n, int m)
{
    int promedio_de_laminas = 0;
    int numero_laminas_agente;
    for (int i = 0; i < n; i++)
    {
        numero_laminas_agente = 0;
        for (int j = 0; j < m; j++)
        {
            numero_laminas_agente += matriz[i][j];
        }
        promedio_de_laminas = (promedio_de_laminas + numero_laminas_agente) / 2;
    }
    return promedio_de_laminas;
}

int main()
{
    //Variable momentanea que servira para guardar las probabilidades insertadas por el usuario
    double p;

    //Variable para almacenar la cantidad de laminas
    int N;

    //Variable que nos permitira saber que forma de insertar las probabilidades de las laminas quiere le usuario
    int forma;
    double suma;

    //Variable para validar errores o inconsistencias
    bool flag;

    //Variable para insertar que familia de distibucion para la simulacion
    int familia;
    cout << "Ingrese número de láminas: ";
    cin >> N;

    //Se pregunta de que forma se quiere llenar la tabla de probabilidades de las laminas
    cout << "Forma de llenar probabilidades (1=tabla, 2=familia paramétrica): ";
    cin >> forma;

    //El maximo de laminas posible es 500
    double prob_laminas[500];
    //Generacion de tabla de probabilidades de laminas
    switch (forma)
    {
    case 1:

        do
        {

            suma = 0;
            flag = false;

            for (int i = 0; i < N; ++i)
            {

                cout << "Probabilidad de lámina número " << i << ": ";
                cin >> prob_laminas[i];
                suma += prob_laminas[i];
            }

            if (suma != 1)
            {

                flag = true;
                cout << "Las probabilidades no suman 1, por favor intentelo de nuevo" << endl;
            }

        } while (flag);

        break;

    default:

        cout << "Seleccione familia paramétrica (1=Uniforme, 2=Geometrica truncada, 3=Binomial): ";
        cin >> familia;
        cout << endl;
        switch (familia)
        {
        case 1:

            cout << "Usando distribución uniforme...\n"
                 << endl;

            for (int i = 0; i < N; ++i)
                prob_laminas[i] = 1.0 / N;

            break;

        case 2:

            cout << "Usando distribución Geométrica truncada...\n"
                 << endl;

            cout << "Defina p: ";
            cin >> p;

            for (int i = 0; i < N; ++i)
                prob_laminas[i] = (p * p * pow(1.0 - p, double(i))) / (p * (1 - pow(1 - p, double(N))));

            break;

        case 3:

            cout << "Usando distribución Binomial con n = " << N - 1 << "...\n";

            cout << "Defina p: ";
            cin >> p;

            for (int i = 0; i < N; ++i)
                prob_laminas[i] = (double(fact(N - 1)) / (fact(i) * fact(N - 1 - i))) * pow(p, double(i)) * pow(1.0 - p, double(N - 1.0 - i));

        default:
            break;
        }

        break;
    }

    cout << "\nProbabilidades asignadas a las láminas:" << endl;

    for (int i = 0; i < N; ++i)
    {

        cout << i << ": " << prob_laminas[i] << endl;
    }

    srand(time(NULL));

    //Definimos el numero de agentes de la red
    double r;
    int n_agentes;
    cout << "\nIngrese la probabilidad de que cualquieras par de nodos sean amigos en esta red social: ";
    cin >> p;
    //Definimos la cantidad maxima de agentes que permite la simulacion, en este caso seran maximo 100 agentes
    bool agentes[100][100] = {false};

    cout << "\nIngrese cantidad de agentes que quiere en el sistema: ";
    cin >> n_agentes;

    // Generamos la matriz de adjacencia de el grafo, para simular las amistades
    for (int i = 0; i < n_agentes; ++i)
    {
        for (int j = i + 1; j < n_agentes; ++j)
        {
            r = rand();
            agentes[i][j] = false;
            agentes[j][i] = false;
            if (r <= p * INT_MAX)
            {
                agentes[i][j] = true;
                agentes[j][i] = true;
            }
        }
    }

    //Mostramos la matriz de adjacencia
    cout << "\nMatriz de adyacencia generada por grafo aleatorio: " << endl;
    for (int i = 0; i < n_agentes; ++i)
    {
        for (int j = 0; j < n_agentes; ++j)
        {
            if (i == j)
                agentes[i][j] = false;
            cout << agentes[i][j] << ' ';
        }
        cout << endl;
    }

    /** SIMULACION **/

    //Tiempo actual
    double current_time = 0;

    //Proximos tiempos, los agentes compran una chocolatina cada x tiempo, estos tiempos se generan con una distribucion de probabilidad
    //Almacena
    double times[n_agentes];

    //album de las personas
    int album[100][500] = {0};

    //Variable para almacenar la persona ganadora
    int win_person = -1;

    //Variable para guardar la lamina que salio en la chocolatina
    int open_chocolate;

    //Aqui consultamos cual es la proxima persona en abrir una chocolatina
    int next_person = why_next_time(times, n_agentes);

    //Condicion final
    bool final = false;
    cout << endl;

    while (!final)
    {

        current_time = times[next_person]; //establecemos el tiempo actual en el tiempo de la persona que abrira una chocolatina

        times[next_person] = current_time + rand() % 20; // establecemos la proxima abertura de ficha que tendra este agente

        open_chocolate = obtener_lamina(prob_laminas, N); //Lamina que le salio al agente al abrir su chocolatina

        //validar si tiene la lamina
        if (album[next_person][open_chocolate] == 0)
        {
            album[next_person][open_chocolate] = 1;

            if (el_agente_lleno_album(album, next_person, N))
            {
                times[next_person] = -1;
                cout << endl
                     << "El agente: " << next_person+1<< " lleno el album" << endl;
            }
        }
        else
        {
            album[next_person][open_chocolate]++;
            //encontrar amigos

            //Intercambio de laminas
            for (int i = 0; i < n_agentes; i++)
            {
                //Buscamos a un amigo que le falte la chocolatina que nos salio
                if (agentes[next_person][i] == 1 && album[i][open_chocolate] == 0)
                {
                    for (int j = 0; j < N; j++)
                    {
                        if (album[i][j] > 1 && album[next_person][j] == 0) //encontramos una chocolatina que nos sirve
                        {
                            cout << endl
                                 << "INTERCAMBIO" << endl;
                            cout << "     Antes del intercambio de la persona: " << next_person+1 << " con " << i+1 << endl;
                            cout << "     Album de la persona: " << next_person+1 << endl<<"       ";
                            for (int z = 0; z < N; z++)
                            {
                                cout << album[next_person][z] << " ";
                            }
                            cout << endl
                                 << "     Album de la persona: " << i+1 << endl<<"       ";
                            for (int z = 0; z < N; z++)
                            {
                                cout << album[i][z] << " ";
                            }
                            cout << endl;

                            //Hacemos el intercambio
                            album[i][j]--;
                            album[next_person][j]++;
                            album[i][open_chocolate]++;
                            album[next_person][open_chocolate]--;

                            //Imprimimos la informacion del intercambio
                            cout << "     Despues del intercambio de la persona: " << next_person+1 << " con " << i+1 << endl;
                            cout << "     Album de la persona: " << next_person+1 << endl<< "      ";
                            for (int z = 0; z < N; z++)
                            {
                                cout << album[next_person][z] << " ";
                            }
                            cout << endl
                                 << "     Album de la persona: " << i+1 << endl<<"       ";
                            for (int z = 0; z < N; z++)
                            {
                                cout << album[i][z] << " ";
                            }
                            cout << endl;

                            //Consultamos si la persona que abrio la chocolatina ya lleno su album
                            if (el_agente_lleno_album(album, next_person, N))
                            {
                                times[next_person] = -1;
                                cout << endl
                                     << "El agente: " << next_person+1 << " "
                                     << "lleno el album" << endl;
                            }
                            //Consultamos si la persona que acepto el intercambio lleno su album
                            if (el_agente_lleno_album(album, i, N))
                            {
                                times[i] = -1;
                                cout << endl
                                     << "El agente: " << i+1 << " "
                                     << "lleno el album" << endl;
                            }

                            break;
                        }
                    }
                }
            }
        }
        next_person = why_next_time(times, n_agentes);
        final = validar_final_simulation(album, n_agentes, N);
    }
    cout<<endl<<"                                                         FIN DE LA SIMULACION ";

    int laminas_promedio = numero_laminas_promedio(album, n_agentes, N);

    cout << endl
         << "RESULTADOS" << endl
         << endl;
    for (int l = 0; l < n_agentes; l++)
    {
        cout << "Agente " << l + 1 << ", Resultado de su album ";
        for (int k = 0; k < N; k++)
        {
            cout << album[l][k] << " ";
        }
        cout << endl;
    }

    cout << endl;
    return 0;
}
