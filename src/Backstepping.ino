/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controlador Backstepping                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 11:28:59 by anonymous         #+#    #+#             */
/*   Updated: 2018/11/10 11:28:11 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define INPUT_IL1 0
#define INPUT_Ipv 2
#define INPUT_Vpv 4
#define INPUT_Vout 15

double IL1,Ipv,Vpv,Vout; //VALORES DOS SENSORES
double 	Vpvref,
		time_previous,
		now,
		delta_T, 
		Vpvref_previous = 0,time_previous_Vpvref = 0,
		Ipvo = 0,
		Vpvo = 0;

//SLEW RATE
double 	Vpvref_SLEWRATE;
double 	Vpvref_SLEWRATE_FINAL = 0 , Vpvref_SLEWRATE_FINAL_AUXILIAR = 0;
const int Vpvref_SLEWRATE_MAX = 550;// MODIFICAR DE ACORDO COM OS TESTES 
const int Vpvref_SLEWRATE_MIN = 510;// MODIFICAR DE ACORDO COM OS TESTES 



double derivada1_SR_aux = 0;
double d1_SR_out = 0;

void setup(){
	Serial.begin(9600);
	time_previous = millis();
	time_previous_Vpvref = millis();
}

void loop() {
	//ENTRADAS	
	Vpv  = double(map(analogRead(INPUT_Vpv), 0, 4095, 0, 1023));//TENSÃO NA PLACA SOLAR 
	Ipv  = double(map(analogRead(INPUT_Ipv), 0, 4095, 0, 1023));//VALOR DA CORRENTE NO PAINEL SOLAR
	IL1  = double(map(analogRead(INPUT_IL1), 0, 4095, 0, 1023));//VALOR DA CORRENTE NO INDUTOR
	Vout = double(map(analogRead(INPUT_Vout), 0, 4095, 0, 1023));//TENSÃO DE SAIDA DO CIRCUITO

	//DERIVADA	
	Vpvref=(-Ipv*(Vpv-Vpvo))/((Ipv-Ipvo)+0.001);  //Equação da corrente de referência
	
	//SLEWRATE
	now = millis();
	delta_T = now - time_previous;
	Vpvref_SLEWRATE = (Vpvref - Vpvref_previous)/delta_T;

	const double SR_ST = 0.0002; //TESTAR PARA REFICAR VALOR

	if(Vpvref_SLEWRATE>Vpvref_SLEWRATE_MAX){		
		Vpvref_SLEWRATE_FINAL=(Vpvref_SLEWRATE_MAX*SR_ST)-(Vpvref_SLEWRATE_FINAL_AUXILIAR);		
	}

	if(Vpvref_SLEWRATE<Vpvref_SLEWRATE_MIN){		
		Vpvref_SLEWRATE_FINAL=(Vpvref_SLEWRATE_MIN*SR_ST)-(Vpvref_SLEWRATE_FINAL_AUXILIAR);		
	}

	Vpvref_SLEWRATE_FINAL_AUXILIAR=Vpvref_SLEWRATE_FINAL;

	const double Vcapv = 633.1;
	if(Vpvref_SLEWRATE_FINAL>Vcapv)Vpvref=Vcapv;  
	if(Vpvref_SLEWRATE_FINAL<0) Vpvref=0; 

	////////////////////////////////////////////////////ILV (Equação 28)///////////////////////////////////////////////

	//////1.1) Slew Rate e Saturação de d1_ Vpvref
	double d1_SR_R=200;
	double d1_SR_F=100;

	//////1) Cálculo da 1º derivada de Vpvref
	double delta_time_ST  = millis()-time_previous_Vpvref;
	double derivada1_Vpvref = (Vpvref-Vpvref_previous)/(delta_time_ST);	

	double derivada1_SR=(derivada1_Vpvref-derivada1_SR_aux)/(delta_time_ST);


	if(derivada1_SR>d1_SR_R){	
		d1_SR_out=((d1_SR_R)*(delta_time_ST))+(derivada1_SR_aux);	
	}

	if(derivada1_SR<d1_SR_F){	
		d1_SR_out=(d1_SR_F*delta_time_ST)-(derivada1_SR_aux);
	}

	derivada1_SR_aux=d1_SR_out;

	if(derivada1_Vpvref>550)derivada1_Vpvref=550;  
	if(derivada1_Vpvref<520) derivada1_Vpvref=520; 

	double Cin = 0.000222;
	double aux4=derivada1_Vpvref*Cin;

	////////2) Cálculo de eVpv-----(OBS Kv=0.05/Cin) mas como eVpv será mult. por Cin esse termo é cancelado

	Serial.print("Vpv: ");
	Serial.println(Vpv,4);
	Serial.print("Ipv: ");
	Serial.println(Ipv,4);
	Serial.print("IL1: ");
	Serial.println(IL1,4);
	Serial.print("Vout: ");
	Serial.println(Vout,4);
	Serial.print("Vpvref: ");
	Serial.println(Vpvref,4);
	Serial.print("Ipvo: ");
	Serial.println(Ipvo,4);
	Serial.print("Vpvo: ");
	Serial.println(Vpvo,4);
	Serial.print("delta_T: ");
	Serial.println(delta_T, 8);
	Serial.print("Vpvref_SLEWRATE: ");
	Serial.println(Vpvref_SLEWRATE, 8);
	Serial.println("______________________");


	Ipvo=Ipv;
	Vpvo=Vpv;
	time_previous = now;
	Vpvref_previous = Vpvref;

	delay(100);
}

/*
	GPIO 0 ==> ADC2_CH1
	GPIO 2 ==> ADC2_CH2
	GPIO 4 ==> ADC2_CH0
	GPIO 12 => ADC2_CH5
	GPIO 13 => ADC2_CH4
	GPIO 14 => ADC2_CH6
	GPIO 15 => ADC2_CH3
	GPIO 25 => ADC2_CH8
	GPIO 26 => ADC2_CH9
	GPIO 27 => ADC2_CH7
	GPIO 32 => ADC1_CH4
	GPIO 33 => ADC1_CH5
	GPIO 34 => ADC1_CH6
	GPIO 35 => ADC1_CH7
	GPIO 36 => ADC1_CH0
	GPIO 37 => ADC1_CH1
	GPIO 38 => ADC1_CH2
	GPIO 39 => ADC1_CH3
*/