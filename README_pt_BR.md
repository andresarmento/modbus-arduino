Biblioteca Modbus para Arduino
==============================

Esta biblioteca permite que seu arduino se comunique através do protocolo Modbus.
O Modbus é um protocolo do tipo mestre-escravo, utilizado em automação industrial,
podendo ser utilizado em outras áreas, como por exemplo, na automação residencial.

O Modbus geralmente utiliza como meio físico as interfaces seriais RS-232 ou RS-485
(quando é chamado Modbus Serial) e TCP/IP via Ethernet ou WiFi (Modbus IP).

Na versão atual a biblioteca permite que o arduino opere como escravo, suportando
tando Modbus Serial quanto Modbus IP. Para mais informações sobre o Modbus consulte:

http://pt.wikipedia.org/wiki/Modbus
http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf
http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf

<b>Nota do autor (motivação e agradecimentos):</b>

Tudo começou quando eu encontrei a biblioteca Arduino Modbus RTU de Juan Pablo Zometa
e estendi a biblioteca para suportar outras funções Modbus.

Depois de pesquisar várias outras bibliotecas Modbus percebi pontos fortes
em fracos em todas elas. Pensei também que serial legal ter uma biblioteca base para
Modbus e derivá-la para cada tipo de meio físico utilizado.

Agradeço ao trabalho de todos os autores das outras bibliotecas, das quais utilizei
várias ideias para compor a modbus-arduino. No final deste documento estão listadas as
bibliotecas e seus respectivos autores.

Características
===============

<ul>
<li>Opera como escravo (Modo mestre em desenvolvimento) </li>
<li>Suporta Modbus Serial (RS-232 ou RS485) e Modbus IP (TCP não keep-alive)</li>
<li>Responde todos os tipos de exceção para as funções suportadas</li>
<li>Suporta as guintes funções Modbus:</li>
<ul>
    <li>0x01 - Read Coils</li>
    <li>0x02 - Read Input Status (Read Discrete Inputs)</li>
    <li>0x03 - Read Holding Registers</li>
    <li>0x04 - Read Input Registers</li>
    <li>0x05 - Write Single Coil</li>
    <li>0x06 - Write Single Register</li>
    <li>0x0F - Write Multiple Coils</li>
    <li>0x10 - Write Multiple Registers</li>
</ul>
</ul>

<b>Observações:</b>

1) Quando se usa Modbus IP o protocolo de transporte é o TCP (porta 502) e, por default, a conexão
é finalizada a cada mensagem transmitida, ou seja, não é do tipo keep-alive. Se você precisa de uma conexão
do tipo keep-alive você deverá remover o comentário desta linha no arquivo ModbusIP.h (ou ModbusIP_*.h):
```
#define TCP_KEEP_ALIVE
```

2) Os offsets para acesso aos registradores são baseados em 0. Assim, tenha cuidado
ao configurar seu seu supervisório ou utilitário de teste. Por exempo, no ScadaBR
(http://www.scadabr.com.br) os offsets são baseados em 0, então, um registrador
configurado como 100 na biblioteca será configurado como 100 no ScadaBR. Por outro
lado, no software de teste CAS Modbus Scanner (http://www.chipkin.com/products/software/modbus-software/cas-modbus-scanner/)
os offsets são baseados em 1, logo, um registrador configurado como 100 na biblioteca
deverá ser 101 neste software.

3) No início do arquivo Modbus.h da biblioteca há uma opção para limitar o funcionamento
da mesma às funções de Holding Registers, salvando espaço na memória de programa.
Basta retirar o comentário da seguinte linha:

```
#define USE_HOLDING_REGISTERS_ONLY
```
Dessa forma, somente as seguintes funções são suportadas:
<ul>
    <li>0x03 - Read Holding Registers</li>
    <li>0x06 - Write Single Register</li>
    <li>0x10 - Write Multiple Registers</li>
</ul>

4) Com o uso da ModbusSerial é possível escolher entre Hardware Serial (padrão) ou Software Serial. Caso
você opte por Software Serial edite o arquivo ModbusSerial.h e retire o comentário da seguinte linha:

```
#define USE_SOFTWARE_SERIAL
```

