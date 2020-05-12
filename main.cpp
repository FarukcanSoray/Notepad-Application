#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define KEY_ESCAPE  0x1B
#define KEY_ENTER   0x0D
#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77
#define KEY_F1      0x3B
#define KEY_F2		0x3C

int x = 0, y = 0;           ///Imlecin koordinatlari
int MAXY = 0 ,tmpy = 0;     ///Satir sayisi
int i;                      ///dongulerdeki sayac
int elemanSayisi2 = 0;      ///o an yazilan satirdaki karakter sayisi
int elemanSayisi  = 0;      ///bir ust ya da bir alt satirdaki eleman sayisi icin kullanilan degisken
int flag1 = 0,flag = 0;     ///Karakter secimi yaparken kullanacagimiz integer
int err = 0;                ///Switch hata dondurdu mu?


typedef struct karakter     ///karakter structimiz bagli liste icin
{
   char c;                  ///Karekterimiz
   struct karakter *next;   ///Bir sonraki karakter
   struct karakter *prev;   ///Bir onceki karakter
}Karakter;

Karakter *first  = NULL;    /// bagli listenin basi
Karakter *ptr    = NULL;    /// imlecin konumu
Karakter *ptr2   = NULL;    /// gecici durumlar icin gecici imlec pointeri
Karakter *tmp    = NULL;    /// gecici durumlar icin gecici pointer
Karakter *tmp2   = NULL;    /// gecici durumlar icin gecici pointer
Karakter *bas    = NULL;    /// Karakter secimi sirasinda kullanilan pointer
Karakter *son    = NULL;    /// Karakter secimi sirasinda kullanilan pointer
Karakter *Tfirst = NULL;    /// CTRL islemleri icin
Karakter *Tend   = NULL;    /// CTRL islemleri icin

void karakterAl(char );     /// karakter alma fonksiyonumuz
void gotoxy(int, int);      /// imlec hareketi
void up();                  /// imleci yukari tasimak
void down();                /// imleci asagi tasimk
void right();               /// imleci saga tasimak
void left();                /// imleci sola tasimak
void save();                /// kayit
void open();                /// txt dosyasi acmak
void sil();                 /// silme fonkiyonu
void yazdir();              /// araya karakter ekendiginde ya da silme isleminden sonra ekrani silip tekrar yazdirmak
void sec();                 /// Shift + Ok tuslari icin karakter secme fonksiyonu
void GeciciBagliListe();    /// Kontrol tuslari icin secili metni yeni listeye atma
void Paste();               /// CTRL+V
void Copy();                /// CTRL+C
void Cut();                 /// CTRL+X
void SelectAll();           /// CTRL+A
void menu();                /// F1 Menu tusu

