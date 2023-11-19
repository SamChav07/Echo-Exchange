#include <iostream>
#include <cstring>
#include <string>

#include <cstdio>

#include <iomanip>
#include <unistd.h>

// libreria para all_of y ::isdigit
#include <algorithm>
#include <functional>
#include <cctype>

#include <fstream>
using namespace std;

// Definici�n de estructuras
const int MAX = 100;

struct accounts
{
    char adminU[50]; // "00017"
    char admP[50];   // "admPass007"
};

struct empleado
{
    char nombre[50];
    char empU[50]; // "00016"
    char empP[50]; // "enployee123"
    string empmail;
    string emptel;
} emp;

struct cliente
{
    char client_name[50];
    char client_lastname[50];
    int client_id;
    char client_mail[50];
    char client_telf[10];
} clt[MAX];

struct cmpr_fecha
{
    int day;
    int month;
    int year;
} fCmp[MAX];

struct reg_compra
{
    cliente clt;
    int cmpr_id;
    cmpr_fecha fCmp;
    float cmpr_Sqty; // monto de la compra
    int cmpr_pts;
    float cmp_iva;
    float cmpr_Tqty;
} cmp[MAX];

struct historial
{
    reg_compra r_cmpr;
    int cmpr_ptsTot;
} hst;

struct gift
{
    int gft_id;
    int gft_cant;
    char gft_name[50];
    int gft_pts;
} gft[MAX];
// fin de struct

int cntClt();
// LOG
void SuperAdmin();
void logAdm();
void logClt();

// menus
void MDclient(); //--listo
void MDcmp();
void MDgift();     // vacio
void searchMclt(); // menu de busqueda de cliente   //--listo

void MAdm();
void MClt();

// CRUD clt
// create
void addClt(cliente currentClt); //--listo
void initClt(int cltPos);        //--listo
// read

void showClt(int pos); //--listo
void showCltRegister();
cliente getClt(int pos);
void searchCltname(char client_name[]);      //--listo
void searchCltlstname(char *client_lstName); //--listo
int searchCltId(int enteredClt_id);          // listo
void searchCltmail(char client_mail[]);      //--listo
void searchCltelf(char client_telf[]);       //--listo
// uptade
void uptdClt(cliente cliente, int i); // --listo
// delete
void delClt(int pos); // --listo

// compra
void addCmp(reg_compra currentCmp);
void showCmpRegister(int i);
void showCmp(int pos);
reg_compra getCmp(int pos);
int searCmpFID(int entered_id);

// create gft
void addGft(gift currentGft);
void initGft(int gftPos);

// funciones
void record();
void MDrewards(); // -------------pendiente
void redeem();    // -------------pendiente
void checkPts();  // -------------pendiente

// archivos clt
FILE *cltRegister;
void saveClt();
void readClt();
int cntClt(FILE *cltRegister);
int getLstCltID();

// variables globales
char user[50], pass[50];
int intentos = 4;

int lastRegClt = 0;
int lasTregGft = 0;
int lastREgCmp = 0;
int proxIDclt = 123;

int idCmp = 321;

// codigos colores ANSI
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

void saveClt()
{
    cltRegister = fopen("clientes.txt", "w");
    if (cltRegister == NULL)
    {
        cerr << "Error al abrir el archivo de clientes para escribir..." << endl;
        return;
    }
    fwrite(clt, sizeof(cliente), lastRegClt, cltRegister);
    fclose(cltRegister);
}

void readClt()
{
    cltRegister = fopen("clientes.txt", "r");
    if (cltRegister == NULL)
    {
        cerr << "Error al abrir el archivo de clientes para leer" << endl;
        return;
    }
    lastRegClt = cntClt(cltRegister);
    fread(clt, sizeof(cliente), MAX, cltRegister);

    fclose(cltRegister);
}

int cntClt(FILE *cltRegister) // calcula el ultimo registro
{
    int tam_archv, num_clts;
    // obtiene el tamaño del archv
    fseek(cltRegister, 0, SEEK_END);
    tam_archv = ftell(cltRegister);
    rewind(cltRegister);

    // calc el # de clts
    num_clts = tam_archv / sizeof(cliente);
    return num_clts;
}

