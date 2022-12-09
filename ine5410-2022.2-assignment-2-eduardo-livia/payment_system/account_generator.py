from random import randint
import time
from threading import Thread

from globals import *
from payment_system.bank import Bank
from utils.transaction import Transaction
from utils.currency import Currency
from utils.logger import LOGGER


class AccountGenerator(Thread):
    """
    Uma classe para gerar e simular clientes de um banco por meio da geracão de transações bancárias.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id : int
        Identificador do gerador de transações.
    bank: Bank
        Banco sob o qual o gerador de transações operará.
    max_accounts: int
        Número máximo de contas a serem criadas.

    Métodos
    -------
    run():
        ....
	"""
	
    def __init__(self, _id: int, bank: Bank, max_account, max_ovdft_limit, max_amount):
        Thread.__init__(self)
        self._id  = _id
        self.bank = bank
        self.max_account = max_account
        self.max_ovdft_limit = max_ovdft_limit
        self.max_amount = max_amount

    def run(self):
        """
        Esse método deverá gerar transacões aleatórias enquanto o banco (self._bank_id)
        estiver em operação.
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!

        LOGGER.info(f"Inicializado AccountGenerator para o Banco Nacional {self.bank._id}!")

        while self.bank.operating and len(self.bank.accounts) < self.max_account:
            self.bank.new_account(randint(0, self.max_account), randint(0, self.max_ovdft_limit)) 
            LOGGER.info(f"Conta {len(self.bank.accounts)} criada para o Banco {self.bank._id}!")  
            time.sleep(0.2 * time_unit)

        LOGGER.info(f"O AccountGenerator {self._id} do banco {self.bank._id} foi finalizado.")
