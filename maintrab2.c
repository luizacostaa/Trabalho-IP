//-------------------------Parte principal---------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "termip.h"	

#define N 67
#define M 12

FILE *arquivo;
FILE *resultado;

//variaveis globais
int idade,preferencias[4],q_generos[12],limite_leitura=N;
int indice=0,matriz[3][4];
char nome[50],nome_pesq[50],P[601],aux[600],ordenado='0';


int xi=2,yi=5,centro,count,ncols, nrowls;		//variaveis para impressao



typedef struct 							//genero
{
	int cod_genero;
	char nome_genero[21];
}genero_filme;
genero_filme *g=NULL;


typedef struct							//filme
{
	int codigo,ano_lancamento,q_genero,genero[3],classind;
	char livro[50],autor_livro[30],outras_adap[15];
	char nome_filme[50],sinopse[600],elenco[300];
}filme;

filme *p=NULL;


typedef struct
{
	int ordem_filmes,di;
}lista;

lista *l=NULL;


//---------------------------tela 1---------------------------------------------------------------------------------------------
void imprime_tela1()			
{
	int tamanho_nome,i;
	nrowls = get_terminal_nrows();		//dimensoes do terminal
    ncols = get_terminal_ncols();
	
	
	
	//janelas
	draw_window(1, 1, ncols-1, nrowls-1, FG_cyan, BG_DEFAULT,"");
	centro= ncols/2;
	draw_window(5, ncols-27, 25, 25, FG_RGB(255,0,255), BG_DEFAULT,"Gêneros");
	draw_window(5, centro-42, 57, 25, FG_RGB(255,0,255), BG_DEFAULT,"");
	
	xi= centro- 41;
	yi= 7;
	//impressao pronta
	gotoxy(yi,xi);
	printf("Digite seu nome e aperte ENTER: ");
	yi+=10;
	gotoxy(yi,xi);
	printf("Digite sua idade: ");
	yi=6;
	xi=ncols-26;
	
	for(i=0;i<13;i++)
	{
		gotoxy(yi, xi);
		printf("%d-%s", i,g[i].nome_genero);
		yi++;
	}
	
	xi=centro- 41;
	yi=25;
	gotoxy(yi,xi);
	printf("Coloque sua preferência e aperte ENTER:\n");		
	
	//entradas
	xi= centro-41;
	yi= 8;
	gotoxy(yi,xi);
	scanf("%[^\n]%*c", nome);	
	
	yi+=10;
	gotoxy(yi,xi);
	scanf("%d%*c", &idade);			
	
									//preferências
	yi=27;
	gotoxy(yi,xi);
	for(i=0;i<3;i++)
	{
		gotoxy(yi,xi);
		scanf("%d%*c", &preferencias[i]);
		
		xi+=25;
	}
}

//---------------------------------tela 2-----------------------------------------------------------------------------------
void imprime_tela2()
{
	int i,j,tamanho;
	
	nrowls = get_terminal_nrows();
    ncols = get_terminal_ncols();
    centro= ncols/2;
	
	draw_window(1, 1, ncols-1, nrowls-1, FG_cyan, BG_DEFAULT,"");
	draw_window(5, centro-42, 57, 25, FG_RGB(255,0,255), BG_DEFAULT,"");
	
	xi=2;
	yi=1;
	gotoxy(yi,xi);
	printf(BG_DEFAULT FG_cyan"%s", nome);
	
	xi=centro-41;
	yi+=5;
	gotoxy(yi,xi);
	printf(BG_DEFAULT FG_white "Preferencias:\n");
	
	xi= centro-36;
	for(i=0;i<3;i++)
	{
		for(j=0;j<13;j++)
		{
			if(preferencias[i]==g[j].cod_genero)
			{
				gotoxy(yi,xi);
				printf("\t\t*%s",g[j].nome_genero);
				yi++;
			}
		}
	}
	
	centro= ncols/2;
	xi= centro - 50;
	yi+=16;
	gotoxy(yi,xi);
	printf("\t\t\tDigite o nome do filme:\n");
	
	
	xi= centro-41;
	yi++;
	gotoxy(yi,xi);
	scanf("%[^\n]%*c",nome_pesq);
	
	tamanho= strlen(nome_pesq);
	
	for(i=0;i<tamanho;i++)				//converte para maiusculas
	{
		nome_pesq[i]= toupper(nome_pesq[i]);
	}
}

