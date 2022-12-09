# from random import randint
# import time
# from threading import Thread

# from globals import *
# from payment_system.bank import Bank
# from utils.logger import LOGGER


# class AccountGenerator(Thread):
#     """
#     Uma classe para gerar e simular clientes de um banco por meio da geracão de transações bancárias.
#     Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

#     ...

#     Atributos
#     ---------
#     _id : int
#         Identificador do gerador de transações.
#     bank: Bank
#         Banco sob o qual o gerador de transações operará.
#     max_accounts:
#         Máximo de contas a serem geradas

#     Métodos
#     -------
#     run():
#         ....
#     """

#     def __init__(self, _id: int, bank: Bank, max_accounts: int):
#         Thread.__init__(self)
#         self._id  = _id
#         self.bank = bank
#         self.max_accounts = max_accounts


#     def run(self):
#         """
#         Esse método deverá gerar transacões aleatórias enquanto o banco (self._bank_id)
#         estiver em operação.
#         """
#         # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!

#         LOGGER.info(f"Inicializado AccountGenerator para o Banco Nacional {self.bank._id}!")

#         i = 0   
#         while self.bank.operating:

#             balance = randint(100, 1000)
#             ovdft_limit = randint(100,1000)
#             self.bank.new_account(balance, ovdft_limit)
            
#             #banks[self.bank._id].transaction_queue.append(new_transaction)
#             #print(f'COLOCOU A TRANSACAO {new_transaction._id} NO BANCO {self.bank._id}. CONTA DE DESTINO = {destination[1]}. CONTA DE ORIGEM = {origin[1]}. ')
#             LOGGER.info(f"Transaction_queue do Banco {self.bank._id}: {len(banks[self.bank._id].transaction_queue)}")
#             #self.bank.transact_queue_sem.release()
#             i+=1
#             time.sleep(0.2 * time_unit)

#         LOGGER.info(f"O AccountGenerator {self._id} do banco {self.bank._id} foi finalizado.")