Você poderá agora construir seu programa principal colocando os includes necessários:
```
#include <Modbus.h>
#include <ModbusSerial.h>
#include <SoftwareSerial.h>
```

E utilizando no setup():

```
SoftwareSerial myserial(2,3);
mb.config(&myserial, 38400);   // mb.config(mb.config(&myserial, 38400, 4) para RS-485
```

Como utilizar
=============

Existem cinco classes que correspondem aos cinco cabeçalhos que podem ser utilizados:
<ul>
    <li>Modbus - Biblioteca Base</li>
    <li>ModbusSerial - Biblioteca Modbus Serial (RS-232 e RS-485)</li>
    <li>ModbusIP - Biblioteca Modbus IP (Shield Ethernet padrão)</li>
    <li>ModbusIP_ENC28J60 - Biblioteca Modbus IP (para o chip ENC28J60)</li>
    <li>ModbusIP_ESP8266AT - Biblioteca Modbus IP (Para o chip ESP8266 com firmware AT) </li>
</ul>
<h4>Se você quer utilizar Modbus no ESP8266 sem o Arduino, tenho novidades:
http://www.github.com/andresarmento/modbus-esp8266</h4>
Ao optar por Modbus Serial ou Modbus IP você deve incluir o cabeçalho correspondente e o cabeçalho da biblioteca base em seu sketch, Ex:
```
#include <Modbus.h>
#include <ModbusSerial.h>
```

<b>Jargão do Modbus</b>

Optou-se por utilizar os termos usados no Modbus para os métodos da biblioteca, assim, convém esclarecer
os nomes dos tipos de registradores:

| Tipo de registrador  | Uso                | Acesso            | Métodos da biblioteca |
| -------------------- | ------------------ | ----------------- | --------------------- |
| Coil                 | Sáida digital      | Leitura/Escrita   | addCoil(), Coil()     |
| Holding Register     | Saída analógica    | Leitura/Escrita   | addHreg(), Hreg()     |
| Input Status         | Entrada digital    | Somente Leitura   | addIsts(), Ists()     |
| Input Register       | Entrada analógica  | Somente Leitura   | addIreg(), Ireg()     |

<b>Observações:</b>

1. <i>Input Status</i> é às vezes chamada de <i>Discrete Input</i>.
2. <i>Holding Register</i> ou apenas <i>Register</i> também é utilizado para armazenar valores no escravo.
3. Exemplos de uso: Um <i>Coil</i> pode ser utilizado para acionar uma lâmpada ou led. Um <i>Holding Register</i> para
armazenar um contador ou acionar um Servo. Um <i>Input Status</i> pode ser utilizado com um reed-switch
em um sensor de porta e um <i>Input Register</i> com um sensor de temperatura.


<h3>ModBus Serial</h3>

Há quatro exemplos que podem ser acessados da interface do Arduino, uma vez que você tenha instalado
a biblioteca. Vejamos o exemplo Lamp.ino (apenas as partes relativas ao modbus serão comentadas):

```
#include <Modbus.h>
#include <ModbusSerial.h>
```
Inclusão das bibliotecas necessárias.


```
const int LAMP1_COIL = 100;
```
Define o registrador Modbus para representar lâmpada ou led. Este valor é o offset
(baseado em 0) a ser colocado no seu sistema supervisório ou software de teste. Observe
que se seu software utiliza offsets baseados em 1 o valor configurado
lá deverá ser 101, para este exemplo.


```
ModbusSerial mb;
```
Cria a instância mb (ModbusSerial) a ser utilizada.


```
mb.config(&Serial, 38400, SERIAL_8N1);
mb.setSlaveId(10);
```
Configura a porta serial e o Id do escravo. Observe a passagem da porta Serial como
referência, o que permite o uso de outras seriais em outros modelos de Arduino.
A taxa e formato padrão 8N1 estão sendo configurados.
Caso se esteja utilizando RS-485 é necessário a configuração de outro pino para
controle de transmissão/recepção. Isto é feito da seguinte forma:

```
mb.config(&Serial, 38400, SERIAL_8N1, 2);
```
Nesse caso, o pino 2 será utilizado para controle de TX/RX.