int getLstCltID() 
{
   cltRegister = fopen("clientes.txt", "r");
   if (cltRegister == NULL)
   {
    cerr << "Error al abrir el archivo de CLIENTES para obtener el ultimo ID..." << endl;
    return proxIDclt;
   }

   int lastId = -1;
   fseek(cltRegister, -sizeof(cliente), SEEK_END);
   if (fread(&clt[0], sizeof(cliente), 1, cltRegister) == 1)
   {
    lastId = clt[0].client_id;
   }
    fclose(cltRegister);

    return (lastId == -1) ? proxIDclt : (lastId + 1);
}

int main()
{
    readClt();
    SuperAdmin();
    return 0;
}

void SuperAdmin()
{
    int spOpcion;
    int intentos = 3;

    cout << GREEN << "EEEE  CCCC  HH  HH  OOOOOO     EEEE  XX    XX  CCCC  HH  HH  AAAAAA  NN     NN  GGGGGG   EEEE" << RESET << endl;
    cout << GREEN << "EE    CC    HH  HH  OO  OO     EE     XX  XX   CC    HH  HH  AA  AA  NN NN  NN  GG       EE" << RESET << endl;
    cout << GREEN << "EEEE  CC    HHHHHH  OO  OO === EEEE    XXXX    CC    HHHHHH  AAAAAA  NN  NN NN  GG  GGG  EEEE" << RESET << endl;
    cout << GREEN << "EE    CC    HH  HH  OO  OO     EE     XX  XX   CC    HH  HH  AA  AA  NN   NNNN  GG  GG   EE" << RESET << endl;
    cout << GREEN << "EEEE  CCCC  HH  HH  OOOOOO     EEEE  XX    XX  CCCC  HH  HH  AA  AA  NN    NNN  GGGGGG   EEEE" << RESET << endl;
    
    cout << "||==================================================||" << endl;
    cout << "Bienvenido Super-Admin" << endl;
    cout << "Que cuenta desea fijar en el dispositivo ?" << endl;
    cout << "1. Administrador." << endl;
    cout << "2. Cliente." << endl;
    cout << "3. Cerrar." << endl;
    cout << "||==================================================||" << endl;
    cout << "\n --> ";
    cin >> spOpcion;
    switch (spOpcion)
    {
    case 1:
        logAdm();
        break;
    case 2:
        logClt();
        break;
    case 3:
        // saveCSVdata(); // llama a la funcion guardar antes de cerrar la sesion
        cout << "--Gracias, por usar el sistema Echo-Exchange--" << endl;
        exit(0);
        break;
    default:
        cout << "Ingrese una opcion valida. 1 - 3..." << endl;
        break;
    }
}

void logAdm()
{
    do
    {
        accounts administrador1;
        strcpy(administrador1.adminU, "."); // admin1
        strcpy(administrador1.admP, ".");   // password123

        cout << "\n---Administrador---" << endl;
        cout << "-------------" << endl;
        cout << "Usuario: ";
        cin >> user;
        cout << "\nContrasena: ";
        cin >> pass;

        // Verificaci�n de credenciales del administrador
        if (strcmp(user, administrador1.adminU) == 0 && strcmp(pass, administrador1.admP) == 0)
        {
            cout << "\n***Acceso concedido***\n";
            MAdm();
        }
        else
        {
            cout << "*** Intento fallido. Usuario o contrasena incorrecta ***" << endl;
            cout << "Intentos restantes: \n"
                 << intentos - 1 << endl;

            intentos--;
        }
    } while (intentos > 0);
    cout << "*** Intento fallido. Usuario o contrasena incorrecta ***" << endl;
    cout << "***Acceso bloqueado, saliendo del sistema...***";
}

void MAdm()
{
    int op1;
    do
    {
        system("cls || clear");
        cout << "Bienvenido Administrador" << endl;

        cout << "***--Opciones--***" << endl;
        cout << "1. Gestionar compras." << endl;
        cout << "2. Gestionar clientes." << endl;
        cout << "3. Gestionar recompensas." << endl;
        cout << "4. Cerrar Sesion" << endl;
        cout << "\n --> ";
        cin >> op1;
        system("pause");

        switch (op1)
        {
        case 1:
            MDcmp();
            break;
        case 2:
            MDclient();
            break;
        case 3:
            MDrewards();
            break;
        case 4:
            system("cls");
            logAdm();
            break;
        default:
            cout << "Ingrese opciones validas. Sean de 1-4..." << endl;
            break;
        }
    } while (op1 != 4);
}

