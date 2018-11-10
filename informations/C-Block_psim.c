/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C-Block_psim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 11:28:59 by anonymous         #+#    #+#             */
/*   Updated: 2018/11/10 16:23:21 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
//----------------------------------------Controlador Backsteeping-------------------------------------

/////////////////////////////////////////////////   Cálculo Vpvref  /////////////////////////////////////////////

Ipv= (double)in[0];
ILin=(double)in[1];
Vpv=(double)in[2];
Vcout=(double)in[3];
Vpulse=(double)in[9];
//out[1]=Ipv;
//out[2]=Ipvo;
//out[3]=Vpv;
//out[4]=Vpvo;
Vpvref=-Ipv*(Vpv-Vpvo)/(Ipv-Ipvo+0.001);  //Equação da corrente de referência
Ipvo=Ipv;
Vpvo=Vpv;

//out[6]=Vpvref;


////////////////////////////////////Slew Rate e Saturação de Vpvref////////////////////////////////////////////////////////////////////////
SR_R=550;
SR_F=510;

//out[1]=SR_R;
//out[2]=SR_F;

SR=(Vpvref-SR_out0)/(SR_ST);
//out[9]=SR;

if(SR>SR_R)
{

	
	SR_out=(SR_R*SR_ST)-(SR_out0);
aux3=1;
}

if(SR<SR_F)
{
	
	
	SR_out=(SR_F*SR_ST)-(SR_out0);
aux3=2;
}

SR_out0=SR_out;


//out[8]=SR_out;


//out[5]=aux3;


if(SR_out>Vcapv)Vpvref=Vcapv;  
if(SR_out<0) Vpvref=0; 




//out[1]=Vpvref; // valor ok 536


////////////////////////////////////////////////////ILV (Equação 28)///////////////////////////////////////////////

//////1) Cálculo da 1º derivada de Vpvref

d1_Vpvref = (Vpvref-Vpvref0)/(ST_d1);
Vpvref0=Vpvref;

//out[5]=d1_Vpvref;

//////1.1) Slew Rate e Saturação de d1_ Vpvref

d1_SR_R=200;
d1_SR_F=100;

//out[1]=d1_SR_R;
//out[2]=d1_SR_F;

d1_SR=(d1_Vpvref-d1_SR_out0)/(d1_SR_ST);
//out[9]=d1_SR;

if(d1_SR>d1_SR_R)
{

	
	d1_SR_out=((d1_SR_R)*(d1_SR_ST))+(d1_SR_out0);
//aux3=1;
}

if(d1_SR<d1_SR_F)
{
	
	
	d1_SR_out=(d1_SR_F*d1_SR_ST)-(d1_SR_out0);
//aux3=2;
}

d1_SR_out0=d1_SR_out;


//out[1]=d1_SR_out;


//out[6]=aux3;


//if(d1_SR_out>1)d1_Vpvref=1;  
//if(d1_SR_out<-1) d1_Vpvref=-1; 


if(d1_Vpvref>550)d1_Vpvref=550;  
if(d1_Vpvref<520) d1_Vpvref=520; 

//out[2]=d1_Vpvref;


aux4=d1_Vpvref*Cin;


//out[9]=aux4;

////////2) Cálculo de eVpv-----(OBS Kv=0.05/Cin) mas como eVpv será mult. por Cin esse termo é cancelado

Kv=(0.05);

eVpv=(Vpvref-Vpv);

//out[8]=eVpv;



/////////3) Cálculo de eI------(OBS KI = 0.01/Cin) idem a eVpv

KI=0.01;

Iterm_max1=664;
Iterm_min1=-664;

integ1=integ1_ant-KI*(((eVpv+eVpv_ant)*ST)/2);  //Área do trapézio
//out[4]=integ1;

if (integ1>Iterm_max1)
{integ1=Iterm_max1;}
if(integ1<Iterm_min1)
{integ1=Iterm_min1;}

//out[5]=integ1;

eVpv_ant=eVpv;
integ1_ant=integ1;


ILV=(-integ1-(eVpv*Kv)-aux4)+Ipv;


if(ILV>1.1*Iccpv)ILV=Iccpv;  
if(SR_out<0) Vpvref=0; 

//out[6]=ILV;

//////////////////////////////////Cálculo de eIL ////////////////////////////////////////////////////////////////////

KI_ILV=0.0001/Lin;


eIL=(ILV-ILin)*KI_ILV;

//out[7]=eIL;
///////////////////////////////////////Segunda derivada e termos adjacentes//////////////////////////////////////////////////////////

///////////////////1) Cálculo de eVpv/Cin

eVpv_1=eVpv/(Cin);

//out[2]=eVpv_1;

/////////////////2) Cálculo de eVpv*KI

eVpv_2=eVpv*KI;

//out[3]=eVpv_2;



////////////////3) Cálculo da 2º derivada de Vpvref


d2_Vpvref = (d1_Vpvref-d1_Vpvref0)/(ST_d2);

//out[4]=d2_Vpvref;

d1_Vpvref0=d1_Vpvref;

//////3.1) Slew Rate e Saturação de d2_ Vpvref

d2_SR_R=200;
d2_SR_F=100;

//out[1]=d2_SR_R;
//out[2]=d2_SR_F;

d2_SR=(d2_Vpvref-d2_SR_out0)/(d2_SR_ST);
//out[9]=d2_SR;

if(d2_SR>d2_SR_R)
{

	
	d2_SR_out=((d2_SR_R)*(d2_SR_ST))+(d2_SR_out0);
aux3=1;
}

if(d2_SR<d2_SR_F)
{
	
	
	d2_SR_out=(d2_SR_F*d2_SR_ST)-(d2_SR_out0);
aux3=2;
}

d2_SR_out0=d2_SR_out;


//out[8]=d2_SR_out;


//out[6]=aux3;


//if(d2_SR_out>1)d2_Vpvref=1;  
//if(d2_SR_out<-1) d2_Vpvref=-1; 

//if(d2_Vpvref>400)d2_Vpvref=400;  
//if(d2_Vpvref<-400) d2_Vpvref=-400; 

//out[7]=d2_Vpvref;

////////////////4) Cálculo eIL/Cin e epv*Kv

eIL_1=(ILV-ILin)/Cin;


eVpv_3=eVpv*Kv;


aux6=Kv*(-eVpv_3 -eIL_1 -integ1);

//out[1]=aux6;

aux7=(d2_Vpvref-eVpv_2-aux6)*Cin;

//out[2]=aux7;

///////////////////////////////////////Cálculo considerando Lin (Final do equacionamento)//////////////////////////////////////////////////////////


////////1) derivada Ipv

deriv_Ipv=Ipv;
deriv_Ipv0=Ipvo;



d1_Ipv=(deriv_Ipv-deriv_Ipv0)/(ST_d2);

deriv_Ipv0=deriv_Ipv;


//////1.1)  Saturação de d1_Ipv


//if(d1_Ipv>0.5)d1_Ipv=0.5;  
//if(d1_Ipv<-0.5) d1_Ipv=-0.5; 

//out[7]=d1_Ipv;


aux5=(-d1_Ipv-eIL+eVpv_1+aux7)*Lin;

if(aux5>3000)aux5=3000;  
if(aux5<0) aux5=0; 

out[5]=aux5;

//out[2]=eIL;
//out[3]=eVpv_1;
//out[4]=aux7;


//////////////////////////////////////(aux5+Vpv)/Vout//////////////////////////////////////////////////////////



aux8=(aux5+Vpv)/(Vcout+1);


out[1]=Vpv;


dc=90-aux8;



if(dc>200)dc=200;
if(dc<0)dc=0;



out[0]=dc;

