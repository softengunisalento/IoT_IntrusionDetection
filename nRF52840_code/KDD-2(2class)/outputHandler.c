float expectedOutput[2] = { //array ottenuto direttamente dal dataset
	0.000000000000000000e+00,
	1.000000000000000000e+00
};
//l'output del modello sono 5 float. di questi va considerato il valore massimo
//l'indice del valore massimo all'interno dell'array, è associata all'indice
//del tipo di attacco
static int getMaxIndex(float* resultArray){ 
    int index=0;

    for(int i=1; i<2; i++){
        if(resultArray[index] < resultArray[i]){
            index=i;
        }
    }

    return index;
}

static char* getAtkByIndex(int index){
    switch (index) {
        case 0:{
            static char attack[] = "Attack";
            return attack;
        }
        case 1:{
            static char attack[] = "Normal";
            return attack;
        }
        default:{
            static char attack[] = "Ignoto";
            return attack;
        }
    }
}

static char* getOutput(float* resultArray){
	int index = getMaxIndex(resultArray);
    return getAtkByIndex(index);
}

static char* getExpectedOutput(){
	int index = getMaxIndex(expectedOutput);
	return getAtkByIndex(index);
}