void logClt()
{
    do
    {
        empleado empleado1;
        strcpy(empleado1.empU, "usuario1");
        strcpy(empleado1.empP, "password123");

        int intentos = 4;

        cout << "---Cliente---" << endl;
        cout << "-------------" << endl;

        cout << "\nUsuario: ";
        cin >> user;
        cout << "\nContrasena: ";
        cin >> pass;

        // Verificaci�n de credenciales del cliente
        if (strcmp(user, empleado1.empU) == 0 && strcmp(pass, empleado1.empP) == 0)
        {
            cout << "\n***Acceso concedido***\n";
            MClt();
        }
        else
        {
            cout << "\n*** Intento fallido. Usuario o contrase�a incorrecta ***" << endl;
            cout << "Intentos restantes: " << intentos - 1 << endl;
            intentos--;
        }
    } while (intentos > 0);
    cout << "\n*** Intento fallido. Usuario o contrase�a incorrecta ***" << endl;
    cout << "***Acceso bloqueado, saliendo del sistema...***";
}

void MClt()
{
    int op2;
    do
    {
        cout << "\nBienvenido Empleado" << endl;
        cout << "\n***--Opciones--***" << endl;
        cout << "\n1.. Canjear puntos." << endl;
        cout << "\n2. Consultar puntos." << endl;
        cout << "\n3. Historial de compra." << endl;
        cout << "\n4. Cerrar Sesion." << endl;
        cout << "\n --> ";
        cin >> op2;
        system("pause");

        switch (op2)
        {
        case 1:
            redeem();
            break;
        case 2:
            checkPts();
            break;
        case 3:
            record();
            break;
        case 4:
            system("cls");
            logClt();
            break;
        default:
            cout << "Ingrese opciones validas. Sean de 1-4.." << endl;
            break;
        }
    } while (op2 != 4);
}

