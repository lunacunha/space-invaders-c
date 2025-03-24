#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;  // contador de interrupções


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  /* To be implemented by the students */
  
  /* 1ª Parte: Obter o status do timer */
  uint8_t status;
  if (timer_get_conf(timer, &status)) return 1;  

  /* 2ª Parte: Mostrar o status do timer */
  if (timer_display_conf(timer, status, field)) return 1; 

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */

  /* Atualizar a frequência do timer */
  if (timer_set_frequency(timer, freq)) return 1;

  return 0;
}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  
  int ipc_st;       // status da interrupção
  int ret;          // valor de retorno da chamada do sistema
  uint8_t irq_set;  // conjunto de interrupções que o programa está interessado em mostrar
  message msg;      // mensagem para a comunicação entre processos


  /* 1ª Parte: Subscrever as interrupções do timer */
  if (timer_subscribe_int(&irq_set)) return 1;


  /* 2ª Parte: Ciclo de interrupções */

  while (time > 0) {
    /* Obter uma mensagem do buffer de mensagens */
    if ((ret = driver_receive(ANY, &msg, &ipc_st)) != 0) {
      printf("driver_receive failed with: %d", ret);
      continue;
    }

    /* Verificar se a mensagem é uma notificação */
    if (is_ipc_notify(ipc_st)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set)) {    // Verificar se a interrupção é do timer 
            timer_int_handler();                      // Incrementar o contador de interrupções

            if (counter % 60 == 0) {
              timer_print_elapsed_time();        // Imprimir o tempo decorrido
              time--;                            // Decrementar o tempo
            }
          }
          break;
        default:
          break;
      }
    }
  }

  /* 3ª Parte: Dessubscrever as interrupções do timer */
  if (timer_unsubscribe_int()) return 1;

  return 0;
}