```
mb.addCoil(LAMP1_COIL);
```
Adiciona o registrador do tipo Coil (saída digital) que será responsável por
acionar a led ou lâmpada e verificar seu estado.
A biblioteca permite configurar um valor inicial para o registrador:

```
mb.addCoil(LAMP1_COIL, true);
```
Nesse caso o registrador é adicionado e configurado como true. Caso se utilize
o primeira forma o valor default é false.


```
mb.task();
```
Este método faz toda a mágica, respondendo as requisições e alterando os registradores
se necessário, ele deve ser chamado apenas uma vez, no início no loop.


```
digitalWrite(ledPin, mb.Coil(LAMP1_COIL));
```
Por fim o valor do registrador LAMP1_COIL é utilizado para acionar a lâmpada ou led.


De forma bastante similar os outros exemplos mostram o uso dos outros métodos
disponíveis na biblioteca:
```
void addCoil(word offset, bool value)
void addHreg(word offset, word value)
void addIsts(word offset, bool value)
void addIreg(word offset, word value)
```
Adiciona registradores e configura valor inicial se especificado.


```
bool Coil(word offset, bool value)
bool Hreg(word offset, word value)
bool Ists(word offset, bool value)
bool Ireg(word offset, word value)
```
Configura um valor para o registrador.

```
bool Coil(word offset)
word Hreg(word offset)
bool Ists(word offset)
word Ireg(word offset)
```
Retorna o valor de um registrador.


<h3>Modbus IP</h3>

Há quatro exemplos que podem ser acessados da interface do Arduino, uma vez que você tenha instalado
a biblioteca. Vejamos o exemplo Switch.ino (apenas as partes relativas ao modbus serão comentadas):

```
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
```
Inclusão das bibliotecas necessárias.


```
const int SWITCH_ISTS = 100;
const int switchPin = 3;
```
Define o registrador Modbus para representar o interruptor. Este valor é o offset
(baseado em 0) a ser colocado no seu sistema supervisório ou software de teste. Observe
que se seu software utiliza offsets baseados em 1 o valor configurado
lá deverá ser 101, para este exemplo.


```
ModbusIP mb;
```
Cria a instância mb (ModbusIP) a ser utilizada.


```
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 120 };
mb.config(mac, ip);
```
Configura a shield Ethernet. Os valores do endereço MAC e o IP são passados
pelo método config(). A sintaxe é igual a da classe Ethernet do arduino,
e suporta os seguintes formatos:
```
void config(uint8_t *mac)
void config(uint8_t *mac, IPAddress ip)
void config(uint8_t *mac, IPAddress ip, IPAddress dns)
void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway)
void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet)
```


Em seguida temos:
```
mb.addIsts(SWITCH_ISTS);
```
Adiciona o registrador do tipo Input Status (entrada digital) que será responsável por
detectar se um interruptor está ou não acionado.
A biblioteca permite configurar um valor inicial para o registrador:

```
mb.addIsts(SWITCH_ISTS, true);
```

Nesse caso o registrador é adicionado e configurado como true. Caso se utilize
o primeira forma o valor default é false.


```
mb.task();
```
Este método faz toda a mágica, respondendo as requisições e alterando os registradores
se necessário, ele deve ser chamado apenas uma vez, no início no loop.


```
mb.Ists(SWITCH_ISTS, digitalRead(switchPin));
```
Por fim o valor do registrador SWITCH_ISTS é alterado conforme o estado da entrada
digital escolhida.


<h3>Modbus IP (ENC28J60)</h3>

A shield Ethernet padrão utilizada no Arduino é baseada no chip da Wiznet W5100, portanto, a IDE já vem
com a biblioteca instalada. Caso você tenha uma shield baseada no ENC28J60 da Microchip você deverá instalar
outra biblioteca Ethernet. Dentre várias disponíveis optamos pela EtherCard.

Faça o download da EtherCard em https://github.com/jcw/ethercard e a instale na sua IDE.
Utilize os seguintes includes nos seus sketches:

