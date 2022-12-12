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
                self.bank.transact_queue_sem.acquire()
                # Caso o processor seja liberado quando a simulação estiver terminado
                if not self.bank.operating:
                    break

                #PROBLEMA: 2 PAYMENT_PROCESSORS PEGANDO A MESMA TRANSACTION
                self.bank.transact_queue_lock.acquire()
                transaction = queue.pop(0)
                self.bank.transact_queue_lock.release()

                LOGGER.info(f"Transaction_queue do Banco {self.bank._id}: {len(queue)}")
                
            except Exception as err:
                LOGGER.error(f"Falha em PaymentProcessor.run(): {err}")
            else:
                status = self.process_transaction(transaction)
                LOGGER.info(f'Transação {transaction._id} do banco {self.bank._id} processada! Status: {transaction.status}')
            #time.sleep(3 * time_unit)  # Remova esse sleep após implementar sua solução!

        LOGGER.info(f"O PaymentProcessor {self._id} do banco {self.bank._id} foi finalizado.")

    def __process_national_transaction(self, transaction: Transaction) -> TransactionStatus:
        # chama withdraw da conta origem e deposita na destino
        origin_account = self.bank.accounts[transaction.origin[1] - 1]
        destination_account = self.bank.accounts[transaction.destination[1] - 1]
        # LOCK
        origin_account.account_lock.acquire()
        success = origin_account.withdraw(transaction.amount)
        origin_account.account_lock.release()
        if success:
            # LOCK
            destination_account.account_lock.acquire()
            destination_account.deposit(transaction.amount)
            destination_account.account_lock.release()

            transaction.set_status(TransactionStatus.SUCCESSFUL)
            self.bank.count_national_transact_lock.acquire()
            self.bank.count_national_transact += 1
            self.bank.count_national_transact_lock.release()
        else:
            # LOGGER.error(f"Erro na Transação Nacional do banco {self.bank._id}")
            transaction.set_status(TransactionStatus.FAILED)
        return transaction.status
        
    def __process_international_transaction(self, transaction: Transaction) -> TransactionStatus:
        origin_account = self.bank.accounts[transaction.origin[1] - 1]
        
        origin = self.bank.currency     #Currency.BRL (banco 0)
        destin = transaction.currency   #Currency.USD (banco 1)
        exch_rate = get_exchange_rate(destin, origin)
        # grana em BRL
        converted_amount = transaction.amount * exch_rate
        tax = converted_amount * 0.01
                
        # LOCK 
        origin_account.account_lock.acquire()
        success = origin_account.withdraw(converted_amount + tax)
        origin_account.account_lock.release()

        if success:
            self.bank.count_profit_lock.acquire()
            self.bank.count_profit += tax
            self.bank.count_profit_lock.release()


            origin_reserve_acc = self.bank.reserves.BRL
            #caso seja USD
            if origin == Currency.USD:
                origin_reserve_acc = self.bank.reserves.USD
            elif origin == Currency.EUR:
                origin_reserve_acc = self.bank.reserves.EUR
            elif origin == Currency.GBP:
                origin_reserve_acc = self.bank.reserves.GBP
            elif origin == Currency.JPY:
                origin_reserve_acc = self.bank.reserves.JPY
            elif origin == Currency.CHF:
                origin_reserve_acc = self.bank.reserves.CHF
            
            # LOCK 
            origin_reserve_acc.account_lock.acquire()
            origin_reserve_acc.deposit(converted_amount + tax)
            origin_reserve_acc.account_lock.release()

            destin_reserve_acc = self.bank.reserves.BRL
        
            #caso seja USD
            if destin == Currency.USD:
                destin_reserve_acc = self.bank.reserves.USD
            elif destin == Currency.EUR:
                destin_reserve_acc = self.bank.reserves.EUR
            elif destin == Currency.GBP:
                destin_reserve_acc = self.bank.reserves.GBP
            elif destin == Currency.JPY:
                destin_reserve_acc = self.bank.reserves.JPY
            elif destin == Currency.CHF:
                destin_reserve_acc = self.bank.reserves.CHF
            #LOCK
            destin_reserve_acc.account_lock.acquire()
            destin_reserve_acc.withdraw(transaction.amount)
            destin_reserve_acc.account_lock.release()

            destination_account = banks[transaction.destination[0]].accounts[transaction.destination[1] - 1]

            # LOCK
            destination_account.account_lock.acquire()
            destination_account.deposit(transaction.amount)
            destination_account.account_lock.release()

            transaction.set_status(TransactionStatus.SUCCESSFUL)

            self.bank.count_international_transact_lock.acquire()
            self.bank.count_international_transact += 1
            self.bank.count_international_transact_lock.release()
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
        
        # Se a transação é nacional ou internacional
        if (transaction.origin[0] == transaction.destination[0]):
            status = self.__process_national_transaction(transaction)
        else:
            status = self.__process_international_transaction(transaction)

        self.bank.count_processed_lock.acquire()
        self.bank.count_processed += 1
        self.bank.count_total_processing_time += transaction.get_processing_time().total_seconds()
        self.bank.count_processed_lock.release()
        
        # NÃO REMOVA ESSE SLEEP!
        # Ele simula uma latência de processamento para a transação.
        time.sleep(3 * time_unit)
        
        #transaction.set_status(TransactionStatus.SUCCESSFUL)
        return status
