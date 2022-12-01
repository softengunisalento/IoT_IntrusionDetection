float expectedOutput[5] = { //array ottenuto direttamente dal dataset
	1.000000000000000000e+00,
	0.000000000000000000e+00,
	0.000000000000000000e+00,
	0.000000000000000000e+00,
	0.000000000000000000e+00,
};
//l'output del modello sono 5 float. di questi va considerato il valore massimo
//l'indice del valore massimo all'interno dell'array, è associata all'indice
//del tipo di attacco
static int getMaxIndex(float* resultArray){ 
    int index=0;

    for(int i=1; i<5; i++){
        if(resultArray[index] < resultArray[i]){
            index=i;
        }
    }

    return index;
}

static char* getAtkByIndex(int index){
    switch (index) {
        case 0:{
            static char attack[] = "DoS";
            return attack;
        }
        case 1:{
            static char attack[] = "Normal";
            return attack;
        }
        case 2:{
            static char attack[] = "Probe";
            return attack;
        }
        case 3:{
            static char attack[] = "R2L";
            return attack;
        }
        case 4:{
            static char attack[] = "U2R";
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