```
#include <EtherCard.h>
#include <ModbusIP_ENC28J60.h>
#include <Modbus.h>
```
Pronto! O uso das funções Modbus é idêntico a da biblioteca ModbusIP descrita anteriormente.

<b>Observações:</b>

1. A Ethercard está configurada para utilizar os pinos 10, 11 , 12 e 13.
2. A alimentação das shields baseadas no ENC28J60 é geralmente 3.3V.

3. Outra alternativa para o ENC28J60 é utilizar a biblioteca UIPEthernet, disponível em
https://github.com/ntruchsess/arduino_uip. Essa biblioteca foi feita de tal forma que
imita as mesmas funções da biblioteca Ethernet padrão, cujo trabalho é feito pelo chip
da Wiznet W5100. Como o ENC28J60 não possui todas as funções do outro chip, a biblioteca
UIPEthernet é muito maior, pois tem que fazer em software o que na shield Wiznet é feito
em hardware. Se você precisar por algum motivo usar esta biblioteca basta alterar
o arquivo ModbusIP.h e seus sketches, trocando as linhas:

```
#include <Ethernet.h>
```

por

```
#include <UIPEthernet.h>
```

Assim, você poderá utilizar a biblioteca ModbusIP (Não a ModbusIP_ENC28J60).
Na realidade isso permite que qualquer biblioteca ou skecth feito para a
shield da Wiznet seja usado na shield ENC28J60. O grande problema com essa solução
(e por isso optamos pela EtherCard) é que a biblioteca UIPEthernet + ModbusIP ocupa cerca de 60%
da memória de programa no Arduino Uno, enquanto que com a EtherCard + ModbusIP_ENC28J60 esse
valor cai para 30%!


<h3>Modbus IP (ESP8266 AT)</h3>

Os módulos baseados no ESP8266 fazem bastante sucesso e são baratos. Com um firmware que
responde a comandos AT (padrão em muitos módulos) é possível utilizá-los como uma
simples interface de rede sem fio para o Arduino.

O firmware utilizado no módulo foi o at_v0.20_on_SDKv0.9.3 disponível em:
http://www.electrodragon.com/w/ESP8266_AT-command_firmware

(Outros firmwares AT compatíveis com a biblioteca WeeESP8266 da ITEAD devem funcionar)

Atenção: Firmwares tais como o NodeMCU e MicroPython não funcionarão pois as bibliotecas
utilizadas aqui dependem de um firmware que responda a comandos AT via interface serial.
Os firmwares mencionados são utilizados quando se deseja utilizar os módulos ESP8266
sem o arduino.

Você precisará da biblioteca WeeESP8266 (ITEAD) para o Arduino. Faça o download em:

https://github.com/itead/ITEADLIB_Arduino_WeeESP8266 e a instale na sua IDE.

<b>Observações:</b>

1. A biblioteca ESP8266 pode ser utilizada com interface serial por hardware (HardwareSerial) ou
por software (SoftwareSerial). Por default ela utilizará HardwareSerial, para alterar edite o arquivo
ESP8266.h removendo o comentário da linha:

```
#define ESP8266_USE_SOFTWARE_SERIAL
```

2. Lembre-se que a alimentação do módulos ESP8266 é 3.3V.


Para a Modbus IP (ESP8266 AT) há quatro exemplos que podem ser acessados da interface do Arduino.
Vejamos o exemplo Lamp.ino (apenas as partes relativas ao modbus serão comentadas):


```
#include <ESP8266.h>
#include <SoftwareSerial.h>   //Apenas se utilizar Softwareserial para se comunicar com o módulo
#include <Modbus.h>
#include <ModbusIP_ESP8266AT.h>
```
Inclusão das bibliotecas necessárias.


```
SoftwareSerial wifiSerial(2 , 3);
```
Cria a interface serial via Software utilizando os pinos 2(RX) e 3(TX). Assim é possível utilizar
a serial por hardware para comunicação com o PC (para debug por exemplo) em modelos de Arduino que
possuem somente uma serial (Ex.: Arduino UNO).


