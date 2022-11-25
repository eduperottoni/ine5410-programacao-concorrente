from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    sem_vazio.acquire()
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    # verifica se há lugar no buffer
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sem_cheio.release()

def consumidor():
  global buffer
  for i in range(10):
    # aguarda que haja um item para consumir 
    sem_cheio.acquire()
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sleep(randint(0,2))         # fica um tempo consumindo...
    sem_vazio.release()

buffer = []
tam_buffer = 3

# cria semáforos
sem_cheio = Semaphore(0)
sem_vazio = Semaphore(tam_buffer)

#caso se tenha mais de uma thread, deve-se ter Locks para produtores e consumidores

# Cria a thread produtora e a consumidora.
produtor = Thread(target=produtor) 
consumidor = Thread(target=consumidor)

# Inicia a thread produtora e a consumidora.
produtor.start()
consumidor.start()

# Aguarda a thread produtora e a consumidora finalizarem.
produtor.join()
consumidor.join()