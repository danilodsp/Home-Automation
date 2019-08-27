//****************************************************************************//
//  MSP430 state machine
//  MdE.h
//  Author: Danilo Pena
//
//  Describtion:
//    A simple state machine for the MSP430
//
//  Generated by hand :)
//  Date:  05/06/2012        Time:  17:45
//
//****************************************************************************//

#ifndef SM_H
#define SM_H

#include <stdint.h>
#include <stdio.h>

#define NO_EVENTOS 28
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_INICIO                                                        1
#define PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_INICIO                                   2
#define PASSANDO_PARA_ENVIAR_MEDICAO_DO_INICIO                                                            3
#define PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_INICIO                                                    4
#define PASSANDO_PARA_ERROR_DO_INICIO                                                                     5
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS                                            6
#define PASSANDO_PARA_AGUARDAR_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS                                   7
#define PASSANDO_PARA_ERROR_DO_CONFIGURAR_ADDRESS                                                         8
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_CONFIGURAR_ADDRESS                                   9
#define PASSANDO_PARA_CONFIGURAR_IS_DO_AGUARDAR_CONFIGURAR_ADDRESS                                       10
#define PASSANDO_PARA_ERROR_DO_AGUARDAR_CONFIGURAR_ADDRESS                                               11
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_IS                                                12
#define PASSANDO_PARA_CONFIGURAR_IS_DO_CONFIGURAR_IS                                                     13
#define PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS                           14
#define PASSANDO_PARA_ENVIAR_MEDICAO_DO_CONFIGURAR_IS                                                    15
#define PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS                                            16
#define PASSANDO_PARA_ERROR_DO_CONFIGURAR_IS                                                             17
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO                                               18 
#define PASSANDO_PARA_ENVIAR_MEDICAO_DO_ENVIAR_MEDICAO                                                   19
#define PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO                                                            20
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_COMANDO_ATUAR                                       21
#define PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_AGUARDAR_COMANDO_ATUAR                                   22
#define PASSANDO_PARA_ERROR_DO_AGUARDAR_COMANDO_ATUAR                                                    23
#define PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR                      24
#define PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR 25
#define PASSANDO_PARA_ERROR_DO_ERROR                                                                     26
#define PASSANDO_PARA_INICIO_D_QQ_ESTADO 	                                                         27
#define PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR                                   28   // Evento esquecido

#define NO_ESTADOS 8
#define INICIO 0
#define CONFIGURAR_ADDRESS 1
#define AGUARDAR_CONFIGURAR_ADDRESS 2
#define CONFIGURAR_IS 3
#define ENVIAR_MEDICAO 4
#define AGUARDAR_COMANDO_ATUAR 5
#define ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR 6
#define ERROR 7




//****************************************************************************//
// COMANDOS IMPLEMENTADOS NA PARTE OPERATIVA (PO.c)
//****************************************************************************//

/*1
#########################################################################
# Objetivo: Ler dados na memória FLASH.                                 #
# Passos:                                                               #
# 1- Verificar se o tipo sensor ou atuador são iguais a ZERO,           #
# 2- Leitura da FLASH,                                                  #
# 3- Verificar se há ID do sensor/atuador,                              #
# 4- Verificar se há configuração do Sensor.                            #
# 5- Verificar se há configuração do Atuador.                           #
 */
void PO_LerMem(void);
/*
#########################################################################
 */




/*2
#################################################################################
# Objetivo: Emitir o comando ConfiguraAddress para o Servidor (Coordenador).    #
# Passos:                                                                       #
# 1- Enviar comando 0x01 (configurarAddress), com o tipoSensor e tipoAtuador    #
# 2- Verificar status de envio (0x8B)                                           #
 */
void PO_ConfigurarAddress(void);
/*
#################################################################################
 */


/*3
#################################################################################
# Objetivo: Registrar o IDsensour e/ou IDatuador.                               #
# Passos:                                                                       #
# 1- Zera Flag de checksum                                                      #
# 2- Recebe resposta do comando 0x01 (configuraAddress)                         #
# 3- Verificar o checksum                                                       #
# 4- Verificar se o tipoSensor e tipoAtuador receberam Ids correspondentes      #
# 5- Armazena IDs recebidos                                                     #
 */
void PO_ArmazenandoID(void);
/*
#################################################################################
 */

/*4
#################################################################################
# Objetivo: Configurar os parametros de envio e atuação.                        #
# Passos:                                                                       #
# 1- Zera Flag de checksum                                                      #                                      
# 2- Recebe o comando 0x02 (configurarSensor) ou 0x04 (configurarAtuador)       #
# 3- Verificar Checksum                                                         #
# 4- Armazena os parametros.                                                    #
# 5- Escrever na FLASH.                                                         #
 */
void PO_ConfiguraIS(void);
/*
#################################################################################
 */

/*5
#################################################################################
# Objetivo: Enviar Medição da entrada do ADC.                                   #
# Passos:                                                                       #
1- Enviar comando 0x03 (medicao)                                                #
2- Verificar status de envio (0x8B)                                             #
3- Zera Flag de checksum                                                        #
4- Recebe resposta do comadno 0x03                                              #
4.1- TIME_OUT da resposta                                                       #
5- Verificar Checksum                                                           #
6- Verifica se o foi sucesso, falha, ou medição de um sensor Não cadastrado     #
 */
void PO_EnviarMedicao(void);
/*
#################################################################################
 */

/*6
#################################################################################
# Objetivo: Aciona/Desaciona o Atuador ou o coloca em um estado especifico.     #
# Passos:                                                                       #
# 1- Zera Flag de checksum                                                      #
# 2- Recebe o comando 0x05 (Atuar)                                              #
# 3- Verificar Checksum                                                         #
# 4- Verifica o Idatuador se é o mesmo que o cadastrado.                        #
# 5- Executa a Ação.                                                            #
# 6- Enviar resposta positiva de atuação                                        #
# 7- Verificar status de envio (0x8B)                                           #
#################################################################################
 */
