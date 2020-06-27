#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include "lcgrand.h"

#define Q_LIMIT 1000
#define BUSY 1
#define IDLE 0
#define MAX_TELLERS 20

using namespace  std ;


int why_next_time(double matriz[], int n){
    int next;
    int menor=99999;
    for ( int i = 0; i < n; i++)
    {
        if (matriz[i]< menor){
            menor=matriz[i];
            next=i;
        }
    }
    return next;
}

bool validar_final( int matriz[][10],int n, int m, int* persona){
    bool res=true;
    for (int i = 0; i < n; i++)
    {   
        res=true;
        for (int j = 0; j < m; j++)
        {
            
            if (matriz[i][j]!= 0 ){
            }else{
                res=false;
            }
        }
        if (res==true){
            *persona = i;
            break;
        }        
    }
    return res;
}
int main()
{   
        srand(time(NULL));
    //Numero de personas
    int n=5;
    
    //Numero de laminas
    int m=10;
    
    //Matriz de adjacensia
    /*
    {{0,1,0,1,0}, 
     {1,0,0,1,0}, 
     {0,0,0,0,1}, 
     {1,1,0,0,1}, 
     {0,0,1,1,0}};
    */
    int m_adj[n][n]={{0,1,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {0,0,1,0,1}, {1,0,1,1,0}};

    //album de las personas
    int album[n][10]={{0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}};
    //
    double matriz_prob_sheets[m]={0.05, 0.1 , 0.15, 0.05,0.15,0.05, 0.1 , 0.15, 0.05,0.15};
    
    //Tiempo actual
    double current_time=0;
    
    //Proximos tiempos
    double times[n]={23,45,23,87,60};
    
    /*
    for (int i = 0; i < n; i++)
    {
        times[i]=0.2;
    }

    */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout<< m_adj[j][i] <<" ";
        }
        cout<<endl;
    }
    
    cout<<endl;
    
    for (int j = 0; j < m; j++)
    {
        cout<< matriz_prob_sheets[j] <<" ";
    }

    //Planteamiento
    int win_person=-1;

    int open_chocolate;

    int next_person= why_next_time(times, n);
    bool final=validar_final(album, n, m, &win_person);
    cout<< endl;
    while( !final ){
    /*    cout<<"el final es"<<next_person<<endl;
        for (int k = 0; k < n; k++)
         {
             cout<<times[k]<<" ";
         }
         cout<<endl;
        
     /*for (int l = 0; l < n ; l++)
     {
         for (int k = 0; k < m; k++)
         {
             cout<<album[l][k]<<" ";
         }
         cout<<endl;
         
     }
       */ 
        //
        
        current_time=times[next_person];

        times[next_person]=current_time+rand() % 20; // + VARIABLE ALEATORIA
        
        open_chocolate = rand() % 10; //variable aleatoria entre 0 y m
        
        //validar si tiene la lamina
        if (album[next_person][open_chocolate] == 0)
        {
            album[next_person][open_chocolate]=1;
        }else{
            album[next_person][open_chocolate]++;
            //encontrar amigos

            //Variable para validar si intercambio o no la lamina
            int change=false;

            //Intercambio de laminas
            for (int i = 0; i < n; i++)
            {
                //Buscamos a un amigo que le falte la chocolatina que nos salio
               if (m_adj[next_person][i]==1 && album[i][open_chocolate]==0 )
               {
                   for (int j = 0; j < m; j++)
                   {
                       if ( album[i][j] > 1 && album[next_person][j]==0 ) //encontramos una chocolatina que nos sirve
                       {
                            cout<<endl<<"      INTERCAMBIO"<<endl;
                            cout<<"Antes del intercambio de la persona: "<<next_person<<" con "<<i<<endl;
                            cout<<"Album de la persona: "<<next_person<<endl;
                           for (int z = 0; z < m; z++)
                           {
                               cout<<album[next_person][z]<<" ";
                           }
                           cout<<endl<<"Album de la persona: "<<i<<endl;
                           for (int z = 0; z < m; z++)
                           {
                                cout<<album[i][z]<<" ";
                           }
                           cout<<endl;
                           album[i][j]--;
                           album[next_person][j]++;
                           album[i][open_chocolate]++;
                           album[next_person][open_chocolate]--;
                           change=true;
                            cout<<"Despues del intercambio de la persona: "<<next_person<<" con "<<i<<endl;
                            cout<<"Album de la persona: "<<next_person<<endl;
                           for (int z = 0; z < m; z++)
                           {
                               cout<<album[next_person][z]<<" ";
                           }
                           cout<<endl<<"Album de la persona: "<<i<<endl;
                           for (int z = 0; z < m; z++)
                           {
                                cout<<album[i][z]<<" ";
                           }
                           cout<<endl;
                           
                       }
                       
                   }
                   
               }

            }
            //Validamos si se hizo el intercambio, de lo contrario la persona se queda con la lamina repetida
            if (change==false)
            {
                album[next_person][open_chocolate]++;
            }   
        }
        next_person= why_next_time(times, n);
        final = validar_final(album, n, m, &win_person);
    }
    
    cout<<"El ganador fue, el agente: "<<win_person<< endl;
    for (int l = 0; l < n ; l++)
     {
         cout<<"Agente: "<<l+1<<", Resultado de su album ";
         for (int k = 0; k < m; k++)
         {
             cout<<album[l][k]<<" ";
         }
         cout<<endl;
         
     }

    cout << endl;
    return 0;
}


