![unilogo-2](https://user-images.githubusercontent.com/100310104/205038405-88cf7e7b-57c1-49bf-887f-caa3f0640c01.png)

# Un modulo di Intrusion Detection basato su tecniche di Intelligenza Artificiale per l’identificazione di attacchi su infrastrutture IoT critiche

Il progetto nasce con lo scopo di testare la capacità di dispositivi IoT dalle risorse limitate di rilevare intrusioni nelle reti attraverso l'analisi del traffico mediante ML.

## Struttura directory

Il progetto si articola nelle seguenti directory:
- *colab_code*:
	- contiene sia i file formato **.ipynb** relativi ai vari modelli ML testati
	- contiene i file **.txt** relativi al dataset. 
	- **PS.** è necessario cambiare i percorsi all'interno dei file .ipynb al fine di caricare correttamente i file del dataset
- *measurements*:
	- *photos*:
		- contiene foto scattate durante la misura dei consumi in laboratorio
	- *no_code*:
		- contiene file relativi ai consumi del dispositivo nel caso in cui non vi sia codice flashato
	- *NSL-KDD 2class*:
		- contiene i consumi del dispositivo nel caso in cui vi sia in esecuzione l'algoritmo ML addestrato in **Classificazione Binaria.ipynb**
	- *NSL-KDD 5class*:
		- contiene i consumi del dispositivo nel caso in cui vi sia in esecuzione l'algoritmo ML addestrato in **Sequenziale_5_Classi.ipynb**
	- *sine prevision*:
		- contiene i consumi del dispositivo nel caso in cui vi sia in esecuzione l'algoritmo ML di test **Hello World** nella directory */contiki-ng-feature-project/examples/tensorflowlite* di Contiki-NG
- *nRF52840_code*:
	- *CoapTestAnomaly*:
		- contiene il codice da eseguire nativamente su macchina unix al fine di testare la comunicazione col **Server CoAP**. Il programma non esegue il codice di ML
		- *resources*:
			- contiene le **risorse CoAP** esposte dal server
	- *KDD-1 (5class)*: 
		- contiene il codice da flashare sul dispositivo contenente il modello allenato in **Sequenziale_5_Classi\[Shuffle\].ipynb**
	- *KDD-2 (2class)*:
		- contiene il codice da flashare sul dispositivo contenente il modello allenato in **Classificazione binaria.ipynb**
	- *KDD-2 (2class) (coap)*:
		- contiene il codice da flashare sul dispositivo contenente server CoAP e modello ML per la classificazione binaria
- *Tesi_Triennale_Culcea_Cezar_Narcis.pdf*:
	- lavoro di tesi che descrive il progetto.

## Setup ambiente di sviluppo

Al fine di ottenere un ambiente di sviluppo di partenza è stato seguito il seguente articolo:
- [Running TensorFlow Lite for Microcontrollers on Contiki-NG | by Atis Elsts | Medium](https://atiselsts.medium.com/running-tensorflow-lite-for-microcontrollers-on-contiki-ng-d938f25193f5)

Link: https://atiselsts.medium.com/running-tensorflow-lite-for-microcontrollers-on-contiki-ng-d938f25193f5

**NB.**: al fine di non avere problemi di librerie, è consigliabile lavorare con i file contenuti in *nRF52840_code* all'interno della directory */contiki-ng-feature-project/examples/*