//------------------------------tela 3------------------------------------------------------------------------------------------
void compara_filmes(filme *p)
{
	int j,aux,d,k,tamanho;
	
	
	tamanho= strlen(p->nome_filme);
	
	for(j=0;j<tamanho;j++)		//transformando em maiuscula
	{
		p->nome_filme[j]= toupper(p->nome_filme[j]);
	}
	
	
	d=strcmp(nome_pesq,p->nome_filme);
	if(d<0) d*=-1;
	
	for(j=0;j<4;j++)
	{
		if(d<= l[j].di)
		{
			for(k=3;k>j;k--)
			{
				l[k].ordem_filmes=l[k-1].ordem_filmes;
				l[k].di=l[k-1].di;
			}
			l[j].ordem_filmes=p->codigo;
			l[j].di=d;
			break;
		}
	}
	
}

int tem_filme(filme *p, lista *l)  //confere se o filme ta na lista
{
	int i,j,tem=0;
	
	for(i=0;i<M;i++)
	{
		if(p->codigo == l[i].ordem_filmes)
		{
			tem=1;
			break;		
		}
	}
	
return tem;
}

int compara_generos(filme *p, lista *l,int *q_generos)
{
	int i,j,limite;
	
	if(ordenado=='0') compara_filmes(p);
	
	
	else
	{
	//indica o genero e delimita a quantidade de filmes para add 
	if(ordenado=='1')
	{
		i=0;
		limite=5;
	}
	
	else if(ordenado=='2')
	{
		i=1;
		limite=8;
	}
	
	else
	{
		i=2;
		limite=11;
	}
	
	//coloca o filme na lista
	if(tem_filme(p,l) ==0)		//se o filme nao ta na lista
	{	
		for(j=0;j<(q_generos[0]);j++)	//se o genero está nas preferencias
		{
			if(preferencias[i] == p->genero[j])
			{
				
				if(indice>limite-1) 	return 1;
				
				indice++;
				l[indice].ordem_filmes= p->codigo;
				break;
			}
		}
	  }
	}
	 	
return 0;
}

void le_arquivo(filme *p)
{
	int i=0,j=0,k=0,pode=0;
	
	for(k=0;k<N;k++)
	{
		i=0;
		fscanf(arquivo, "%d%*c", &p[i].codigo);
		
		fscanf(arquivo, "%[^\n]%*c", p[i].nome_filme);
		
		fscanf(arquivo, "%[^\n]%*c", p[i].sinopse);
				
		fscanf(arquivo, "%d%*c", &p[i].ano_lancamento);
				
		fscanf(arquivo, "%d%*c", &p[i].classind);
				
		fscanf(arquivo, "%d%*c", &q_generos[0]);
		for(j=0;j<(q_generos[0]);j++)
		{
			fscanf(arquivo, "%d", &p[i].genero[j]);
		}
				
		fscanf(arquivo, "%*c%[^\n]%*c", p[i].elenco);
		
		fscanf(arquivo, "%[^\n]%*c",  p[i].livro);
		
		fscanf(arquivo, "%[^\n]%*c", p[i].autor_livro);
		
				
		fscanf(arquivo, "%[^\n]%*c", p[i].outras_adap);
		
		if(ordenado<'4')
		{
			if(compara_generos(p,l,q_generos)==1) break;
		}
		
	}
}

void adiciona_lista()
{
	ordenado='0';
	le_arquivo(p);
	indice=2;
	
	
	ordenado='1';
	rewind(arquivo);
	le_arquivo(p);
	
	
	ordenado='2';
	rewind(arquivo);
	le_arquivo(p);
	
	
	ordenado='3';
	rewind(arquivo);
	le_arquivo(p);
	
	ordenado='4';

}


