#include <stdio.h>
#include <stdlib.h>
#define MAX 24

/**
 * 
 * @brief HavaLimani ucus yonetim sistemi
 * 
 * @param i 
 * @return int
 * 
 * 
 */

typedef struct ucak{
    int talep_edilen_inis_saati;
    int inis_saati;
    int gecikme_suresi;
    int ucak_id;
    int oncelik_id;
    int kalkis_saati;
}ucak;

void ertele(ucak * H [MAX], ucak * n);
void printKalkis();
void outputGuncelle();

ucak * ucakOlustur(int oncelik_id, int talep_edilen_inis_saati, int ucak_id)
{
    ucak * newNode = (ucak *)malloc(sizeof(ucak));

    newNode->gecikme_suresi = 0;
    newNode->ucak_id = ucak_id;
    newNode->talep_edilen_inis_saati = talep_edilen_inis_saati;
    newNode->oncelik_id = oncelik_id;
    newNode->kalkis_saati = talep_edilen_inis_saati + 1;
    newNode->inis_saati = talep_edilen_inis_saati;

    return newNode;
}

void freeArray(ucak ** arr)
{
    for(int i=0;i<MAX;i++)
    {
        free(arr[i]);
    }
}

void nullify(ucak ** H)
{
    for (int i = 0; i < MAX; i++)
    {
        H[i] = NULL;
    }
    
}
ucak * kalkis_pisti_kullanim_sirasi[MAX];

int main()
{
    FILE * fp = fopen("input.txt","r");

    if(fp == NULL)
    {
        printf("dosya bulunmamaktadir...\n");
        exit(EXIT_FAILURE);
    }


    fseek(fp,0,SEEK_SET);

    ucak * inis_pisti_kullanim_sirasi[MAX];
    
    nullify(inis_pisti_kullanim_sirasi);

    char buff[100];

    fgets(buff,100,fp);

    while(!feof(fp))
    {
        int ucak_id;
        int talep_edilen_inis_saati;
        int oncelik_id;
        fgets(buff,100,fp);
        sscanf(buff,"%d %d %d",&oncelik_id,&ucak_id,&talep_edilen_inis_saati);
        printf("oncelik_id: %d\n",oncelik_id);
        printf("ucak_id: %d\n",ucak_id);
        printf("talep_edilen_inis_saati: %d\n",talep_edilen_inis_saati);
        ucak * n = ucakOlustur(oncelik_id, talep_edilen_inis_saati, ucak_id);
        ertele(inis_pisti_kullanim_sirasi,n);
        printf("\n");
    }

    fclose(fp);
    
    return 0;
}


void ertele(ucak * H [MAX], ucak * n)
{
    ucak * onceki = H[n->talep_edilen_inis_saati + n->gecikme_suresi - 1];

    if(n->gecikme_suresi > 3)
    {
        printf("%d Sabiha Gokcen havalimanina yonlendirilmistir...\n",n->ucak_id);
        return;
    }

    if(onceki == NULL)
    {
        n->inis_saati = n->talep_edilen_inis_saati + n->gecikme_suresi;
        n->kalkis_saati = n->inis_saati + 1;
        printf("%d id'li ucagin inis izin talebi ONAYLANMISTIR...\n",n->ucak_id);
        H[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
        kalkis_pisti_kullanim_sirasi[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
        outputGuncelle();
        printKalkis();
        return;
    }else
    {
        if(n->oncelik_id > onceki->oncelik_id)
        {
            n->gecikme_suresi = n->gecikme_suresi + 1;
            printf("onceligi daha yuksek olan %d id'li ucak ayni saatte inis yapmak istedigi icin %d id'li ucagin inisi ertelenmistir...\n",onceki->ucak_id,n->ucak_id);
            ertele(H, n);
        }else if(n->oncelik_id < onceki->oncelik_id)
        {
            onceki->gecikme_suresi = onceki->gecikme_suresi + 1;
            n->inis_saati = n->talep_edilen_inis_saati + n->gecikme_suresi;
            n->kalkis_saati = n->inis_saati + 1;
            H[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
            kalkis_pisti_kullanim_sirasi[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
            outputGuncelle();
            printKalkis();
            printf("onceligi daha yuksek olan %d id'li ucak ayni saatte inis yapmak istedigi icin %d id'li ucagin inisi ertelenmistir...\n",n->ucak_id,onceki->ucak_id);
            printf("%d id'li ucagin inis izin talebi ONAYLANMISTIR...\n",n->ucak_id);
            ertele(H, onceki);
        }else{
            ucak * ertelenecek = n;
            if(n->ucak_id < onceki->ucak_id )
            {
                n->inis_saati = n->talep_edilen_inis_saati + n->gecikme_suresi;
                n->kalkis_saati = n->inis_saati + 1;
                ertelenecek = onceki;
                printf("%d id'li ucagin inis izin talebi ONAYLANMISTIR...\n",n->ucak_id);
                printf("%d id'li ucak inis talebi %d id'li ucaktan daha once talep ettigi icin %d nin inisi ertelenmistir...\n",n->ucak_id,ertelenecek->ucak_id,ertelenecek->ucak_id);
                H[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
                kalkis_pisti_kullanim_sirasi[n->talep_edilen_inis_saati + n->gecikme_suresi - 1] = n;
                outputGuncelle();
                printKalkis();
            }else{
                printf("%d id'li ucak inis talebi %d id'li ucaktan daha once talep ettigi icin %d nin inisi ertelenmistir...\n",onceki->ucak_id,ertelenecek->ucak_id,ertelenecek->ucak_id);
            }
            
            ertelenecek->gecikme_suresi = ertelenecek->gecikme_suresi + 1;
            
            ertele(H, ertelenecek);
            
        }
    }


}

void printKalkis()
{
    printf("\noncelik_id ucak_id talep_edilen_inis_saati inis_saati gecikme_suresi kalkis_saati\n");
    for (int i = 0; i < MAX; i++)
    {
        if(kalkis_pisti_kullanim_sirasi[i]!=NULL)
        {
            //fprintf(fw,"bos\n");
            printf("%10d %7d %23d %10d %13d %12d\n", kalkis_pisti_kullanim_sirasi[i]->oncelik_id, kalkis_pisti_kullanim_sirasi[i]->ucak_id, kalkis_pisti_kullanim_sirasi[i]->talep_edilen_inis_saati, kalkis_pisti_kullanim_sirasi[i]->inis_saati, kalkis_pisti_kullanim_sirasi[i]->gecikme_suresi, kalkis_pisti_kullanim_sirasi[i]->kalkis_saati);
        }
        
    }
    printf("\n");
}

void outputGuncelle()
{
    FILE * fw = fopen("output.txt", "w+");

    fprintf(fw,"oncelik_id ucak_id talep_edilen_inis_saati inis_saati gecikme_suresi kalkis_saati\n");
    for (int i = 0; i < MAX; i++)
    {
        if(kalkis_pisti_kullanim_sirasi[i]!=NULL)
        {
            //fprintf(fw,"bos\n");
            fprintf(fw,"%10d %7d %23d %10d %13d %12d\n", kalkis_pisti_kullanim_sirasi[i]->oncelik_id, kalkis_pisti_kullanim_sirasi[i]->ucak_id, kalkis_pisti_kullanim_sirasi[i]->talep_edilen_inis_saati, kalkis_pisti_kullanim_sirasi[i]->inis_saati, kalkis_pisti_kullanim_sirasi[i]->gecikme_suresi, kalkis_pisti_kullanim_sirasi[i]->kalkis_saati);
        }
        
    }

    fclose(fw);
}