void MDclient()
{
    readClt();

    cliente currentClt;
    int pos, resp;
    int enteredClt_id;

    int optMdclt;
    system("cls || clear");
    cout << "Clientes" << endl;
    cout << "Cantidad de registros de clientes: " << lastRegClt << endl;
    cout << "***--Opciones--***" << endl;
    cout << "1. Agregar cliente." << endl;
    cout << "2. Buscar cliente." << endl;
    cout << "3. Modificar cliente." << endl;
    cout << "4. Eliminar cliente." << endl;
    cout << "5. Atras..." << endl;
    cout << "\n --> ";
    cin >> optMdclt;
    system("pause");

    switch (optMdclt)
    {
    case 1:
        system("cls || clear");
        cout << "** Ingrese los datos a añadir **" << endl;
        system("cls || clear");
        currentClt.client_id = getLstCltID();

        if (currentClt.client_id == -1)
        {
            cerr << "Error al obtener el último ID. No se pudo agregar el cliente..." << endl;
            system("pause || read -p 'Presiona Enter para continuar...' -n 1 -s");
            return;
        }

        cout << "ID: " << currentClt.client_id << endl;

        // Nombre del cliente
        do
        {
            cout << "Nombre del cliente: ";
            cin.ignore(); // Ignorar el carácter de nueva línea residual
            cin.getline(currentClt.client_name, sizeof(currentClt.client_name));
            if (any_of(currentClt.client_name, currentClt.client_name + strlen(currentClt.client_name), ::isdigit))
            {
                cout << "Ingrese un nombre válido, solo con caracteres." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

        } while (any_of(currentClt.client_name, currentClt.client_name + strlen(currentClt.client_name), ::isdigit));

        // Apellido del cliente
        do
        {
            cout << "Apellido del cliente: ";
            cin.getline(currentClt.client_lastname, sizeof(currentClt.client_lastname));
            if (any_of(currentClt.client_lastname, currentClt.client_lastname + strlen(currentClt.client_lastname), ::isdigit))
            {
                cout << "Ingrese un apellido válido, solo con caracteres." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

        } while (any_of(currentClt.client_lastname, currentClt.client_lastname + strlen(currentClt.client_lastname), ::isdigit));

        // E-mail del cliente
        do
        {
            cout << "E-mail del cliente: ";
            cin.getline(currentClt.client_mail, sizeof(currentClt.client_mail));
            if (strstr(currentClt.client_mail, "@gmail.com") == NULL)
            {
                cout << "El correo debe terminar en @gmail.com. Inténtelo de nuevo..." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

        } while (strstr(currentClt.client_mail, "@gmail.com") == NULL);

        // Teléfono del cliente
        do
        {
            cout << "Teléfono del cliente XXXX-XXXX: ";
            cin.getline(currentClt.client_telf, sizeof(currentClt.client_telf));

            // Verificar si el formato del número de teléfono es correcto
            if (!(strlen(currentClt.client_telf) == 8))
            {
                cout << "Ingrese un número telefónico válido. Use el formato XXXX-XXXX y solo incluya dígitos." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

        } while (!(strlen(currentClt.client_telf) == 8));

        addClt(currentClt);
        saveClt();
        // saveCSVdata(); // llama a la función guardar antes de cerrar la sesión
        system("pause || read -p 'Presiona Enter para continuar...' -n 1 -s");
        MDclient();
        break;

    case 2:
        searchMclt();
        break;
    case 3:
        system("cls || clear");
        cout << "** Escribe el ID del cliente a modificar **" << endl;
        cin >> enteredClt_id;
        pos = searchCltId(enteredClt_id);
        if (pos != -1)
        {
            system("cls || clear");
            showClt(pos);
            cout << "Datos a modificar: " << endl;
            cout << "ID: ";
            cin >> currentClt.client_id;
            cout << "Nombre del cliente: ";
            scanf(" %[^\n]", currentClt.client_name);
            cout << "Apellido del cliente: ";
            scanf(" %[^\n]", currentClt.client_lastname);
            cout << "E-mail del cliente: ";
            scanf(" %[^\n]", currentClt.client_mail);
            cout << "Telefono del cliente: ";
            scanf(" %[^\n]", currentClt.client_telf);

            cout << "Registro actualizado...\n";
            system("cls || clear");
            uptdClt(currentClt, pos);
            // saveCSVdata();
        }
        else
        {
            cout << "Registro inexistente" << endl;
        }
        MDclient();
        saveClt();
        system("pause || read -p 'Presiona Enter para continuar...' -n 1 -s");
        break;
    case 4:
        system("cls || clear");
        if (lastRegClt == 0)
        {
            cout << "No hay nada que eliminar\n";
            break;
        }
        cout << "Escribe el ID del cliente: " << endl;
        cin >> enteredClt_id;
        pos = searchCltId(enteredClt_id);
        if (pos != -1)
        {
            currentClt = getClt(pos);
            cout << "¿Realmente deseas eliminar el cliente ?" << endl;
            cout << "Escriba 1 para SI o 2 para NO : ";
            cin >> resp;
            if (resp == 1)
            {
                delClt(pos);
                cout << "Registro eliminado...\n";
            }
            else
            {
                cout << "Operacion cancelada...\n";
            }
            system("pause || read -p 'Presiona Enter para continuar...' -n 1 -s");
        }
        else
        {
            cout << "Registro inexistente" << endl;
        }
        saveClt();
        break;
    case 5:
        MAdm();
        break;

    default:
        cout << "Ingrese una opcion valida. Del 1 - 5..." << endl;
        break;
    }
}

void MDcmp()
{
    readClt();

    string regreso;
    reg_compra currentCmp;
    int cmpOPt;
    do
    {
        system("cls || clear");
        cout << "Modulo Compra" << endl;
        cout << "**OPCIONES**" << endl;
        cout << "1. Registro de compra." << endl;
        cout << "2. Historial de compras." << endl;
        cout << "3. Atras..." << endl;
        cout << "\n --> ";
        cin >> cmpOPt;

        switch (cmpOPt)
        {
        case 1:
            system("cls || clear");
            cout << "\tRegistro de Compras" << endl;
            cout << "______________________________" << endl;
            cout << "** Ingrese los datos a añadir **" << endl;
            cout << "------------------------------" << endl;
            system("cls || clear");
            cout << "ID del cliente: ";
            cin >> currentCmp.clt.client_id;

            cout << "N* de Recibo: ";
            cin >> currentCmp.cmpr_id;

            cout << "Fecha de compra: ";
            cin >> currentCmp.fCmp.day; cout << "-"; cin >> currentCmp.fCmp.month; cout << "-"; cin >> currentCmp.fCmp.year;

            cout << "Monto de compra: C$ ";
            cin >> currentCmp.cmpr_Sqty;

            currentCmp.cmp_iva = currentCmp.cmpr_Sqty * 0.15;
            cout << "Iva: " << currentCmp.cmp_iva << endl;
            
            currentCmp.cmpr_Tqty = currentCmp.cmpr_Sqty + currentCmp.cmp_iva;
            cout << "Total: C$ " << currentCmp.cmpr_Tqty << endl;

            currentCmp.cmpr_pts = currentCmp.cmpr_Tqty / 180;
            cout << "Puntos por compra: " << currentCmp.cmpr_pts << " pts" << endl;
            cout << "||=========================||" << endl;
            // cout << "¿Desea volver a registrar otra compra o salir al menu? (v/s): " << endl;

            addCmp(currentCmp);
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            break;
        case 2:
            record();
            break;
        case 3:
            MAdm();
            break;
        }
    } while (cmpOPt != 3);
}

void record()
{
    int i;
    int enteredClt_id;
    cout << "ID de cliente: ";
    cin.ignore();
    cout << "" << endl;
    cin >> enteredClt_id;
    searCmpFID(enteredClt_id);
    showCmpRegister(i);
    system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
    MDcmp();
    system("cls || clear");
}

void MDgift()
{
    gift currentGft;
    int pos, resp;
}

void addClt(cliente currentClt)
{
    if (lastRegClt < MAX)
    {
        clt[lastRegClt] = currentClt;
        lastRegClt++;
        proxIDclt++;
    }
    else
    {
        cout << "Clientes esta en Maxima capacidad..." << endl;
    }
}

void addGft(gift currentGft)
{
    if (lasTregGft < MAX)
    {
        gft[lasTregGft] = currentGft;
        lasTregGft++;
    }
    else
    {
        cout << "Recompensas esta en Maxima capacidad..." << endl;
    }
}

void initClt(int pos) // inicializa los datos del cliente
{
    clt[pos].client_id = 0;

    strncpy(clt[pos].client_name, "", sizeof(clt[pos].client_name));

    strncpy(clt[pos].client_lastname, "", sizeof(clt[pos].client_lastname));

    strncpy(clt[pos].client_mail, "", sizeof(clt[pos].client_mail));

    strncpy(clt[pos].client_telf, "", sizeof(clt[pos].client_telf));
}

/*void initGft(int gftPos)
{
    gft[pos].gft_id = 0;

    strncpy(gft[pos].client_name, "", sizeof(clt[pos].client_name));
    clt[pos].client_name[sizeof(clt[pos].client_name) - 1] = '\0';
}*/

void searchMclt()
{
    // loadCSVdata();
    int options, pos;
    char enteredClt_name[50];
    char enteredClt_lstName[20];
    int enteredClt_id = 0;
    char enteredClt_mail[50];
    char enteredClt_telf[15];

    do
    {
        system("cls || clear");
        cout << "** Menu de busqueda **" << endl;
        cout << "1. Nombre." << endl;
        cout << "2. Apellido." << endl;
        cout << "3. Id." << endl;
        cout << "4. E-mail." << endl;
        cout << "5. Telefono." << endl;
        cout << "6. Mostrar todos los registros." << endl;
        cout << "7. Atras..." << endl;
        cout << "\n --> ";

        cin >> options;

        switch (options)
        {
        case 1:
            system("cls || clear");
            cout << "Buscar por nombre de Cliente" << endl;
            cout << "Nombre a buscar: ";
            cin.ignore(); // limpia la entrada
            cin.getline(enteredClt_name, sizeof(enteredClt_name));
            searchCltname(reinterpret_cast<char *>(&enteredClt_name));
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        case 2:
            system("cls || clear");
            cout << "Buscar por apellido de Cliente" << endl;
            cout << "Apellido a buscar: ";
            cin.ignore();
            cin.getline(enteredClt_lstName, sizeof(enteredClt_lstName));
            searchCltlstname(&enteredClt_lstName[20]);
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        case 3:
            system("cls || clear");
            cout << "Buscar por id de Cliente" << endl;
            cout << "ID a buscar: ";
            cin.ignore();
            cin >> enteredClt_id;
            searchCltId(enteredClt_id);
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        case 4:
            system("cls || clear");
            cout << "Buscar por e-mail de Cliente" << endl;
            cout << "E-mail a buscar: ";
            cin.ignore();
            cin.getline(enteredClt_mail, sizeof(enteredClt_mail));
            searchCltmail(&enteredClt_mail[50]);
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        case 5:
            system("cls || clear");
            cout << "Buscar por telefono de Cliente" << endl;
            cout << "Telefono a buscar: ";
            cin.ignore();
            cin.getline(enteredClt_telf, sizeof(enteredClt_telf));
            searchCltelf(&enteredClt_telf[15]);
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        case 6:
            system("cls || clear");
            cout << "Visualizando todos los registros..." << endl;
            showCltRegister();
            system("cls || clear");
            break;
        case 7:
            system("cls || clear");
            cout << "Saliendo..." << endl;
            MDclient();
            system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
            system("cls || clear");
            break;
        default:
            cout << "Opcion Invalida. Ingrese una opcion valida del 1 - 7" << endl;
            break;
        }

    } while (options != 7);
}

void showClt(int pos) // muestra los datos del cliente en X posición
{
    system("cls || clear");
    cout << "====================================" << endl;
    cout << "ID: ";
    cout << clt[pos].client_id << endl;
    cout << "Nombre: ";
    cout << clt[pos].client_name << endl;
    cout << "Apellido: ";
    cout << clt[pos].client_lastname << endl;
    cout << "E-mail: ";
    cout << clt[pos].client_mail << endl;
    cout << "Telefono: ";
    cout << clt[pos].client_telf << endl;

    system("pause || read -p 'Presiona enter para continuar...' -n 1 -s");
    system("cls || clear");
}

void showCltRegister()
{
    system("cls || clear");
    if (lastRegClt == 0)
    {
        cout << "No hay registros" << endl;
        return;
    }
    cout << "Registro de Clientes: " << endl;
    cout << "====================================" << endl;

    for (int i = 0; i < lastRegClt; i++)
    {
        cout << "Cliente #" << i + 1 << endl;
        showClt(i);
    }
    cout << "====================================" << endl;
    cout << "Ultimo registro..." << endl;
}

cliente getClt(int pos)
{
    return clt[pos];
}

void searchCltname(char *enteredClt_name) // funcion de prueba
{
    int position = 0;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (strcmp(enteredClt_name, clt[i].client_name) == 0)
        {
            cout << "=====" << endl;
            showClt(i);
            position++;
        }
        if (position == 0)
        {
            cout << "No se encontraron resultados..." << endl;
        }
    }
}

void searchCltlstname(char *enteredClt_lstName) // funcion de prueba
{
    int position = 0;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (strcmp(enteredClt_lstName, clt[i].client_lastname) == 0)
        {
            cout << "=========================" << endl;
            showClt(i);
            position++;
        }

        if (position == 0)
        {
            cout << "No se encontraron resultados" << endl;
        }
    }
}

int searchCltId(int enteredClt_id) // esta funcional
{
    int position = 0;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (enteredClt_id == (clt[i].client_id))
        {
            cout << "=========================" << endl;
            showClt(i);
            position++;
        }
    }

    if (position == 0)
    {
        cout << "No se encontraron resultados" << endl;
    }
    return position;
}

void searchCltmail(char *enteredClt_mail) // prueba
{
    int position = 0;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (strcmp(enteredClt_mail, clt[i].client_mail) == 0)
        {
            showClt(i);
            position++;
        }

        if (position == 0)
        {
            cout << "No se encontraron resultados" << endl;
        }
    }
}