void cria_resultado()
{
	int i,j,k,d;
	
	for(k=0;k<12;k++)
	{
		d=l[k].ordem_filmes;
		//meu fseek kkkk
		for(j=0;j<((d*10)-9);j++)		//conta da linha desejada
		{
			fscanf(arquivo, "%[^\n]%*c", aux);
			
		}
		sprintf(P,"%s\n", aux);
		fputs(P, resultado);
		
		for(i=0;i<9;i++)
		{
			fscanf(arquivo, "%[^\n]%*c", aux);
			sprintf(P,"%s\n", aux);
			fputs(P, resultado);
		}
		rewind(arquivo);
		memset(P,'\0',600);
	}

}



void adiciona_struct()
{
		int i,j;
		
		for(i=0;i<12;i++)
		{
			fscanf(resultado, "%d%*c", &p[i].codigo);
		
			fscanf(resultado, "%[^\n]%*c", p[i].nome_filme);
			
			fscanf(resultado, "%[^\n]%*c", p[i].sinopse);
				
			fscanf(resultado, "%d%*c", &p[i].ano_lancamento);

			fscanf(resultado, "%d%*c", &p[i].classind);
			
			fscanf(resultado, "%d%*c", &(q_generos[i]));
			for(j=0;j<(q_generos[i]);j++)
			{
				fscanf(resultado, "%d", &p[i].genero[j]);
			}
			
			
			fscanf(resultado, "%*c%[^\n]%*c", p[i].elenco);
			
			fscanf(resultado, "%[^\n]%*c",  p[i].livro);

			fscanf(resultado, "%[^\n]%*c", p[i].autor_livro);
				
			fscanf(resultado, "%[^\n]%*c", p[i].outras_adap);
	
		
		}
}


void imprime_struct()
{
	int i,j;
	
	for(i=0;i<12;i++)
	{
		printf("cod:%d\n", p[i].codigo);
		printf("nome:%s\n", p[i].nome_filme);
		printf("sinopse:%s\n", p[i].sinopse);
		printf("ano:%d\n", p[i].ano_lancamento);
		printf("class:%d\n", p[i].classind);
		printf("genero:");
		for(j=0;j<(q_generos[i]);j++)
		{
			if(p[i].genero[j]!=0)printf("%d,", p[i].genero[j]);
		}
		printf("\n");
		printf("elenco:%s\n", p[i].elenco);
		printf("livro:%s\n", p[i].livro);
		printf("autor:%s\n", p[i].autor_livro);
		printf("obras:%s\n\n", p[i].outras_adap);
	}
}


void cria_matriz()
{
	int i,j,k=0;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		{
			matriz[i][j]=k;
			k++;
		}
	}
}


