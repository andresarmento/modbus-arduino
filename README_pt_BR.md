Biblioteca Modbus para Arduino
==============================

Esta biblioteca permite que seu arduino se comunique através do protocolo Modbus.
O Mosbus é um protocolo do tipo mestre-escravo, utilizado em automação industrial,
podendo ser utilizado em outras áreas, como por exemplo, na automação residencial.

O Modbus geralmente utiliza como meio físico as interfaces seriais RS-232 ou RS-485
(quando é chamado Modbus Serial) ou TCP/IP via Ethernet ou Wi-fi (Modbus IP).

Na versão atual a biblioteca permite que o arduino opere como escravo, suportando
tando Modbus Serial quanto Modbus IP.

Para mais informações sobre o Modbus consulte:

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
várias ideias para compor a modbus-arduino. No final do texto estão listadas as
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

1. Ao utilizar ModbusIP o protocolo de transporte é o TCP (porta 502) e a conexão
é finalizada a cada mensagem transmitida, ou seja, não é do tipo keep-alive.

2. Os offsets para acesso aos registradores são baseados em 0. Assim, tenha cuidado
ao configurar seu seu supervisório ou utilitário de teste. Por exempo, no ScadaBR
(http://www.scadabr.com.br) os offsets são baseados em 0, então, um registro
configurado como 100 na biblioteca será configurado como 100 no ScadaBR. Por outro
lado, no utilitário CAS Modbus Scanner (http://www.chipkin.com/products/software/modbus-software/cas-modbus-scanner/)
os offsets são baseados em 1, logo, um registro configurado como 100 na biblioteca
deverá ser 101 neste software.


Como utilizar
=============

Existem três classes que correspondem a três includes que podem ser utilizados:
<ul>
    <li>Modbus - Biblioteca Base</li>
    <li>Modbus Serial - Biblioteca Modbus Serial (RS-232 e RS-485)</li>
    <li>Modbus IP - Biblioteca Modbus IP</li>
</ul>

Ao optar por Serial ou IP deve-se incluir o cabeçalho correspondente e o cabeçalho da biblioteca base, Ex:
```
#include <Modbus.h>
#include <ModbusSerial.h>
```

<b>Jargão do protocolo Modbus</b>

Optou-se por utilizar os termos do Modbus para os métodos de acesso à biblioteca, assim, convém esclarecer
os tipos de registradores:

| Tipo de registrador  | Uso                | Acesso            | Métodos da biblioteca |
| -------------------- | ------------------ | ----------------- | ---------------------
| Coil                 | Sáida digital      | Leitura/Escrita   | addCoil(), Coil()     |
| Holding Register     | Saída analógica    | Leitura/Escrita   | addHReg(), HReg()     |
| Input Status         | Entrada digital    | Somente Leitura   | addIsts(), Ists()     |
| Input Register       | Entrada analógica  | Somente Leitura   | addIreg(), Ireg()     |

<b>Observações:</b>

1. <i>Input Status</i> também é chamada de <i>Discrete Input</i>.
2. <i>Holding Register</i> ou apenas <i>Register</i> também é utilizado para armazenar valores no escravo.

Um <i>Coil</i> pode ser utilizado para acionar uma lâmpada ou led.
Um <i>Holding Register</i> pode ser utilizado para armazenar um contador ou acionar um Servo.
Um <i>Input Status</i> pode ser utilizado com um reed-switch em um sensor de porta.
Um <i>Input Register</i> pode ser utilizado com um sensor de temperatura.


<h3>ModBus Serial</h3>

Há quatro exemplos que podem ser acessados da interface do Arduino, uma vez que você tenha instalado
a biblioteca. Vejamos um deles (Lamp.ino):

```
#include <Modbus.h>
#include <ModbusSerial.h>
```
Inclusão das bibliotecas necessárias.


```
const int LAMP1_COIL = 100;
```
Define o registrador Modbus para representar lâmpada ou led. Este valor é o offset
(baseado em 0) a ser colocado no seu supervisório ou utilitário de teste. Observe
que se seu supervisório ou utilitário utiliza offsets baseados em 1 o valor configurado
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
Este método faz toda a mágica, respondendo as requições e alterando os registradores
se necessário, ele deve ser chamado apenas uma vez, no início no loop.


```
digitalWrite(ledPin, mb.Coil(LAMP1_COIL));
```
Por fim o valor do regitrador LAMP1_COIL é utilizado para acionar a lâmpada ou led.


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


<h3>ModBus IP</h3>

Há quatro exemplos que podem ser acessados da interface do Arduino, uma vez que você tenha instalado
a biblioteca. Vejamos um deles (Switch.ino):

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
Define o registrador Modbus para representar o interrupor. Este valor é o offset
(baseado em 0) a ser colocado no seu supervisório ou utilitário de teste. Observe
que se seu supervisório ou utilitário utiliza offsets baseados em 1 o valor configurado
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
pelo método config(). A sintaxe é idêndica ao uso da classe Ethernet do arduino,
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
Este método faz toda a mágica, respondendo as requições e alterando os registradores
se necessário, ele deve ser chamado apenas uma vez, no início no loop.


```
mb.Ists(SWITCH_ISTS, digitalRead(switchPin));
```
Por fim o valor do regitrador SWITCH_ISTS é alterado conforme o estado da entrada
digital escolhida.


Outras bibliotecas Modbus
=========================

<b>Arduino Modbus RTU</b>
Autor   : Juan Pablo Zometa, Samuel Marco and Andras Tucsni
Ano     : 2010
Website : https://sites.google.com/site/jpmzometa/


<b>Simple Modbus</b>
Autor   : Bester.J
Ano     : 2013
Website : https://code.google.com/p/simple-modbus/


<b>Arduino-Modbus-Slave</b>
Autor   : Jason Vreeland [CodeRage]
Ano     : 2010
Website : http://code.google.com/p/arduino-modbus-slave/


<b>Mudbus (Modbus TCP)</b>
Autor   : Dee Wykoff
Ano     : 2011
Website : http://code.google.com/p/mudbus/


<b>ModbusMaster Library for Arduino</b>
Autor   : Doc Walker
Ano     : 2012
Website : https://github.com/4-20ma/ModbusMaster
Website : http://playground.arduino.cc/Code/ModbusMaster


Contribuições
=============
http://github.com/andresarmento/modbus-arduino


Licença
=======

The code in this repo is licensed under the BSD New License.
See [LICENSE.txt](LICENSE.txt) for more info.


