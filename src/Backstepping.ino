/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controlador Backsteepin                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 11:28:59 by anonymous         #+#    #+#             */
/*   Updated: 2018/10/26 11:28:11 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//ENTRADA
double Ipv,ILin,Vpv,Vcout,Ipvo,Vpvo;
double Vpvo = 0 ;

//SAIDA
double out_0;

void setup() {
	/////////////////////////////////////////////////   Cálculo Vpvref  /////////////////////////////////////////////
	//ENTRADAS
	Ipv = (double)in[0];//VALOR DA CORRENTE NO PAINEL SOLAR 1
	ILin = (double)in[1];//VALOR DA CORRENTE NO PAINEL SOLAR 2 (CORRENTE DE ENTRADA)
	Vpv = (double)in[2];//TENSÃO NA PLACA SOLAR 
	Vcout = (double)in[3];//TENSÃO DE SAIDA DO CIRCUITO

	//DERIVADA	
	Vpvref=-Ipv*(Vpv-Vpvo)/(Ipv-Ipvo+0.001);  //Equação da corrente de referência
	
	Ipvo=Ipv;
	Vpvo=Vpv;
}

void loop() {
  
}