void imprime_matriz()
{
	int i,j,xi=2,yi=5,centro,aux,count,d;

	cria_matriz();
	eval(CURSOR_INVISIBLE);
	
	
	nrowls = get_terminal_nrows();
    ncols = get_terminal_ncols();
   
	draw_window(1, 1, ncols, nrowls, FG_cyan, BG_DEFAULT,"");	//janela maior
	gotoxy(1,2);
	printf(BG_DEFAULT FG_cyan"%s", nome);
	
	
	
	centro= (ncols/2) - (strlen(nome_pesq)/2);
    
	
	count=0;
	aux=0;
	
		eval( BG_DEFAULT FG_DEFAULT CURSOR_INVISIBLE );	
		gotoxy(3,centro);
	printf(ATTR_ITALIC_ON ATTR_UNDERLINE_ON "%s\n" ATTR_ITALIC_OFF ATTR_UNDERLINE_OFF, nome_pesq);  //nome filme
	draw_window(2, ncols-15, 14, 2, FG_RGB(255,0,255) ATTR_BOLD, BG_DEFAULT, "");	//filme
		gotoxy(1,1);
		for(i=0;i<3;i++)
		{
			for(j=0;j<4;j++)
			{
				d= matriz[i][j];
				draw_window(yi, xi, 34, 10, FG_RGB(255,110,255) ATTR_BOLD, BG_DEFAULT, "");
			
				gotoxy(yi+1,xi+16);
				printf(BG_DEFAULT FG_RGB(255,255,0) "(%d)",d);
				gotoxy(yi+3,xi+1);
				printf(BG_DEFAULT FG_RGB(255,255,150)"%s", p[d].nome_filme);
				gotoxy(yi+5,xi+1);
				printf(BG_DEFAULT FG_RGB(255,255,150)"Ano lancamento:%d", p[d].ano_lancamento);
				gotoxy(yi+7,xi+1);
				printf(BG_DEFAULT FG_RGB(255,255,150)"Classificação:%d", p[d].classind);
				gotoxy(yi+9,xi+1);
				//printf(BG_DEFAULT FG_RGB(255,255,150)"genero:");
				xi+=35;
				count++;
			}
			xi=2;
			yi+=11;
		}
		gotoxy(2,ncols-14);
		printf(FG_RGB(255,0,255) "Filme:");
		gotoxy(3,ncols-14);
		printf("--->");
		
		eval(BG_DEFAULT FG_RGB(255,255,255));
		gotoxy(3,ncols-10);


}

void imprime_tela_final(int n)
{
	int i,j;
	char nome_f[50];
	
	//filme nao permitido
	if(idade< (p[n].classind))
	{
		//janela com nome usu
	draw_window(1, 1, ncols, nrowls, FG_cyan, BG_DEFAULT,"");
	
		draw_window(7, 2, ncols-3, nrowls-9, FG_RGB(255,255,255), BG_red,"");
		centro= (ncols/2) - (strlen(nome_f)/2);
		gotoxy(9,centro);
		printf(BG_RED FG_RGB(0,0,0) "%s", p[n].nome_filme);
		
		gotoxy(11,3);
		printf(BG_RED FG_RGB(0,0,0) "Esse filme não é permitido para sua idade!!!!!");
		
		return;
		
	}
	
	strcpy(nome_f, p[n].nome_filme);
	
	//janela com nome usu
	draw_window(1, 1, ncols, nrowls, FG_cyan, BG_DEFAULT,"");	//janela maior
	gotoxy(1,2);
	printf(BG_DEFAULT FG_cyan"%s", nome);
	
	//informacoes do filme
	draw_window(7, 2, ncols-3, nrowls-9, FG_RGB(255,110,255), BG_DEFAULT,"");
	
	centro= (ncols/2) - (strlen(nome_f)/2);
	gotoxy(9,centro);
	printf(ATTR_ITALIC_ON ATTR_UNDERLINE_ON "%s\n" ATTR_ITALIC_OFF ATTR_UNDERLINE_OFF, nome_f);
	
	//sinopse
	xi=3;
	yi=12;
	gotoxy(yi,xi);
	printf("Sinopse:");
	xi+=8;
	for(i=0;p[n].sinopse[i]!='\0';i++)
	{
		//pula linha
		if(xi==ncols-2)
		{
			xi=3;	
			yi++;
		}
		
		else
		{
			gotoxy(yi,xi); 
			printf("%c", p[n].sinopse[i]);
			xi++;
		}
	}
	
	//generos
	xi=3;
	yi+=3;
	gotoxy(yi,xi);
	printf("Genero");
	if(q_generos[n]>1)	printf("s");
	printf(":");
	for(i=0;i<(q_generos[n]);i++)
	{
		for(j=0;j<13;j++)
		{
			if((p[n].genero[i]) == g[j].cod_genero) printf("%s  ", (g[j-1].nome_genero));
		}
	}
	
	yi+=3;
	xi=3;
	gotoxy(yi,xi);
	printf("Lançamento:%d", p[n].ano_lancamento);
	
	yi+=3;
	gotoxy(yi,xi);
	printf("Classificação indicativa:%d", p[n].classind);
	
	yi+=3;
	gotoxy(yi,xi);
	printf("Elenco:");
	xi+=7;
	for(i=0;p[n].elenco[i]!='\0';i++)
	{
		//pula linha
		if(xi==ncols-2)
		{
			xi=3;	
			yi++;
		}
		
		else
		{
			gotoxy(yi,xi); 
			printf("%c", p[n].elenco[i]);
			xi++;
		}
	}
	
	//livro
	if(strcmp(p[n].livro,"0")!=0)
	{
		xi=3;
		yi+=3;
		gotoxy(yi,xi);
		printf("Livro:%s", p[n].livro);
	
		xi=3;
		yi+=1;
		gotoxy(yi,xi);
		printf("%s", p[n].autor_livro);
	}
	
	else
	{
		xi=3;
		yi+=3;
		gotoxy(yi,xi);
		printf("O filme não tem origem de um livro!");
	
	}
	
	//adap
	yi=nrowls-3;
	gotoxy(yi,xi);
	if(strcmp(p[n].outras_adap,"0")!=0)
	{
		printf("Esse filme tem adaptacoes de:%s ",p[n].outras_adap);
	}
	
	else printf("Esse filme nao teve outras adaptacoes!");
	
	
	
	


}

