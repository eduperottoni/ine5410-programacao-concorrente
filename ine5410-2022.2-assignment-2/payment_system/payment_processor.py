import time
from threading import Thread

from globals import *
from payment_system.bank import Bank
from utils.transaction import Transaction, TransactionStatus
from utils.currency import get_exchange_rate, Currency
from utils.logger import LOGGER



class PaymentProcessor(Thread):
    """
    Uma classe para representar um processador de pagamentos de um banco.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id : int
        Identificador do processador de pagamentos.
    bank: Bank
        Banco sob o qual o processador de pagamentos operará.

    Métodos
    -------
    run():
        Inicia thread to PaymentProcessor
    process_transaction(transaction: Transaction) -> TransactionStatus:
        Processa uma transação bancária.
    """

    def __init__(self, _id: int, bank: Bank):
        Thread.__init__(self)
        self._id  = _id
        self.bank = bank


    def run(self):
        """
        Esse método deve buscar Transactions na fila de transações do banco e processá-las 
        utilizando o método self.process_transaction(self, transaction: Transaction).
        Ele não deve ser finalizado prematuramente (antes do banco realmente fechar).
        """
        # TODO: IMPLEMENTE/MODIFIQUE O CÓDIGO NECESSÁRIO ABAIXO !

        LOGGER.info(f"Inicializado o PaymentProcessor {self._id} do Banco {self.bank._id}!")
        queue = self.bank.transaction_queue

        while self.bank.operating:
            try:

                #SEMÁFORO QUE CONTA QUANTIDADE DE TRANSACOES NA FILA (EVITA ESPERA OCUPADA)
                #PROBLEMA: 2 PAYMENT_PROCESSORS PEGANDO A MESMA TRANSACTION
                queue_sems[self.bank._id].acquire()
                
                transaction = queue.pop(0)
                print(f'RETIROU UMA TRANSACAO {transaction._id} DO BANCO {self.bank._id}. origem = {transaction.origin[1]}. destino = {transaction.destination[1]}')
                LOGGER.info(f"Transaction_queue do Banco {self.bank._id}: {len(queue)}")
                
            except Exception as err:
                LOGGER.error(f"Falha em PaymentProcessor.run(): {err}")
            else:
                status = self.process_transaction(transaction)
                print(status)
            #time.sleep(3 * time_unit)  # Remova esse sleep após implementar sua solução!

        LOGGER.info(f"O PaymentProcessor {self._id} do banco {self.bank._id} foi finalizado.")

    def __process_national_transaction(self, transaction: Transaction) -> TransactionStatus:
        # chama withdraw da conta origem e deposita na destino
        origin_account = self.bank.accounts[transaction.origin[1] - 1]
        destination_account = self.bank.accounts[transaction.destination[1] - 1]
        success = origin_account.withdraw(transaction.amount)
        if success:
            destination_account.deposit(transaction.amount)
            transaction.set_status(TransactionStatus.SUCCESSFUL)
        else:
            # LOGGER.error(f"Erro na Transação Nacional do banco {self.bank._id}")
            transaction.set_status(TransactionStatus.FAILED)
        return transaction.status
        
    def __process_international_transaction(self, transaction: Transaction) -> TransactionStatus:
        origin_account = self.bank.accounts[transaction.origin[1] - 1]

        success = origin_account.withdraw(transaction.amount)

        if success:
            f = self.bank.currency  #Currency.BRL
            t = transaction.currency
            exch_rate = get_exchange_rate(f, t)
            converted_amount = transaction.amount * exch_rate
            
            origin_reserve_acc = self.bank.reserves.BRL
            #caso seja USD
            if t == Currency.USD:
                origin_reserve_acc = self.bank.reserves.USD
            elif t == Currency.EUR:
                origin_reserve_acc = self.bank.reserves.EUR
            elif t == Currency.GBP:
                origin_reserve_acc = self.bank.reserves.GBP
            elif t == Currency.JPY:
                origin_reserve_acc = self.bank.reserves.JPY
            elif t == Currency.CHF:
                origin_reserve_acc = self.bank.reserves.CHF

            origin_reserve_acc.deposit(converted_amount)

            origin_reserve_acc.withdraw(0.99 * converted_amount)

            destination_account = banks[transaction.destination[0]].accounts[transaction.destination[1] - 1]
            destination_account.deposit(0.99 * converted_amount)
            transaction.set_status(TransactionStatus.SUCCESSFUL)
        else:
            transaction.set_status(TransactionStatus.FAILED)
        
        return transaction.status

    def process_transaction(self, transaction: Transaction) -> TransactionStatus:
        """
        Esse método deverá processar as transações bancárias do banco ao qual foi designado.
        Caso a transferência seja realizada para um banco diferente (em moeda diferente), a 
        lógica para transações internacionais detalhada no enunciado (README.md) deverá ser
        aplicada.
        Ela deve retornar o status da transacão processada.
        """
        # TODO: IMPLEMENTE/MODIFIQUE O CÓDIGO NECESSÁRIO ABAIXO !

        LOGGER.info(f"PaymentProcessor {self._id} do Banco {self.bank._id} iniciando processamento da Transaction {transaction._id}!")
        
        print(f'CONTA DE ORIGEM: {self.bank.accounts[transaction.origin[1] - 1].balance}')
        print(f'QUANTO QUER TIRAR: {transaction.amount}')
        # Se a transação é nacional ou internacional
        if (transaction.origin[0] == transaction.destination[0]):
            status = self.__process_national_transaction(transaction)
        else:
            status = self.__process_international_transaction(transaction)
        
        # NÃO REMOVA ESSE SLEEP!
        # Ele simula uma latência de processamento para a transação.
        time.sleep(3 * time_unit)
        
        #transaction.set_status(TransactionStatus.SUCCESSFUL)
        return status
