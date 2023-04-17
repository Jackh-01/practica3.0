#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;


string text2bin(string texto);
string bin2text(string bin);

string decodificar(string archivo, int metodo, int semilla);
string cambio_debit1(string partido, string partido2);
string intercambio(string bi, int c);
string cambio_debit2(string bi);


string codificar(string archivo, int metodo, int semilla);
string cambio_bit1(string partido, string partido2);
string cambio_bit2(string bi);


void aplicacion(string archivo_administrador, string archivo_clientes, int metodo, int semilla);



string leer_txt(string name);
bool validar_administrador(string archivo, string usuario_ingresado, string clave_ingresado);
string agregar_clientes(string archivo);

string validar_clientes(string archivo, string cedula_ingresada, string clave_ingresada);
string retirar_dinero(string saldo);
string verificar_saldo(string saldo);
string configurar_archivo_clientes(string archivo, string cedula, string clave, string saldo);

void escribir_txt(string name, string data);

int main()
{
    string archivo_administradores = "../sudo.txt";
    string archivo_clientes = "../datos.txt";
    int metodo, semilla;

    cout << "Ingresar el metodo para la codificacion: ";
    cin >> metodo;
    cout << "Ingresar la semilla para la codificacion: ";
    cin >> semilla;
    aplicacion(archivo_administradores, archivo_clientes, metodo, semilla);
    return 0;
}


string text2bin(string texto){
  string binario;
  for (unsigned long long int i=0;i<texto.length();i++){
    for (int j=0;j<8;j++){
      binario.push_back(char(((texto[i]<<j)&(0x80))/128+48));
    }
  }
  return binario;
}

string bin2text(string bin){
    string bin2, texto;
    unsigned long long int i, exponente, calculo, k=0;

    for (i=0;i<bin.length();i++){
        bin2.push_back(bin[i]);
        if (i==(k+1)*8-1){
            calculo=0;
            exponente=0;
            for (int c=7;c>=0;c--){
                if (bin2[c]=='1') calculo+=pow(2,exponente);
                exponente++;
            }
            texto+= calculo;
            k++;
            bin2.clear();
        }
    }
    return texto;
}

string decodificar(string archivo, int metodo, int semilla){
    if (metodo==1){
        string archivo2, copia, copia2;
        unsigned int k=0;
        for (unsigned long long int i=0;i<archivo.length();i++){
            copia.push_back(archivo[i]);
            if (k<1){
                if (archivo[i]=='1') archivo2.push_back('0');
                else archivo2.push_back('1');

                if (i==semilla-1){
                    copia2=archivo2;
                    copia.clear();
                    k++;
                }

            }
            else if (i==archivo.length()-1){
                copia2=cambio_debit1(copia, copia2);
                archivo2=archivo2+copia2;
            }
            else{
                if (i==(k+1)*semilla-1){
                    copia2=cambio_debit1(copia, copia2);
                    archivo2=archivo2+copia2;
                    copia.clear();
                    k++;
                }
            }
        }
        return archivo2;
    }

    else{
        string archivo2, copia;
        unsigned int k=0;

        for(unsigned int i=0;i<archivo.length();i++){
            copia.push_back(archivo[i]);
            if (i==(k+1)*semilla-1){
                archivo2+=cambio_debit2(copia);
                copia.clear();
                k++;

            }
            else if (i==archivo.length()-1){
                archivo2+=cambio_debit2(copia);
            }
        }
        return archivo2;
    }
}

string cambio_debit1(string partido, string partido2){
    long int unos=0,ceros=0;
    for (unsigned long long int i=0;i<partido.length();i++){
        if (partido2[i]=='1') unos++;
        else ceros++;
    }

    if (unos==ceros){
        for(unsigned int c=0;c<partido.length();c++){
            partido=intercambio(partido, c);
        }
    }

    else if(ceros>unos){
        for (unsigned int c=1;c<partido.length();c+=2){
            partido=intercambio(partido, c);
        }
    }

    else{
        for (unsigned int c=2;c<partido.length();c+=3){
            partido=intercambio(partido, c);
        }
    }
    return partido;
}

string intercambio(string bi, int c){
    if(bi[c]=='1') bi[c]='0';
    else bi[c]='1';
    return bi;
}

string cambio_debit2(string bi){
    unsigned int i;
    string bi2, copia;
    for (i=1;i<=bi.length();i++){
        if (i==bi.length()) bi2+=bi[0];
        else bi2+=bi[i];
    }
    return bi2;
}


