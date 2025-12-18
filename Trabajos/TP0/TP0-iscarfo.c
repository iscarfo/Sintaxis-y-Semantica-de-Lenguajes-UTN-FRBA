//Punto 3: Hola Mundo

#include <stdio.h>

int main() 
{
    printf("Hola Mundo\n");
    return 0;
}


//Punto 4:

#include <stdio.h>

int main() {
    char nombre[50+1]; 
    
    printf("Ingrese su nombre: ");
    
    scanf("%s", nombre);
    
    printf("Hola %s!\n", nombre);
    
    return 0;
}

