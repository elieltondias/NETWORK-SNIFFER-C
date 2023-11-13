#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct pcap_pkthdr header;
  const uint8_t *packet;

  //Abre a interface de rede wireless
  
  int tamanho_maximo_do_pacote;
  printf("QUAL O TAMANHO MAXIMO DO PACOTE(em bytes)?\n");
  scanf("%i", &tamanho_maximo_do_pacote);

  if (tamanho_maximo_do_pacote == NULL){
    int tamanho_maximo_do_pacote = 1000000;
  }else if(tamanho_maximo_do_pacote == 0){
   int tamanho_maximo_do_pacote = 1000000;
  }

  int tempo_limite;
  printf("QUAL O TEMPO LIMITE PARA CAPTURAR CADA PACOTE? (em ms)\n");
  scanf("%i", &tempo_limite);

  if (tempo_limite == NULL){
    int tempo_limite = 1000 ;
  }else if(tempo_limite == 0){
    int tempo_limite = 1000;
  }
  

  // handle = pcap_open_live("interface de rede", tamanho do pacote em bytes, 1 para ativar o modo promiscuo, tempo limite para capturar o pacote, errbuf para n gravar erros)
  handle = pcap_open_live("wlan0", tamanho_maximo_do_pacote, 1, tempo_limite, errbuf); 
  // Wlan0 é a interface de rede que sera usada, logo em seguida o tamanho maximo do pacote 1000000(definida em bytes) igual a 1mb
  // O valor 1 define o modo promiscuo se for 1, a interface captura todos os pacotes(inclusive os que são destinados a outras maquinas)
  // o valor 1000 define o tempo limite para a captura do pacote em ms
  // errbuf n permite que mensagens de erro sejam armazenadas
  
  if (handle == NULL) {
    printf("Erro ao abrir a interface de rede: %s\n", errbuf);
    return 1;
  }

  //Cria a pasta para armazenar os pacotes
  char *pasta = "PascotesCapturados";

  int status = mkdir(pasta, 0755);

  //Verifica se a pasta foi criada com sucesso
  if (status == 0) {
    printf("Pasta '%s' criada com sucesso.\n", pasta);
  } else {
    printf("Erro ao criar a pasta '%s'.\n", pasta);
  }

  //Captura os pacotes
  printf("A captura de pacotes foi iniciada!");

  while(1){
    packet = pcap_next(handle, &header);
    if (packet == NULL) {
      break;
    }

    //Grava o pacote em um arquivo
    char filename[255];
    sprintf(filename, "PascotesCapturados/%d.pcap", header.ts.tv_sec);
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
      printf("Erro ao abrir o arquivo: %s\n", filename);
      return 1;
    }
    fwrite(packet, header.caplen, 1, fp);
    fclose(fp);
  }
 //Fecha a interface de rede
  pcap_close(handle);

  return 0;
}