string codificar(string archivo, int metodo, int semilla){
    if (metodo==1){
        string copia, copia2, archivo2;
        for (unsigned long long int i=0, k=0;i<archivo.length();i++){
              copia.push_back(archivo[i]);
              if (k<1){
                  if (archivo[i]=='1') archivo2.push_back('0');
                  else archivo2.push_back('1');

                  if (semilla-1==i){
                      copia2=copia;
                      copia.clear();
                      k++;
                  }
              }
              else if (i==archivo.length()-1){
                  archivo2=archivo2+cambio_bit1(copia,copia2);
              }

              else{
                  if ((k+1)*semilla-1==i){
                    archivo2=archivo2+cambio_bit1(copia,copia2);
                    copia2=copia;
                    copia.clear();
                    k++;
                  }
              }
        }
        return archivo2;
    }

    else{
        string archivo2, copia;
        unsigned int k=0;

        for(unsigned long long int i=0;i<archivo.length();i++){
            copia.push_back(archivo[i]);
            if (i==(k+1)*semilla-1){
                archivo2+=cambio_bit2(copia);
                copia.clear();
                k++;
            }
            else if (i==archivo.length()-1){
                archivo2+=cambio_bit2(copia);
            }
        }
        return archivo2;
    }
}

string cambio_bit1(string partido, string partido2){
    long int unos=0,ceros=0;
    for (unsigned long long int i=0;i<partido.length();i++){
        if (partido2[i]=='1') unos++;
        else ceros++;
    }

    if (unos==ceros){
        for(unsigned int c=0;c<partido.length();c++){
            partido=intercambio(partido, c);
        }
    }

    else if(ceros>unos){
        for (unsigned int c=1;c<partido.length();c+=2){
            partido=intercambio(partido, c);
        }
    }

    else{
        for (unsigned int c=2;c<partido.length();c+=3){
            partido=intercambio(partido, c);
        }
    }
    return partido;
}

string cambio_bit2(string bi){
    unsigned int i;
    string bi2, copia;
    copia=bi[bi.length()-1];
    for (i=0;i<bi.length();i++){
        bi2+=copia;
        copia=bi[i];
    }
    return bi2;
}

string leer_txt(string name){
  long long int tam;
  string data;
  fstream k(name, fstream::in | fstream::ate | fstream::binary);
  if(k.is_open()){
    tam=k.tellg();
    k.seekg(0);
    for(long long int i=0;i<tam; i++) data.push_back(k.get());
  }
  else cout<<"El archivo de texto no existe"<<endl;
  k.close();
  return data;
}

bool validar_administrador(string archivo, string usuario_ingresado, string clave_ingresado){

    string usuario_confirmar, clave_confirmar;
    bool ingreso=false;

    for(unsigned int i=0; i<archivo.length(); i++){
        for(;archivo[i]!=' '; i++){
            usuario_confirmar.push_back(archivo[i]);
        }
        for(i++;archivo[i]!='\n'; i++){
            clave_confirmar.push_back(archivo[i]);
        }
        if(usuario_ingresado==usuario_confirmar && clave_ingresado==clave_confirmar){
            cout << "Usted ha ingresado." << endl;
            ingreso=true;
        }
        else{
            usuario_confirmar.clear();
            clave_confirmar.clear();
        }
    }
    return ingreso;
}

string agregar_clientes(string archivo){
    string cedula, clave, saldo;

    cout << "Ingresar cedula: ";
    cin >> cedula;
    cout << "Ingresar clave: ";
    cin >> clave;
    cout << "Ingresar saldo: ";
    cin >> saldo;
    archivo = archivo + cedula + ";" + clave + ";" + saldo + "\n";
    return archivo;
}

string validar_clientes(string archivo, string cedula_ingresada, string clave_ingresada){
    string confirmar_cedula, confirmar_clave, saldo;

    for(unsigned int i=0; i<archivo.length(); i++){
        for(;archivo[i]!=';'; i++){
            confirmar_cedula.push_back(archivo[i]);
        }
        for(i++;archivo[i]!=';'; i++){
            confirmar_clave.push_back(archivo[i]);
        }
        for(i++;archivo[i]!='\n'; i++){
               saldo.push_back(archivo[i]);
        }
        if(cedula_ingresada==confirmar_cedula && clave_ingresada==confirmar_clave){
            cout << "Usted ha ingresado." << endl;
            return saldo;
        }
        else{
            confirmar_cedula.clear();
            confirmar_clave.clear();
            saldo.clear();
        }
    }
}

string retirar_dinero(string saldo){
    long int saldoint, dinero_retirar;
    istringstream(saldo) >> saldoint;
    saldoint -= 1000;
    while(true){
        cout << "Su saldo es de: " << saldoint << endl;
        cout << "Cuanto dinero desea retirar: ";
        cin >> dinero_retirar;

        if (dinero_retirar<0) cout << "Por favor ingresar numero positivos." << endl;

        else if (saldoint>=dinero_retirar){
            saldoint -= dinero_retirar;
            saldo = to_string(saldoint);
            return saldo;
        }

        else cout << "Usted no posee esa cantidad de dinero" << endl;
    }
}

string verificar_saldo(string saldo){
    long int saldoint;
    istringstream(saldo) >> saldoint;
    saldoint -= 1000;
    cout << "Su saldo es de: " << saldoint << endl;
    saldo = to_string(saldoint);
    return saldo;
}

