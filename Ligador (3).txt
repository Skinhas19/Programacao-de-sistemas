#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // para usar istringstream
#include <unordered_map> // para usar unordered_map

using namespace std;
//usado para comparar as instruçoes do arquivo e ver se tem essa instruçao na lista
string instrucao[13] = {
    "LA", "SA", "ADD", "MUL", "DIV", "SUB",
    "JMP", "JEQ", "JGT", "JLT", "PW", "RW", "STOP"
};

//mapa para colocar as instruçoes em decimal no arquivo do gerar codigo
unordered_map<string, string> instrucao_codigo = {
    {"LA", "00"}, {"SA", "01"}, {"ADD", "02"}, {"MUL", "03"},
    {"DIV", "04"}, {"SUB", "05"}, {"JMP", "06"}, {"JEQ", "07"},
    {"JGT", "08"}, {"JLT", "09"}, {"PW", "10"}, {"RW", "11"}, {"STOP", "12"}
};

struct tabela {
    string simbolo;
    int endereco;
};

int tem=0;
int tabcont = 0;
int cont=0;
int space=-1;
int endcont=0;
tabela tab[50];
void criar_tabela_de_simbolos(string arquivo);
void gerar_codigo(string arquivo);
int encontra_endereco(string simbolo,int tamanho);
int comparar_palavra(string p);
void comparar_endereco();
//pega ultima linha do codigo decimal
string ler_ultima_linha(string arquivo) {
    ifstream leitura(arquivo);
    string linha;
    string ultima_linha;

    if (!leitura.is_open()) {
        cout << "Erro ao abrir o arquivo." << "\n";
        exit(EXIT_FAILURE);
    }

    // Ler todas as linhas e armazenar a última lida
    while (getline(leitura, linha)) {
        ultima_linha = linha;
    }

    leitura.close();
    return ultima_linha;
}
// Função para encontrar o endereço de um símbolo na tabela
int encontra_endereco(string simbolo,int tamanho) {
    
    for (int i = 0; i < tamanho; i++) {
        if (tab[i].simbolo == simbolo) {
            return tab[i].endereco;
        }
    }
    return -1; // Não encontrado
}

// Funcao para encontrar a instrucao
int comparar_palavra(string p)
{
    for(int i=0;i<13;i++)
    {
        if(p == instrucao[i])
        {
            return i;

        }
    }
    return (-1); 
}

//para arrumar os enderecos dos spaces,para nao dar conflito com os jmps
void comparar_endereco()
{
    for(int i=0;i<tabcont;i++)
    {
        if(space<tab[i].endereco)
        {
            tab[i].endereco++;
        }
        if(space==tab[i].endereco)
        {
            tab[i].endereco++;
        }
        
    }
}

//Funçao para gerar o arquivo com as instruçoes e os endereços em decimal
void gerar_codigo(string arquivo,string saida)
{
  
    string linha;
    string palavra;
    string palavra1, palavra2, palavra3;
    ifstream leitura(arquivo);
    ofstream escrita(saida,ios::app);

    //verificar se vai abrir os arquivos corretamente
    if (!leitura.is_open() || !escrita.is_open()) {
        cout << "\nErro ao abrir os arquivos." << "\n";
        exit(EXIT_FAILURE);
    }
    if(cont==0)
    {
        escrita<<space<< " " << 0 <<"\n";
       cont=1;
    }    
    //pega a linha inteira 
    while (getline(leitura, linha)) {
        // le palavra por palavra da linha
        istringstream stream_linha(linha);
        
        int contagem_palavras = 0;
        int endereco;

        // Conta as palavras e armazena as três primeiras
        // Armazenando uma palavra por vez na variavel "palavra" ate acabar a linha
        while (stream_linha >> palavra) {
            contagem_palavras++;
            if (contagem_palavras == 1) {
                palavra1 = palavra;
            } else if (contagem_palavras == 2) {
                palavra2 = palavra;
            } else if (contagem_palavras == 3) {
                palavra3 = palavra;
            }
        }

        switch (contagem_palavras) {
                
                case 2:
                // Gera o código decimal para instruções com 2 palavras
                if(palavra2!="STOP")
                {
                   
                    endereco = encontra_endereco(palavra2,tabcont);
                    //verifica se a variavel consta na tabela de simbolos
                    if(endereco==-1)
                    {
                        cout<<"Erro, simbolo nao definido: "<<palavra2<<"\n";
                        exit(EXIT_FAILURE);
                    }
                    escrita<< instrucao_codigo[palavra1] << " " << endereco << "\n";
                }
                //L STOP
                else
                {
                    escrita<< instrucao_codigo[palavra2] << " "<<0<<"\n";
                }
                    
                break;
            
            case 3:

                // Gera o código decimal para instruções com 3 palavras
                if(palavra2!="space")//jmp
                {
                   
                    endereco=encontra_endereco(palavra3,tabcont);
                  
                    if(endereco==-1)
                    {
                        cout<<"Erro, simbolo nao definido: "<<palavra3<<"\n";
                        exit(EXIT_FAILURE);
                    }
                   escrita<< instrucao_codigo[palavra2] << " " << endereco << "\n";
                }
                break;
            
        }
        }
        leitura.close();
        escrita.close();
        
    }