```
ESP8266 wifi(wifiSerial, 9600);
```
Cria o objeto wifi (ESP8266) especificando a taxa em bps.
Atenção: Se utilizar Softwareserial não especifique taxas de 115200bps ou mais pois a serial por
software não irá funcionar. Alguns firmwares/módulos vem com 115200bps por default. Você
terá que alterar o módulo via comando AT:
```
AT+CIOBAUD=9600
```

Continuando o exemplo:
```
const int LAMP1_COIL = 100;
```
Define o registrador Modbus para representar lâmpada ou led. Este valor é o offset
(baseado em 0) a ser colocado no seu sistema supervisório ou software de teste. Observe
que se seu software utiliza offsets baseados em 1 o valor configurado
lá deverá ser 101, para este exemplo.


```
ModbusIP mb;
```
Cria a instância mb (ModbusIP) a ser utilizada.


```
mb.config(wifi, "your_ssid", "your_password");
```
Configura o módulo ESP8266. Os valores entre aspas correspodem ao nome da rede (SSID) e a chave
de sergurança.

Por default a configuração IP será recebida via DHCP. Veja ao final as opções para ter um IP
estático (importante para que não seja necessário alterar no mestre/supervisório caso o IP mude).


Em seguida temos:
```
mb.addCoil(LAMP1_COIL);
```
Adiciona o registrador do tipo Coil (saída digital) que será responsável por
acionar a led ou lâmpada e verificar seu estado.
A biblioteca permite configurar um valor inicial para o registrador:

```
mb.addCoil(LAMP1_COIL, true);
```
Nesse caso o registrador é adicionado e configurado como true. Caso se utilize
o primeira forma o valor default é false.


```
mb.task();
```
Este método faz toda a mágica, respondendo as requisições e alterando os registradores
se necessário, ele deve ser chamado apenas uma vez, no início no loop.


```
digitalWrite(ledPin, mb.Coil(LAMP1_COIL));
```
Por fim o valor do registrador LAMP1_COIL é utilizado para acionar a lâmpada ou led.

De forma bastante similar os outros exemplos mostram o uso dos outros métodos
disponíveis na biblioteca.


<b>Como utilizar um IP estático com o módulo ESP8266</b>

Temos conhecimento atualmente de duas opções:

1) No seu roteador configure o MAC Address do módulo para que o endereço IP fornecido por
DHCP seja sempre o mesmo (A maior parte dos roteadores possuem esta funcionalidade).

2) No seu código, inclua duas linhas para alterar o endereço IP após a configuração do módulo:

```
mb.config(wifi, "your_ssid", "your_password");
delay(1000);
wifiSerial.println("AT+CIPSTA=\"192.168.1.44\"");
```

Obs.: Para que o módulo volte a receber IP via DHCP será necessário remover as linhas
e executar (pelo menos uma vez) o comando: AT+CWDHCP=1,1 via conexão direta com o
módulo, ou:

```
wifiSerial.println("AT+CWDHCP=1,1");
```

Outras bibliotecas Modbus
=========================

<b>Arduino Modbus RTU</b><br>
Autor   : Juan Pablo Zometa, Samuel Marco and Andras Tucsni<br>
Ano     : 2010<br>
Website : https://sites.google.com/site/jpmzometa/


<b>Simple Modbus</b><br>
Autor   : Bester.J<br>
Ano     : 2013</br>
Website : https://code.google.com/p/simple-modbus/


<b>Arduino-Modbus-Slave</b><br>
Autor   : Jason Vreeland [CodeRage]<br>
Ano     : 2010<br>
Website : http://code.google.com/p/arduino-modbus-slave/


<b>Mudbus (Modbus TCP)</b><br>
Autor   : Dee Wykoff<br>
Ano     : 2011<br>
Website : http://code.google.com/p/mudbus/


<b>ModbusMaster Library for Arduino</b><br>
Autor   : Doc Walker<br>
Ano     : 2012<br>
Website : https://github.com/4-20ma/ModbusMaster<br>
Website : http://playground.arduino.cc/Code/ModbusMaster


Contribuições
=============
http://github.com/andresarmento/modbus-arduino<br>
prof (at) andresarmento (dot) com

Licença
=======

O código neste repositório é licenciado pela BSD New License.
Veja [LICENSE.txt](LICENSE.txt) para mais informações.


