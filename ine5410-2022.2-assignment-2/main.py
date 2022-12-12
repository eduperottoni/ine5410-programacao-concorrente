import argparse, time, sys
from logging import INFO, DEBUG
from random import randint
from threading import Semaphore, Lock

from globals import *
from payment_system.bank import Bank
from payment_system.payment_processor import PaymentProcessor
from payment_system.transaction_generator import TransactionGenerator
from utils.currency import Currency
from payment_system.account_generator import AccountGenerator
from utils.logger import CH, LOGGER



if __name__ == "__main__":
    # Verificação de compatibilidade da versão do python:
    if sys.version_info < (3, 5):
        sys.stdout.write('Utilize o Python 3.5 ou mais recente para desenvolver este trabalho.\n')
        sys.exit(1)

    # Captura de argumentos da linha de comando:
    parser = argparse.ArgumentParser()
    parser.add_argument("--time_unit", "-u", help="Valor da unidade de tempo de simulação")
    parser.add_argument("--total_time", "-t", help="Tempo total de simulação")
    parser.add_argument("--debug", "-d", help="Printar logs em nível DEBUG")
    args = parser.parse_args()
    if args.time_unit:
        time_unit = float(args.time_unit)
    if args.total_time:
        total_time = int(args.total_time)
    if args.debug:
        debug = True

    # Configura logger
    if debug:
        LOGGER.setLevel(DEBUG)
        CH.setLevel(DEBUG)
    else:
        LOGGER.setLevel(INFO)
        CH.setLevel(INFO)

    # Printa argumentos capturados da simulação
    LOGGER.info(f"Iniciando simulação com os seguintes parâmetros:\n\ttotal_time = {total_time}\n\tdebug = {debug}\n")
    time.sleep(3)

    # Inicializa variável `tempo`:
    t = 0
    
    # Cria os Bancos Nacionais e popula a lista global `banks`:
    for i, currency in enumerate(Currency):
        print(i)
        print(currency)
        
        # Cria Banco Nacional
        bank = Bank(_id=i, currency=currency)
        
        # Deposita valores aleatórios nas contas internas (reserves) do banco
        bank.reserves.BRL.deposit(randint(100_000_000, 10_000_000_000))
        bank.reserves.CHF.deposit(randint(100_000_000, 10_000_000_000))
        bank.reserves.EUR.deposit(randint(100_000_000, 10_000_000_000))
        bank.reserves.GBP.deposit(randint(100_000_000, 10_000_000_000))
        bank.reserves.JPY.deposit(randint(100_000_000, 10_000_000_000))
        bank.reserves.USD.deposit(randint(100_000_000, 10_000_000_000))
        
        # Adiciona banco na lista global de bancos
        banks.append(bank)

        # Seta operando como True 
        bank.operating = True

    #Gera contas para cada banco
    for bank in banks:
        bank.account_generator = AccountGenerator(0, bank, 100, 1000, 1000)
        bank.account_generator.start()

    
    # Inicializa gerador de transações e processadores de pagamentos para os Bancos Nacionais:
    for i, bank in enumerate(banks):
        # Inicializa um TransactionGenerator thread por banco:
        transaction = TransactionGenerator(_id=i, bank=bank)
        bank.generator = transaction
        transaction.start()

        # Inicializa um PaymentProcessor thread por banco.
        # Sua solução completa deverá funcionar corretamente com múltiplos PaymentProcessor threads para cada banco.
        for j in range(n_processors):
            payment_proc = PaymentProcessor(_id=j, bank=bank)
            bank.payment_processors.append(payment_proc)
            payment_proc.start()
     
    # Enquanto o tempo total de simuação não for atingido:
    while t < total_time:
        # Aguarda um tempo aleatório antes de criar o próximo cliente:
        dt = randint(0, 3)
        time.sleep(dt * time_unit)
        # Atualiza a variável tempo considerando o intervalo de criação dos clientes:
        t += dt


    # Seta operating para Falso - Término da simulação
    for bank in banks: 
        bank.operating = False

    # Finaliza todas as threads
    # TODO
    for bank in banks:
        bank.generator.join()
        # alguns releases para garantir que nenhum processor ficará trancado nos semáforos das filas
        for processor in bank.payment_processors:
            bank.transact_queue_sem.release()
        for processor in bank.payment_processors:
            processor.join()

    # Termina simulação. Após esse print somente dados devem ser printados no console.
    LOGGER.info(f"A simulação chegou ao fim!\n")

    total_time_processing = 0
    total_transact_processed = 0
    total_not_processed = 0

    #calcula tempos totais para todos os bancos e não processadas
    for bank in banks:
        total_time_processing += bank.count_total_processing_time
        total_transact_processed += bank.count_processed
        total_not_processed += len(bank.transaction_queue)
        bank.info()

    LOGGER.info(f'=== ESTATÍSTICAS GERAIS DA SIMULAÇÃO ===')
    LOGGER.info(f'| Tempo médio de fila das transações processadas: {total_time_processing / total_transact_processed} seconds')
    LOGGER.info(f'| Total de Transações processadas: {total_transact_processed} transações')
    LOGGER.info(f'| Total de Transações não processadas: {total_not_processed} transações')

    
