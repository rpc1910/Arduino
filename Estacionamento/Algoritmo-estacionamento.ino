/*********************************************************
**														**
**	Estacionamento controlado por Arduino				**
**	Projeto Integrado I - CIC 2º Semestre - 2013/1 		**
**	Professor: Rogério Costa							**
**	Integrantes: Anderson | Fernanda | Renan | Rodrigo	**
**														**
**********************************************************/

#include <Servo.h>
#include <LiquidCrystal.h>

//Define portas de comunicação Digital
const int ledVerde = 13;
const int ledVermelho = 12;
const int portaMotorEntrada = 9;
const int portaMotorSaida = 8;
const int luz = 7;	//Utilizado para o Display LCD

//Define portas de comunicação analógica
const int sensorEntrada = 0;
const int sensorSaida = 1;

//Define posições de abertura e fechamento do portão
const int aberto = 90;
const int fechado = 0;

//Define número de vagas totais do estacionamento
const int totalVagas = 5;

//Cria variáveis de controle dos motores
Servo motorEntrada;
Servo motorSaida;

LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

//Variáveis 
int preenchido = 0, 
	totalDia = 0, 
	leitorEntrada = 0, 
	leitorSaida = 0,
	entradaAnterior = 0,
	saidaAnterior = 0;

int entradaAberto = 0,
	saidaAberto = 0;


void setup() {
	//Inicia componentes
	pinMode(ledVerde, OUTPUT);
	pinMode(ledVermelho, OUTPUT);

	//LCD
	lcd.begin(16, 2);
	pinMode(luz, OUTPUT);
	digitalWrite(luz, HIGH);

	lcd.setCursor(0, 0);
	lcd.print("Arduino Park");
	lcd.setCursor(0, 1);
	lcd.print(totalVagas - preenchido);
	lcd.print(" vagas");

	motorEntrada.attach(portaMotorEntrada);
	motorSaida.attach(portaMotorSaida);
	
	//Define porta serial
	Serial.begin(9600);

	//Mantem portões fechados
	motorEntrada.write(fechado);
	motorSaida.write(fechado);

	if(preenchido < totalVagas) {
		digitalWrite(ledVerde, HIGH);
	}
	else {
		digitalWrite(ledVermelho, HIGH);
	}
}


void loop() {
	delay(1000);
	leitorEntrada = analogRead(sensorEntrada);
	leitorSaida = analogRead(sensorSaida);

	//Recupera a casa da centena para realizar comparação
	leitorEntrada = leitorEntrada / 100;
	leitorSaida = leitorSaida / 100;
	Serial.println(leitorEntrada);

	//Entrada de veiculos
	if(leitorEntrada >= 4) {
		if((leitorEntrada >= 4) && (entradaAnterior < 4) && (entradaAberto == 0) && (preenchido < totalVagas)) {
			preenchido++;
			totalDia++;
			entradaAberto = 1;
			delay(100);
			motorEntrada.write(aberto);
			delay(100);
			Serial.println("Entrada de 1 veiculo");

			lcd.setCursor(0, 1);
			lcd.print(totalVagas - preenchido);
			lcd.print(" vagas");
		}
	}
	else {
		if((leitorEntrada < 4) && (entradaAnterior >= 4) && (entradaAberto == 1)) {
			delay(3000);
			motorEntrada.write(fechado);
			delay(100);
			entradaAberto = 0;
		}
	}

	//Saída de veículos
	if(leitorSaida >= 4) {
		if((leitorSaida >= 4) && (saidaAnterior < 4) && (saidaAberto == 0) && (preenchido > 0)) {
			preenchido--;
			saidaAberto = 1;
			delay(100);
			motorSaida.write(aberto);
			delay(100);
			Serial.println("Saida de 1 veiculo");
			lcd.setCursor(0, 1);
			lcd.print(totalVagas - preenchido);
			lcd.print(" vagas");
		}
	}
	else {
		if((leitorSaida < 4) && (saidaAnterior >= 4) && (saidaAberto == 1)) {
			delay(3000);
			motorSaida.write(fechado);
			delay(100);
			saidaAberto = 0;
		}
	}

	//Verifica leds
	if(preenchido < totalVagas) {
		digitalWrite(ledVerde, HIGH);
		digitalWrite(ledVermelho, LOW);
	}
	else {
		digitalWrite(ledVermelho, HIGH);
		digitalWrite(ledVerde, LOW);
	}

	//Armazena dados da leitura Atual
	delay(100);
	entradaAnterior = leitorEntrada;
	saidaAnterior = leitorSaida;

}