void searchCltelf(char enteredClt_telf[]) // prueba
{
    int position = 0;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (strcmp(enteredClt_telf, clt[i].client_telf) == 0)
        {
            showClt(i);
            position++;
        }

        if (position == 0)
        {
            cout << "No se encontraron resultados" << endl;
        }
    }
}

void uptdClt(cliente currentClt, int pos)
{
    clt[pos] = currentClt;
}

void delClt(int pos)
{
    if (pos == lastRegClt)
    {
        cout << "No hay registros " << endl;
        return;
    }
    for (int i = pos; i < lastRegClt - 1; i++)
    {
        clt[i] = clt[i + 1];
    }
    lastRegClt--;
    proxIDclt--;
    initClt(lastRegClt);
}

void addCmp(reg_compra currentCmp)
{
    if (lastREgCmp < MAX)
    {
        cmp[lastREgCmp] = currentCmp;
        lastREgCmp++;
    }
    else
    {
        cout << "El Registro de compras esta en Maxima capacidad..." << endl;
    }
}

void showCmpRegister(int i) // pendiente
{
    system("cls || clear");
    if (lastREgCmp == 0)
    {
        cout << "No hay registros" << endl;
        return;
    }
    cout << "Registro de de compras: " << endl;
    cout << "====================================" << endl;

    for (int i = 0; i < lastREgCmp; i++)
    {
        cout << "Compra #" << i + 1 << endl;
        showCmp(i);
    }
    cout << "====================================" << endl;
    cout << "Ultimo registro..." << endl;
}

