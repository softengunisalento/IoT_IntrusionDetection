//l'output del modello sono 2 float. di questi va considerato il valore massimo
//l'indice del valore massimo all'interno dell'array, è associata all'indice
//del tipo di attacco
static int getMaxIndex(float* resultArray){ 
    int index=0;

    for(int i=1; i<2; i++){
        if(resultArray[index] < resultArray[i]){
            index=i;
        }
    }
    
    return index; //1 = Normal, 0 = Attack
}

static int getOutput(float* resultArray){
    return getMaxIndex(resultArray);
}