//funcao para criar tabela de simbolos
void criar_tabela_de_simbolos(string arquivo)
{
    int i = 0;
    int x=0;
   
    
    string linha;
    ifstream leitura(arquivo);

    if (!leitura.is_open()) {
        cout << "\nNao foi possível abrir arquivo! Programa sera terminado!\n";
        exit(EXIT_FAILURE);
    }
    
    while (getline(leitura, linha)) {
        istringstream stream_linha(linha);
        string palavra;
        string palavra1, palavra2, palavra3;
        int contagem_palavras = 0;

        
        while (stream_linha >> palavra) {
            contagem_palavras++;
            if (contagem_palavras == 1) {
                palavra1 = palavra;
            } else if (contagem_palavras == 2) {
                palavra2 = palavra;
            } else if (contagem_palavras == 3) {
                palavra3 = palavra;
            }
        }

        switch (contagem_palavras) {
            
            case 1:
                x=comparar_palavra(palavra1);
                if(x!=12)
                {
                    cout << "\nErro no codigo, instrucao diferente de stop: " << palavra1 << "\n";
                    exit(EXIT_FAILURE);
                }
                else
                {
                    tem++;
                    endcont--;
                }
                break;

            case 2:
                //verifica se a instruçao existe
                x=comparar_palavra(palavra1);
                if (x==-1)//primeira palavra!=instruçao 
                {   
                    x=comparar_palavra(palavra2);
                    if (x==12)//adiciona o simbolo caso seja l stop 
                    {
                        if(encontra_endereco(palavra1,tabcont)!=-1)
                        {
                        cout << "\nErro no codigo,simbolo ja definido anteriormente : " << palavra1 << "\n";
                        exit(EXIT_FAILURE);
                        }
                        tab[tabcont].simbolo = palavra1;
                        tab[tabcont].endereco = endcont;
                        tabcont++;
                        
                    }
                    else
                    {
                    cout << "\nErro no codigo, instrucao nao conhecida: " << palavra1 << "\n";
                    exit(EXIT_FAILURE);
                    }
                }
                
                break;

            case 3:
                
                x=comparar_palavra(palavra1);
                if(x != -1)//3 palavras com erro de ordem de escrita
                {
                    cout<<"\nErro de sintaxe no codigo"<<" "<<palavra1<<" "<<palavra2<<" "<<palavra3<<"\n";
                    exit(EXIT_FAILURE);
                    
                }
                if(palavra2!="space")//jmp
                {
                    
                    //caso a palavra2 n for uma instrucao,ele so vai parar o codigo
                    x=comparar_palavra(palavra2);
                    if(x!=-1)
                    {
                        if(encontra_endereco(palavra1,tabcont)!=-1)
                         {
                            cout << "\nErro no codigo,simbolo ja definido anteriormente : " << palavra1 << "\n";
                            exit(EXIT_FAILURE);
                        }
                        tab[tabcont].simbolo = palavra1;
                        tab[tabcont].endereco = endcont;
                        tabcont++;
                        
                    }
                    else{
                    cout << "\nErro no codigo, instrucao nao conhecida: " << palavra2 << "\n";
                    exit(EXIT_FAILURE);
                    }
                        
                    
                }
                else    //space
                {

                if(encontra_endereco(palavra1,tabcont)!=-1)
                {
                    cout << "\nErro no codigo,simbolo ja definido anteriormente : " << palavra1 << "\n";
                    exit(EXIT_FAILURE);
                }
                space++;
                comparar_endereco();
                tab[tabcont].simbolo = palavra1;
                tab[tabcont].endereco = space;
                tabcont++;
                
                }
                break;

                default:
                cout <<"Erro, linha em branca ou com mais de 3 palavras"<<"\n";
                exit(EXIT_FAILURE);
        }
        endcont++;
    }
    leitura.close();
    
}



int main(int argc,char *argv[]) {
   
   int i=0;
   remove(argv[argc-1]);
  for(i=1;i<argc-1;i++)
  {
    criar_tabela_de_simbolos(argv[i]);
  }
  if(tem!=1)
  {
        cout <<"Erro no codigo,nao tem stop em nenhum dos arquivos lidos ou tem mais de um stop"<<"\n";
        exit(EXIT_FAILURE);
  }

  cout << "\nTabela de simbolos:\n";
 
    for (i = 0; i < tabcont; i++) {
        cout << "Endereco: " << tab[i].endereco << " -> Simbolo: " << tab[i].simbolo << "\n";
    }
    for(i=1;i<argc-1;i++)
  {
    gerar_codigo(argv[i],argv[argc-1]);
  }
  string ultima_linha = ler_ultima_linha(argv[argc-1]);
  if(ultima_linha!="12 0")
  {
    ofstream escrita(argv[argc - 1], ios::app);
    escrita << instrucao_codigo["STOP"] << " " << 0 << "\n";
    escrita.close();
  }
}