string configurar_archivo_clientes(string archivo, string cedula, string clave, string saldo){
    string confirmar_cedula, confirmar_clave;
    string archivo2, saldo2;

    for(unsigned int i=0; i<archivo.length(); i++){
        for(;archivo[i]!=';'; i++){
            confirmar_cedula.push_back(archivo[i]);
        }
        for(i++;archivo[i]!=';'; i++){
            confirmar_clave.push_back(archivo[i]);
        }
        for(i++;archivo[i]!='\n'; i++){
            saldo2.push_back(archivo[i]);
        }

        if(cedula==confirmar_cedula && clave==confirmar_clave){
            archivo2 = archivo2 + cedula + ";" + clave + ";" + saldo + "\n";
        }
        else{
            archivo2 = archivo2 + cedula + ";" + clave + ";" + saldo2 + "\n";
            confirmar_cedula.clear();
            confirmar_clave.clear();
            saldo2.clear();
        }
    }
    return archivo2;
}



void escribir_txt(string name, string data){
    fstream k(name, fstream::out | fstream::binary);
    k.write(data.c_str(), data.length());
    k.close();
}

void aplicacion(string archivo_administradores, string archivo_clientes, int metodo, int semilla){
    int tipo_persona;
    string datos_administradores, datos_clientes;
    string usuario, clave, saldo;
    bool validar=false;

    datos_administradores = leer_txt(archivo_administradores);
    datos_administradores = text2bin(datos_administradores);
    datos_administradores = decodificar(datos_administradores, metodo, semilla);
    datos_administradores = bin2text(datos_administradores);

    datos_clientes = leer_txt(archivo_clientes);
    datos_clientes = text2bin(datos_clientes);
    datos_clientes = decodificar(datos_clientes, metodo, semilla);
    datos_clientes = bin2text(datos_clientes);

    cout << datos_administradores << endl;
    cout << datos_clientes << endl;


    while(true){

        cout << " ------------------" << endl;
        cout << "|Administrador (1) |" << endl;
        cout << "|Cliente       (2) |" << endl;
        cout << "|Salir         (3) |" << endl;
        cout << " ------------------" << "\n\n";

        cin >> tipo_persona;
        if(tipo_persona==1){
            cout << "Ingresar su usuario de administrador: ";
            cin >> usuario;
            cout << "Ingresar su clave de administrador: ";
            cin >> clave;
            cout << datos_administradores << endl;
            validar = validar_administrador(datos_administradores, usuario, clave);
            if (validar){

                while (true){

                    cout << " ------------------" << endl;
                    cout << "|Crear perfil  (1) |" << endl;
                    cout << "|Salir         (2) |" << endl;
                    cout << " ------------------" << "\n\n";

                    cin >> tipo_persona;
                    if(tipo_persona==1){
                        datos_clientes = agregar_clientes(datos_clientes);
                    }

                    else if (tipo_persona==2) break;

                    else cout << "Debe de ingresar una de las siguientes opciones." << endl;
                }
            }

            else cout << "El usuario o la contraseña son incorrectas." << endl;
        }

        else if(tipo_persona==2){
            cout << "Ingresar su cedula: ";
            cin >> usuario;
            cout << "Ingresar su clave: ";
            cin >> clave;
            cout << datos_clientes << endl;
            saldo = validar_clientes(datos_clientes, usuario, clave);
            //cout << "validar: " << validarstr << endl;

            if (saldo!=""){

                while(true){

                    cout << " --------------------" << endl;
                    cout << "|Retirar dinero  (1) |" << endl;
                    cout << "|Verificar saldo (2) |" << endl;
                    cout << "|Salir           (3) |" << endl;
                    cout << " --------------------" << "\n\n";

                    cin >> tipo_persona;
                    if (tipo_persona==1){
                        saldo = retirar_dinero(saldo);
                        datos_clientes = configurar_archivo_clientes(datos_clientes, usuario, clave, saldo);
                    }

                    else if (tipo_persona==2){
                        saldo = verificar_saldo(saldo);
                        datos_clientes = configurar_archivo_clientes(datos_clientes, usuario, clave, saldo);
                    }

                    else if (tipo_persona==3) break;

                    else cout << "Debe de ingresar una de las siguientes opciones." << endl;
                }
            }

            else cout << "Su cedula o clave son incorrectas." << endl;
        }

        else if (tipo_persona==3) break;

        else cout << "Debe de elegir una de las siguientes opciones." << "\n\n";
    }

    datos_clientes = text2bin(datos_clientes);
    datos_clientes = codificar(datos_clientes, metodo, semilla);
    datos_clientes = bin2text(datos_clientes);
    escribir_txt("../datos.txt", datos_clientes);

    datos_administradores = text2bin(datos_administradores);
    datos_administradores = codificar(datos_administradores, metodo, semilla);
    datos_administradores = bin2text(datos_administradores);
    escribir_txt("../sudo.txt", datos_administradores);

    cout << "archivos guardados." << endl;
}
