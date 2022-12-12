# Lista de Bancos Nacionais
banks = []

# Printar Logs de DEBUG no console?
debug = False

# Tempo total de simulação - original = 1000
total_time = 1000

# Unidade de tempo (quanto menor, mais rápida a simulação)
time_unit = 0.1  # 0.1 = 100ms

# Quantidade de Payment Processors por banco
n_processors = 30


# Máximo de contas criadas por banco
max_accounts = 100

# Máximo de balance por conta criada
max_balance = 1000000

# Máximo de overdraft_limit por conta criada
max_overdraft_limit = 10000

'''Quanto % sobre o valor da conta é concedido de limite para o cliente
no momento da criação da conta'''
ovdft_limit_pct = 0.15 # 15%