void showCmp(int pos) // muestra los datos del cliente en X posición
{
    cout << "====================================" << endl;
    cout << "N* de Recibo: " << cmp[pos].cmpr_id << endl;
    cout << "Fecha de compra: " << cmp[pos].fCmp.day << " - " << cmp[pos].fCmp.month << " - " << cmp[pos].fCmp.year << endl;
    cout << "Monto de compra: C$ " << cmp[pos].cmpr_Sqty << endl;
    cout << "Iva: C$ " << cmp[pos].cmp_iva << endl;
    cout << "Total: C$ " << cmp[pos].cmpr_Tqty << endl;
    cout << "Puntos por compra: " << cmp[pos].cmpr_pts << " pts" << endl;
    cout << "====================================" << endl;
}

reg_compra getCmp(int pos)
{
    return cmp[pos];
}

int searCmpFID(int entered_id)
{
    int position = -1;
    for (int i = 0; i < lastRegClt; i++)
    {
        if (entered_id == (clt[i].client_id))
        {
            cout << "=========================" << endl;
            showCmpRegister(i);
            position++;
        }
    }
    if (position == -1)
    {
        cout << "No se encontraron resultados" << endl;
    }
    return position;
}

void MDrewards() // -------------pendiente
{
    cout << "||=========================||" << endl;
    cout << "--------Echo-Exchange--------" << endl;
    cout << "" << endl;
    cout << "\tCRUD rewards" << endl;
    cout << "______________________________" << endl;
    cout << "------------------------------" << endl;
    cout << " Agregar - Editar - Eliminar " << endl;
    cout << "¿Desea volver Agregar, Editar, Eliminar recompensas? (a/ e/ d): " << endl;
    cout << "a" << endl;
    cout << "=============================" << endl;
    cout << "ID-gft:    00190" << endl;
    cout << "Nombre de la recompensa:    MX log S2" << endl;
    cout << "Cantidad:    02" << endl;
    cout << "Puntos necesarios:  320 pts" << endl;
    cout << "||=========================||" << endl;
    cout << "¿Desea volver al CRUD o salir al menu? (v/s): " << endl;
}
void redeem() // -------------pendiente
{
    cout << "||=========================||" << endl;
    cout << "--------Echo-Exchange--------" << endl;
    cout << "" << endl;
    cout << "\tSeleccion de recompensas" << endl;
    cout << "______________________________" << endl;
    cout << "------------------------------" << endl;
    cout << "Puntos actuales:   495" << endl;
    cout << "=======================" << endl;
    cout << "ID:    123" << endl;
    cout << "Cantidad:  2" << endl;
    cout << "Descuento del 20%" << endl;
    cout << "Puntos necesarios: 295" << endl;
    cout << "+++++++++++++++++++++++" << endl;
    cout << "ID:    234" << endl;
    cout << "Cantidad:  3" << endl;
    cout << "Producto Gratis" << endl;
    cout << "Puntos necesarios: 500" << endl;
    cout << "+++++++++++++++++++++++" << endl;
    cout << "ID:    345" << endl;
    cout << "Cantidad:  1" << endl;
    cout << "Bonus" << endl;
    cout << "Puntos necesarios: 700" << endl;
    cout << "=======================" << endl;
    cout << "Elija su recompensa: \n(escriba el ID correspondiente)" << endl;
    cout << "ID:    123" << endl;
    cout << "***Transaccion exitosa***" << endl;
    cout << "||=========================||" << endl;
    cout << "¿Desea volver a mostrar de nuevo su historial o salir al menu? (v/s): " << endl;
}
void checkPts() // -------------pendiente
{
    cout << "Visualizar puntos" << endl;
}