int main()
{
    system("color F0");     /// arkaplani beyaz yapiyoruz
    while(1)
    {
        ///Shift ve ok tusu ile karakter secme
        /**************************************************************/
        if(GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_LEFT))
        {
            tmpy = y;
            sec();
        }
        if(GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_RIGHT))
        {
            tmpy = y;
            sec();
        }
        if(GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_UP))
        {
            tmpy = y+1;
            sec();
        }
        if(GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_DOWN))
        {
            tmpy = y-1;
            sec();
        }
        /**************************************************************/
        if(GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(0x56) && Tfirst != NULL) ///CTRL + V
        {
            Paste();
        }
        if(GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(0x43) && bas != NULL)///CTRL + C
        {
            Copy();
        }
        if(GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(0x58))///CTRL + X
        {
            Cut();

        }
        if(GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(0x41))///CTRL + A
        {
            SelectAll();
        }
        int c = getch();            ///karakter okuma
        if (c == 0 || c == 0xE0)    ///ok tuslarini okuma
        {
            switch (getch())
            {
            case KEY_UP:
                up();
                break;
            case KEY_DOWN:
                down();
                break;
            case KEY_RIGHT:
                right();
                break;
            case KEY_LEFT:
                left();
                break;
            case KEY_F1:
                menu();
                break;
            }
        }
        else if (c == KEY_ENTER) /// icin ayri kosul tanimlandi
        {
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
            karakterAl('\n');
            y++;
            x=0;
            MAXY++;
            elemanSayisi2=0;
        }
        else if(c == 127 || c == 8) ///backspace tusunu okuma
        {
            if(son != NULL)
            {
                if(son->next != NULL)
                son = son->next;
            }

            if(bas != son)          ///Secili karakterleri toplu silmek icin
            {
                while(ptr != son)
                {
                    right();
                }
                while(ptr != bas)
                {
                    sil();
                }
            }
            else
            {
                sil();
            }
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
        }
        else if (c > 31 && c < 127) ///karakter alma
        {
            karakterAl(c);
            elemanSayisi2++;
            x++;
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
        }

        if(elemanSayisi2 == 118) ///satir sonu
        {
            karakterAl('\n');
            y++;
            x=0;
            MAXY++;
            elemanSayisi2=0;
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
        }
        if(first == NULL) ///koorinatlar sifirlaniyor
        {
            x=0;
            y=0;
        }
        gotoxy(x,y);     ///yapilan her degisiklikte imlec hareket ettirilir
    }
    return 0;
}
void karakterAl(char ch)
{
    Karakter *yeni = (Karakter *)malloc(sizeof(Karakter));
    yeni->c = ch;
    if(x == 0 && y == 0)
    {
        if(first == NULL) ///Ilk karakter
        {
            first = yeni;
            ptr   = yeni;
            ptr->next = NULL;
            ptr->prev = NULL;
            first->next = NULL;
            first->prev = NULL;
        }
        else ///Ilk karakterin onune ilk karakter
        {
            ptr = yeni;
            ptr->next = first;
            first->prev = ptr;
            ptr->prev = NULL;
            first = ptr;
        }
    }
    else if(first == ptr)
    {
        if(ptr->next == NULL) ///Ikinci karakteri eklemek
        {
            ptr = yeni;
            first->next = ptr;
            ptr->prev = first;
            ptr->next = NULL;
        }
        else ///Ikinci karakterin onune karakter eklemek
        {
            yeni->prev = ptr;
            yeni->next = ptr->next;
            ptr->next->prev = yeni;
            ptr->next = yeni;
            ptr = ptr->next;
        }
    }
    else
    {
        if(ptr->next == NULL) ///Ikiden fazla karakter
        {
            yeni->next = NULL;
            yeni->prev = ptr;
            ptr->next = yeni;
            ptr = yeni;
        }
        else  ///Araya karakter eklemek icin
        {
            yeni->prev = ptr;
            yeni->next = ptr->next;
            ptr->next->prev = yeni;
            ptr->next = yeni;
            ptr = ptr->next;
        }
    }
    if(ptr->next != NULL) ///araya karakter ekledikten sonra tekrardan yazdirmak
    {
        yazdir();
    }
    else ///En sona karakter ekleme yapildigi zaman yazdirmak
    {
        putchar(ptr->c);
    }
}
void sil()
{
    if(x != 0 || y != 0)
    {
        Karakter *silinen;
        silinen = ptr;
        if(ptr->next == NULL) ///imlec metnin sonundaysa
        {

            if(silinen->c == '\n' && ptr != first)
            {
                if(silinen->prev->c == '\n')
                {
                    x=0;
                    y--;
                    ptr = ptr->prev;
                }
                else
                {
                    left();
                    x++;
                    ptr = ptr->next;
                }
            }
            if(ptr != first)
            {
                ptr = ptr->prev;
                ptr->next = NULL;
                x--;
            }
            else /// Metinde tek karakter varsa
            {
                y = 0;
                x = 0;
                ptr = NULL;
                first = NULL;
            }

        }
        else ///imlec ortalardayken karakter siliniyorsa
        {

            if(ptr != first)  ///birinci karakter disindakiler siliniyorsa
            {
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
                ptr = ptr->prev;
                if(silinen->c == '\n')
                {
                    x = 0;
                    if(silinen->prev->c == '\n')
                    {
                        y--;
                    }
                    else
                    {
                        Karakter *ptr3 = ptr->prev;
                        while(ptr3->c != '\n' && ptr3!=first)
                        {
                            x++;
                            ptr3=ptr3->prev;
                        }
                        x++;
                        if(ptr3== first || ptr3->prev == first)
                        {
                            x++;
                        }
                        y--;
                    }
                }
                else
                {
                    x--;
                }

            }
            else ///birinci karakteri silmek icin
            {
                ptr = ptr->next;
                ptr->prev = NULL;
                first = ptr;
                if(silinen-> c == '\n')
                {
                    x = 0;
                    y = 0;
                }
                else
                {
                    x--;
                }

            }
        }
        free(silinen);
        if(flag == 0)
        {
            yazdir();
        }

    }

}
void yazdir() ///araya karakter eklendigi ya da karakter silindigi zaman ekrani temizlemek
{
    Karakter *temp = NULL;
    system("CLS");
    for(temp = first; temp != NULL; temp = temp->next)
    {
        putchar(temp->c);

    }
}
void gotoxy(int x1, int y1) ///imlec hareketi icin kullanilan fonksiyon
{
    COORD coord;
    coord.X = x1;
    coord.Y = y1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void menu()
{
    system("CLS");
    int s;
    printf("||======================================================||\n");
    printf("||                                                      ||\n");
    printf("||                     ==  MENU  ==                     ||\n");
    printf("||                                                      ||\n");
    printf("||  1. Kaydet                                           ||\n");
    printf("||  2. Dosya Ac                                         ||\n");
    printf("||  3. Yazmaya Geri Don                                 ||\n");
    printf("||  0. Cikis                                            ||\n");
    printf("||                                                      ||\n");
    printf("||======================================================||\n");
    printf("||                                                      ||\n");
    printf("||  Lutfen Seciminizi Giriniz:                          ||\n");
    printf("||                                                      ||\n");
    printf("||======================================================||\n");
    printf("\n\n\nProgram basit bir not defteri ornegidir. \n");
    printf("Programi kullanarak yazi yazabilir ve yazdiklarinizi kaydedebilirsiniz.\nBunun icin 1e basip dosyanizin adini ya da yolunu girmelisiniz.\n\n");
    printf("Onceden calistiginiz dosyayi tekrar acabilirsiniz.\nBunun icin 3 girdikten sonra dosya yolunu girmelisiniz.\n\n");
    printf("Programda Shift + ok tuslariyla secim yaptiginiz karakterleri kopyalayabilir ya da kesebilirsiniz.\nSonra isterseniz yere yapistirabilirsiniz.\n\n");
    printf("Bunun icin CTRL+C, CTRL+V, CTRL+X kullanabilirsiniz.\n");

    if(err)
    {
         printf("Yanlis bir deger girdiniz. Lutfen 1, 2, 4, veya 0 giriniz.\n");
    }
    gotoxy(31,11);
    scanf("%d",&s);
    switch(s)
    {
        case 1:
            save();
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
            break;
        case 2:
            open();
            bas   = NULL;
            son   = NULL;
            flag1 = 0;
            break;
        case 3:
            yazdir();
            main();
        case 0:
            system("ClS");
            printf("Cikmak istediginizden emin misiniz? (1/0): ");
            scanf("%d",&i);
            if(i)
            exit(EXIT_SUCCESS);
            else
                menu();
            break;
        default:
            err = 1;
            menu();
            err = 0;
            break;
    }
}
void save()
{
    system("CLS");
    printf("Belgenizi kaydetmek icin dosya adi veya yolu giriniz.\n");
    char dosyaAdi [250];
    scanf("%s",dosyaAdi);
    FILE *dosya = fopen(dosyaAdi,"w");
    Karakter *ptr1;
    for(ptr1 = first; ptr1 != NULL; ptr1 = ptr1->next) ///Dongu icerisinde bagli listedeki
    {                                                  ///karakterler yazdirlir
        putc(ptr1->c,dosya);
    }
    yazdir();
    fclose(dosya);
}
void open()
{
	system("CLS");
    char dosyaAdi [250];
    printf("Acmak istediginiz belgeyi giriniz:\n");
    scanf("%s",dosyaAdi);
    system("CLS");
    while(first != NULL)
    {
        sil();
    }
    x = 0; y = 0; MAXY = 0; elemanSayisi2 = 0;
    FILE *dosya = fopen(dosyaAdi,"r");
    ptr = NULL;
    if(dosya)
    {
        char c;
        while((c=getc(dosya)) != EOF)
        {

            if(c == '\n')
            {
                karakterAl('\n');
                x=0;
                elemanSayisi2=0;
                y++;
                MAXY++;

            }
            else
            {
                karakterAl(c);
                elemanSayisi2++;
                x++;
            }
            if(elemanSayisi2 == 118)
            {
                karakterAl('\n');
                x=0;
                elemanSayisi2=0;
                y++;
                MAXY++;
            }
        }
    }
    sec();
    fclose(dosya);
}
void up()
{
    elemanSayisi=0;
    if(y != 0)
    {
        tmp = first;
        for(i=0;i<y-1;i++){
            while(tmp->c!='\n'){
                tmp=tmp->next;
            }
            tmp=tmp->next;
        }
        tmp2=tmp;
        while(tmp2->c!='\n'){
            tmp2=tmp2->next;
            elemanSayisi++;
        }
        if(x<=elemanSayisi){
            for(i=0;i<x;i++){
                tmp=tmp->next;
            }
        }
        else if(x>elemanSayisi){
            x=0;
            while(tmp->c!='\n'){
                tmp=tmp->next;
                x++;
            }
        }
        y--;
        ptr=tmp->prev;
    }

}
void down()
{
    int tmp=x, tmp2=0;
    if(y!=MAXY) ///fonksiyon son satirdayken calismasin diye
    {
        x++;
        while(tmp!=x) ///karakterin altinda karakter yoksa imleci alt satirin sonuna atmak
        {
            right();
            if(ptr->c=='\n')
            {
                tmp2++;
            }
            if(tmp2==2 || ptr->next == NULL)
            {
                left();
                break; ///eger karakterin altinda karakter yoksa
            }
        }
    }
}
void left()
{
    elemanSayisi = 0;
    if(x != 0 || y != 0)
    {
        if(x != 0)
        {
            ptr = ptr->prev;
            x--;
        }
        else
        {
            if(first->c == '\n' && y == 1)
            {
                y = 0;
                x = 0;
                ptr = first;
            }
            else
            {
                y--;
                Karakter *ptr3 = ptr->prev;
                while(ptr3->c != '\n' && ptr3!=first)
                {
                    x++;
                    ptr3=ptr3->prev;
                }
                ptr=ptr->prev;
                if(ptr->c == '\n' && x!=0)
                    x--;
                if(y == 0)
                    x++;
            }
        }
    }
}
void right()
{
    if(ptr == NULL)
    {
        ptr = first;
        x++;
    }
    else if(ptr->next != NULL)
    {
        if(ptr->next->c!='\n') ///satir üzerinde saga hareket
        {
            x++;
            ptr = ptr->next;
        }
        else if(ptr->next->c == '\n') ///satirin sonuna gelince alt satira atlamak
        {
            x=0;
            y++;
            ptr=ptr->next;
        }

    }

}
void sec()
{
    if(first != NULL && bas != first)
    {
        if(flag1 == 0)
        {
            bas = ptr;
            son = bas;
            flag1 = 1;
        }
        else if(flag1 == 1)
        {
            if(ptr == bas->prev || y < tmpy)
            {
                bas = ptr;
                flag1 = -1;
            }
            else if(ptr == bas->next || y >  tmpy)
            {
                son = ptr;
                flag1 = 2;
            }
        }
        else if(flag1 == -1)
        {
            if(ptr == bas->prev || y < tmpy)
            {
                bas = ptr;
                flag1 = -1;
            }
            else if(ptr == bas->next || y > tmpy)
            {
                bas = ptr;
                if(y>tmpy)
                {
                    tmp = son;
                    while(tmp != NULL)
                    {
                        if(tmp == bas)
                        {
                            tmp = son;
                            son = bas;
                            bas = tmp;
                            break;
                        }
                        if(tmp->next == NULL)
                        {
                            son = bas;
                            tmp = son;
                        }
                        tmp = tmp -> next;
                    }
                }
                if(bas == son)
                    flag1 = 1;
                else
                    flag1 = -1;
            }
        }
        else if(flag1 == 2)
        {
            if(ptr == son->next || y > tmpy)
            {
                son = ptr;
                flag1 = 2;
            }
            else if(ptr == son->prev || y < tmpy)
            {
                son = ptr;
                if(y < tmpy)
                {
                    tmp = bas;
                    while(tmp != NULL)
                    {
                        if(tmp == son)
                        {
                            tmp = bas;
                            bas = son;
                            son = tmp;
                            break;
                        }
                        tmp = tmp -> prev;
                        if(tmp == NULL)
                        {
                            son = bas;
                            bas = first;
                        }
                    }
                }
                if(bas == son)
                    flag1 = 1;
                else
                    flag1 = 2;
            }
        }
    }
}
void GeciciBagliListe()
{
    Karakter *yeni = (Karakter *) malloc(sizeof(Karakter));
    yeni->c = tmp->c;
    if(Tfirst == NULL)
    {
        yeni->prev = NULL;
        yeni->next = NULL;
        Tfirst = yeni;
        Tend   = yeni;
    }
    else if(Tfirst == Tend)
    {
        Tend   = yeni;
        Tend->prev = Tfirst;
        Tfirst->next = Tend;
        Tend->next = NULL;
    }
    else
    {
        Tend->next = yeni;
        yeni->prev = Tend;
        yeni->next = NULL;
        Tend=yeni;
    }
}
void Paste()
{
    for(tmp = Tfirst ; tmp != NULL ; tmp = tmp->next)
    {
        if(tmp->c == '\n')
        {
            karakterAl('\n');
            y++;
            x=0;
            MAXY++;
            elemanSayisi2=0;
        }
        else
        {
            karakterAl(tmp->c);
            elemanSayisi2++;
            x++;
        }
        if(elemanSayisi2 == 118)
        {
            karakterAl('\n');
            y++;
            x=0;
            MAXY++;
            elemanSayisi2=0;
        }
    }
    son = NULL;
    bas = NULL;
    flag1 = 0;
    gotoxy(x,y);
}
void Copy()
{
    if(son->next != NULL)
    {
        son = son->next;
    }
    if(bas != son)
    {
        bas = bas->next;
    }
    if(Tfirst != NULL || Tend != NULL)
    {
        Tfirst = NULL;
        Tend   = NULL;
    }
    for(tmp = bas ; tmp != son->next ; tmp = tmp->next)
    {
        GeciciBagliListe();
    }
    son = NULL;
    bas = NULL;
    flag1 = 0;
}
void Cut()
{
    if(bas != son)
    {
        flag = 1;
        if(son->next != NULL)
        {
            son = son->next;
        }
        bas = bas->next;
        if(Tfirst != NULL || Tend != NULL)
        {
            Tfirst = NULL;
            Tend   = NULL;
        }
        for(tmp = bas ; tmp != son->next ; tmp = tmp->next)
        {
            GeciciBagliListe();
        }
        while(ptr != son)
        {
            right();
        }
        while(ptr != bas->prev)
        {
            sil();
        }
        yazdir();
        son = NULL;
        bas = NULL;
        flag1 = 0;
        flag  = 0;
    }
}
void SelectAll()
{
    bas = first;
    while(tmp!=NULL)
    {
        tmp=tmp->next;
    }
    son = tmp;
}
