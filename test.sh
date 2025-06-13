#!/bin/bash

# Script de teste para o Minishell com Valgrind

# Cores para o output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Compila o minishell com flags de debug
echo "Compilando o minishell com flags de debug..."
make debug

# Verifica se a compilação foi bem-sucedida
if [ ! -f ./minishell ]; then
    echo -e "${RED}Erro na compilação. Abortando teste.${NC}"
    exit 1
fi

# Verifica se o arquivo de supressão existe
if [ ! -f ./readline.supp ]; then
    echo -e "${RED}Arquivo de supressão 'readline.supp' não encontrado. Abortando teste.${NC}"
    exit 1
fi


echo -e "\n--- ${GREEN}Iniciando testes com Valgrind para memory leaks e double frees${NC} ---\n"

# Array de comandos para testar
declare -a commands=(
    "echo hello world"
    "ls -l | grep 'minishell' | wc -l"
    "cat < Makefile > outfile.txt"
    "cat < srcs/10.builtins/ft_echo.c >> outfile.txt"
    "grep 'static' < Makefile | sort -r"
    "export TEST_VAR=123"
    "echo \$TEST_VAR"
    "unset TEST_VAR"
    "echo \$TEST_VAR"
    "cd srcs"
    "pwd"
    "cd .."
    "pwd"
    "invalid_command"
    "echo 'teste com aspas simples'"
    "echo \"teste com aspas duplas e \$PWD\""
    "cat << EOF\nheredoc test\nline 2\nEOF"
    "| pipes no inicio" # Erro de sintaxe
    "echo test >" # Erro de sintaxe
    "exit 42"
)

# Executa cada comando com Valgrind
for cmd in "${commands[@]}"; do
    echo -e "\n--- Testando comando: ${GREEN}${cmd}${NC} ---"
    # ATUALIZADO: Adicionada a flag --suppressions
    echo "${cmd}" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./readline.supp --log-file="valgrind_report.txt" ./minishell > /dev/null 2>&1

    # Analisa o relatório do Valgrind
    if grep -q "All heap blocks were freed -- no leaks are possible" valgrind_report.txt && ! grep -q "Invalid read" valgrind_report.txt && ! grep -q "Invalid write" valgrind_report.txt && ! grep -q "double free" valgrind_report.txt; then
        echo -e "Resultado: ${GREEN}Nenhum leak ou erro detectado.${NC}"
    else
        echo -e "Resultado: ${RED}Memory leak, double free ou outro erro detectado!${NC}"
        echo "Veja o arquivo 'valgrind_report.txt' para detalhes."
        # Pausa para o usuário analisar o report antes de continuar
        read -p "Pressione Enter para continuar para o próximo teste..."
    fi
done

echo -e "\n--- ${GREEN}Testes concluídos.${NC} ---"
echo "Limpando arquivos gerados..."
rm -f outfile.txt valgrind_report.txt
make fclean > /dev/null