//tela 3 ****************************************************
void imprime_tela3()
{
	int n;
	
	xi=yi=0;
	
	//fazendo a lista a ser exibida
	adiciona_lista();
	rewind(arquivo);
	
	cria_resultado();
	
	indice=0;
	clear();
	rewind(resultado);
	adiciona_struct();
	imprime_matriz();
	scanf("%d",&n);
	clear();

	return;

}

//---------------tela4--------------
void imprime_tela4()
{
	imprime_tela_final(n);
}


int main()
{
	int a;
	char c;
	
	arquivo= fopen("banco_de_dados_filmes.txt", "r");
	
	resultado= fopen("resultado_pesquisa.txt", "w+");


	g= (genero_filme*) calloc(13,sizeof(genero_filme));
	
	p= (filme*) calloc(12,sizeof(filme));
	
	l= (lista*) calloc(M,sizeof(lista));
	
	
	for(a=0;a<12;a++)
	{
		l[a].di=10000;
	}

g[0].cod_genero=0;
strcpy(g[0].nome_genero,"Ação");
g[1].cod_genero=1;
strcpy(g[1].nome_genero,"Animação");
g[2].cod_genero=2;
strcpy(g[2].nome_genero,"Aventura");
g[3].cod_genero=3;
strcpy(g[3].nome_genero,"Comédia");
g[4].cod_genero=4;
strcpy(g[4].nome_genero,"Fantasia");
g[5].cod_genero=5;
strcpy(g[5].nome_genero,"Faroeste");
g[6].cod_genero=6;
strcpy(g[6].nome_genero,"Ficção científica");
g[7].cod_genero=7;
strcpy(g[7].nome_genero,"Guerra");
g[8].cod_genero=8;
strcpy(g[8].nome_genero,"Musical");
g[9].cod_genero=9;
strcpy(g[9].nome_genero,"Romance");
g[10].cod_genero=10;
strcpy(g[10].nome_genero,"Suspense");
g[11].cod_genero=11;
strcpy(g[11].nome_genero,"Terror");
g[12].cod_genero=12;
strcpy(g[12].nome_genero,"Tragédia/drama");

	
	eval(CURSOR_INVISIBLE);					//deixa o cursor invisivel
	clear();
//---------------------------tela 1-----------
	imprime_tela1();
	
	
	
	clear();
//----------------------------tela 2-----------
	imprime_tela2();
	

	clear();
//----------------------------tela 3-------	
	imprime_tela3();
	
	clear();
//---------------------------tela4---------
	imprime_tela4();
	
	
	scanf("%*c%c", &c);
	



//liberando memória
	free(g);
	free(p);
	free(l);
	
//fechando arquivo
	fclose(arquivo);
	fclose(resultado);
	
return 0;
}
