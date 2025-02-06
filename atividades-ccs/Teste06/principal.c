#include "ezdsp5502.h"
#include "stdio.h"

int testeFalhou = (int)-1;
extern Int16 oled_test();

void pararTeste(){
    return;
}

void main(void) {

    // Inicializa a placa
    EZDSP5502_init();

    printf("\nTeste do OLED...\n");

    testeFalhou = oled_test();

    //Verifica o teste
    if(testeFalhou != 0){
        printf("\n         TESTE FALHOU!!!          \n", testeFalhou);
        pararTeste();
    }
    else{
        printf("\n      TESTE PASSOU!!!        \n");
    }
}