void PO_Atuar(void);
/*
#################################################################################
 */

/*7
#################################################################################
# Objetivo: Aciona o atuador, enquanto, enviar Medição da entrada do ADC,       #
# Passos:                                                                       #
# 1- Zera Flag de checksum                                                      #
# 2- Recebe o comando 0x05 (Atuar)                                              #
# 3- Verificar Checksum                                                         #
# 4- Verifica o Idatuador se é o mesmo que o cadastrado.                        #
# 5- Executa a Ação.                                                            #
# 6- Verifica se o tempo de enviar medicao expirou                              #
# 6.1- Enviar comando 0x03 (medicao)                                            #
# 6.2- Verificar status de envio (0x8B)                                         #
# 7- Zera Flag de checksum                                                      #
# 8- Recebe resposta do comadno 0x03                                            #
# 8.1- TIME_OUT da resposta                                                     #
# 9- Verificar Checksum                                                         #
# 10- Verifica se o foi sucesso, falha, ou medição de um sensor Não cadastrado  #
 */
void PO_EnviarMedicaoComAtuacao(void);
/*
#################################################################################
 */

/*8
#################################################################################
# Objetivo: Retorna um interio cujo valor corresponde ao erro descrito abaixo.  #
# Valores Possíveis do Error                                                    #
# (1) tipoSensor e tipoAtuador não cadastrados                                  #
# (2) Recebeu Ids zerados                                                       #
# (3) É um Sensor e um Atuador, mas recebeu apenas um ID.                       #
# (4) Recebeu um Idsensor sendo um Atuador;                                     #
# (5) Recebeu um IdAtuador sendo um Sensor;                                     #
# (6) Se é só sensor e recebeu uma configuração do Atuador                      #
# (7) Se é só atuador e recebeu uma configuração do Sensor                      #
# (8) Erro no atuador físico                                                    #
# (9) Erro no envio da resposta do comando atuar                                #
 */
void PO_Error(void);
/*
#################################################################################
#################################################################################
#################################################################################
 */

//****************************************************************************//
// Ações de Transição (MdE.c)
// Objetivo: Verificar o estado do sinal e alterar o estado Atual da Máquina.
//****************************************************************************//
/*1
#################################################################################
#################################################################################
#################################################################################
# Objetivo: Executa os configs e dar inicio a maquina de estado                 #
#                                                                               #
# sinal      -> 1                                                               #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 2                                                               #
# ProxEstado -> ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR                         #
#                                                                               #                                                  
# sinal      -> 3                                                               #
# ProxEstado -> ENVIAR_MEDICAO                                                  #
#                                                                               #
# sinal      -> 4                                                               #
# ProxEstado -> AGUARDAR_COMANDO_ATUAR                                          #
#                                                                               #  
# sinal      -> 5 ou 0                                                          #
# ProxEstado -> ERROR                                                           #
 */
void MdE_Comecar();

/*2
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 6                                                               #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 7                                                               #
# ProxEstado -> AGUARDAR_CONFIGURAR_ADDRESS                                     #
#                                                                               #
# sinal      -> 8 e (OUTROS)                                                    #
# ProxEstado -> ERROR                                                           #
#                                                                               #
 */
void MdE_ConfigurandoAddress();

/*3
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 9                                                               #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 10                                                              #
# ProxEstado -> CONFIGURAR_IS                                                   #
#                                                                               #                                                  
# sinal      -> 11 e OUTROS                                                     #
# ProxEstado -> ERROR                                                           #
 */
void MdE_ArmazenandoID();

/*4
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 12                                                              #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 13                                                              #
# ProxEstado -> CONFIGURAR_IS                                                   #
#                                                                               #                                                  
# sinal      -> 14                                                              #
# ProxEstado -> ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR                         #
#                                                                               #
# sinal      -> 15                                                              #
# ProxEstado -> ENVIAR_MEDICAO                                                  #
#                                                                               #  
# sinal      -> 16                                                              #
# ProxEstado -> AGUARDAR_COMANDO_ATUAR                                          #
#                                                                               #
# sinal      -> 17                                                              #
# ProxEstado -> ERROR                                                           #
 */
void MdE_ConfigurandoIS();

/*5
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 18                                                              #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 19                                                              #
# ProxEstado -> ENVIAR_MEDICAO                                                  #
#                                                                               #                                                  
# sinal      -> 20                                                              #
# ProxEstado -> ERROR                                                           #
 */
void MdE_EnviandoMedicao();

/*6
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 21                                                              #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 22                                                              #
# ProxEstado -> AGUARDAR_COMANDO_ATUAR                                          #
#                                                                               #                                                  
# sinal      -> 23                                                              #
# ProxEstado -> ENVIAR_RESPOSTA_CMD_ATUAR                                       #
#                                                                               #
# sinal      -> 24                                                              #
# ProxEstado -> ERROR                                                           #
#                                                                               #
 */
void MdE_Atuando();

/*7
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 25                                                              #
# ProxEstado -> CONFIGURAR_ADDRESS                                              #
#                                                                               #
# sinal      -> 26                                                              #
# ProxEstado -> ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR                         #
 */
void MdE_EnviandoMedicaoAcionandoAtuador();



/*8
#################################################################################
# Objetivo:                                                                     #
#                                                                               #
# sinal      -> 27                                                              #
# ProxEstado -> ERROR                                                           #
 */
void MdE_ERROU();

/*
#################################################################################
*/
//****************************************************************************//

#endif /* MDE_H */

//****************************************************************************//