from payment_system.account import Account, CurrencyReserves
from utils.transaction import Transaction
from utils.currency import Currency
from utils.logger import LOGGER
from threading import Semaphore, Lock


class Bank():
    """
    Uma classe para representar um Banco.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id : int
        Identificador do banco.
    currency : Currency
        Moeda corrente das contas bancárias do banco.
    reserves : CurrencyReserves
        Dataclass de contas bancárias contendo as reservas internas do banco.
    operating : bool
        Booleano que indica se o banco está em funcionamento ou não.
    accounts : List[Account]
        Lista contendo as contas bancárias dos clientes do banco.
    transaction_queue : Queue[Transaction]
        Fila FIFO contendo as transações bancárias pendentes que ainda serão processadas.
    transact_queue_sem: Semaphore()
        Semáforo que conta quantidade de transações na fila
    transact_queue_lock: Lock()
        Lock que protege a fila inteira
    count_national_transact: int
        Contador de transações nacionais realizadas
    count_international_transact: int
        Contador de transações internacionais realizadas
    count_national_transact_lock: Lock
        Lock para proteger incremento de contador de transações nacionais
    count_international_transact_lock: Lock
        Lock para proteger incremento de contador de transações internacionais
    count_profit: int
        Conta lucro do banco com taxas de câmbio e juros de cheques 
    count_profit_lock = Lock()
        Mutex para proteger o contador de lucro
    account_generator = Thread()
        Thread que cria as contas do banco
    count_processed = int
        Indica quantas transações foram processadas
    count_processed_lock() = Lock()
        Lock para contagem de transações procesadas
    count_total_processing_time = int
        Tempo total de processamento das transações do banco

    
    Métodos
    -------
    new_account(balance: int = 0, overdraft_limit: int = 0) -> None:
        Cria uma nova conta bancária (Account) no banco.
    new_transfer(origin: Tuple[int, int], destination: Tuple[int, int], amount: int, currency: Currency) -> None:
        Cria uma nova transação bancária.
    info() -> None:
        Printa informações e estatísticas sobre o funcionamento do banco.
    
    """

    def __init__(self, _id: int, currency: Currency):
        self._id                = _id
        self.currency           = currency
        self.reserves           = CurrencyReserves()
        self.operating          = False
        self.accounts           = []
        self.transaction_queue  = []
        self.transaction_generator = None
        self.account_generator = None
        self.payment_processors = []
        self.transact_queue_sem = Semaphore(0)
        self.transact_queue_lock = Lock()
        self.count_national_transact_lock = Lock()
        self.count_international_transact_lock = Lock()
        self.count_national_transact = 0
        self.count_international_transact = 0
        self.count_profit = 0
        self.count_profit_lock = Lock()
        self.count_processed = 0
        self.count_processed_lock = Lock()
        self.count_total_processing_time = 0

    def new_account(self, balance: int = 0, overdraft_limit: int = 0) -> None:
        """
        Esse método deverá criar uma nova conta bancária (Account) no banco com determinado 
        saldo (balance) e limite de cheque especial (overdraft_limit).
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!

        # Gera _id para a nova Account
        acc_id = len(self.accounts) + 1

        # Cria instância da classe Account
        acc = Account(_id=acc_id, _bank_id=self._id, currency=self.currency, balance=balance, overdraft_limit=overdraft_limit)
  
        # Adiciona a Account criada na lista de contas do banco
        self.accounts.append(acc)


    def info(self) -> None:
        """
        Essa função deverá printar os seguintes dados utilizando o LOGGER fornecido:
        1. Saldo de cada moeda nas reservas internas do banco
        2. Número de transferências nacionais e internacionais realizadas
        3. Número de contas bancárias registradas no banco
        4. Saldo total de todas as contas bancárias (dos clientes) registradas no banco
        5. Lucro do banco: taxas de câmbio acumuladas + juros de cheque especial acumulados
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!
        print('=' * 80)
        LOGGER.info(f"Estatísticas do Banco Nacional {self._id}:")
        print()
        LOGGER.info(f"Saldo de cada moeda nas reservas internas do banco:")
        LOGGER.info(f"| USD = {self.reserves.USD.balance}")
        LOGGER.info(f"| EUR = {self.reserves.EUR.balance}")
        LOGGER.info(f"| GBP = {self.reserves.GBP.balance}")
        LOGGER.info(f"| JPY = {self.reserves.JPY.balance}")
        LOGGER.info(f"| CHF = {self.reserves.CHF.balance}")
        LOGGER.info(f"| BRL = {self.reserves.BRL.balance}")
        print()
        LOGGER.info(f"Número de transferências nacionais e internacionais realizadas:")
        LOGGER.info(f"Quantidade de transações nacionais: {self.count_national_transact}")
        LOGGER.info(f"Quantidade de transações internacionais: {self.count_international_transact}")
        print()
        LOGGER.info(f'Número de contas bancárias registradas no banco: {len(self.accounts)}')
        LOGGER.info(f'Saldo das contas:')
        for account in self.accounts:
            LOGGER.info(f'| Conta {account._id} = {account.balance}')
        print()
        LOGGER.info(f'Lucro do banco: {self.count_profit}\n')

