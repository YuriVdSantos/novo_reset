#!/bin/bash

# ==============================================
# TESTER PARA MINISHELL - CONFORME EN.SUBJECT.PDF
# ==============================================

# Cores para output
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
RESET="\033[0m"

# Variáveis
MINISHELL="./minishell"
TEST_DIR="test_files"
OUT_DIR="test_outputs"
REFERENCE_OUT="reference_outputs"
LOG_FILE="test_results.log"
VALGRIND_LEAKS="valgrind_leaks.log"
MEMCHECK="valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --log-file=$VALGRIND_LEAKS"

# ==============================================
# FUNÇÕES AUXILIARES
# ==============================================

# Inicializar ambiente de teste
init_testing() {
    echo -e "${BLUE}\n[INICIANDO TESTES]${RESET}"
    mkdir -p $TEST_DIR $OUT_DIR $REFERENCE_OUT
    rm -f $OUT_DIR/* $REFERENCE_OUT/* $LOG_FILE $VALGRIND_LEAKS
}

# Rodar comando no minishell e no bash e comparar
run_test() {
    local test_num=$1
    local cmd=$2
    local test_name=$3
    
    echo -e "\n${YELLOW}Test $test_num: $test_name${RESET}"
    echo "Command: $cmd"
    
    # Rodar no minishell
    echo "$cmd" | $MEMCHECK $MINISHELL > $OUT_DIR/mini_$test_num 2>&1
    local mini_exit=$?
    
    # Rodar no bash (referência)
    echo "$cmd" | bash > $REFERENCE_OUT/bash_$test_num 2>&1
    local bash_exit=$?
    
    # Comparar outputs e exit codes
    diff -u $REFERENCE_OUT/bash_$test_num $OUT_DIR/mini_$test_num > /dev/null
    local diff_result=$?
    
    if [ $diff_result -eq 0 ] && [ $mini_exit -eq $bash_exit ]; then
        echo -e "${GREEN}✅ PASS${RESET}"
        echo "Test $test_num: PASS - $test_name" >> $LOG_FILE
    else
        echo -e "${RED}❌ FAIL${RESET}"
        echo "=== BASH OUTPUT ==="
        cat $REFERENCE_OUT/bash_$test_num
        echo "=== MINISHELL OUTPUT ==="
        cat $OUT_DIR/mini_$test_num
        echo "Exit codes: minishell=$mini_exit, bash=$bash_exit"
        echo "Test $test_num: FAIL - $test_name" >> $LOG_FILE
    fi
}

# Verificar vazamentos de memória
check_leaks() {
    if grep -q "definitely lost:" $VALGRIND_LEAKS; then
        echo -e "${RED}⚠️  MEMORY LEAKS DETECTED!${RESET}"
        grep -A5 "definitely lost:" $VALGRIND_LEAKS
        echo "Memory leaks detected in test $1" >> $LOG_FILE
    else
        echo -e "${GREEN}✅ No memory leaks detected${RESET}"
    fi
}

# ==============================================
# TESTES OBRIGATÓRIOS (MANDATORY)
# ==============================================

# 1. Testes de comandos simples
test_simple_commands() {
    echo -e "${BLUE}\n[1. TESTANDO COMANDOS SIMPLES]${RESET}"
    
    run_test 1 "ls" "Comando simples (ls)"
    run_test 2 "/bin/ls" "Comando com caminho absoluto"
    run_test 3 "echo Hello World" "Echo com argumentos"
    run_test 4 "echo -n Hello World" "Echo com opção -n"
    run_test 5 "pwd" "Comando pwd"
    run_test 6 "cd $TEST_DIR && pwd" "Comando cd + pwd"
    run_test 7 "exit 42" "Exit com código"
}

# 2. Testes de redirecionamentos
test_redirections() {
    echo -e "${BLUE}\n[2. TESTANDO REDIRECIONAMENTOS]${RESET}"
    
    # Criar arquivos de teste
    echo "test content" > $TEST_DIR/file1.txt
    echo "more content" > $TEST_DIR/file2.txt
    
    run_test 8 "cat < $TEST_DIR/file1.txt" "Redirecionamento de entrada (<)"
    run_test 9 "echo test > $OUT_DIR/out1.txt" "Redirecionamento de saída (>)"
    run_test 10 "echo append >> $OUT_DIR/out1.txt" "Redirecionamento append (>>)"
    run_test 11 "cat < $TEST_DIR/file1.txt > $OUT_DIR/out2.txt" "Redirecionamento entrada e saída"
    run_test 12 "cat < $TEST_DIR/nonexistent" "Redirecionamento para arquivo inexistente"
}

# 3. Testes de pipes
test_pipes() {
    echo -e "${BLUE}\n[3. TESTANDO PIPES]${RESET}"
    
    run_test 13 "ls | grep .sh" "Pipe simples"
    run_test 14 "cat $TEST_DIR/file1.txt | grep test" "Pipe com grep"
    run_test 15 "ls | grep .sh | wc -l" "Múltiplos pipes"
    run_test 16 "ls | grep nonexistent | wc -l" "Pipe com comando falho"
}

# 4. Testes de variáveis de ambiente
test_environment_vars() {
    echo -e "${BLUE}\n[4. TESTANDO VARIÁVEIS DE AMBIENTE]${RESET}"
    
    run_test 17 "echo \$HOME" "Variável $HOME"
    run_test 18 "echo \$PATH" "Variável $PATH"
    run_test 19 "echo \$NONEXISTENT" "Variável inexistente"
    run_test 20 "echo \$?" "Variável \$? (exit status)"
    run_test 21 "export TEST_VAR=123 && echo \$TEST_VAR" "Exportar variável"
    run_test 22 "unset TEST_VAR && echo \$TEST_VAR" "Unset variável"
}

# 5. Testes de built-ins
test_builtins() {
    echo -e "${BLUE}\n[5. TESTANDO BUILT-INS]${RESET}"
    
    run_test 23 "cd $TEST_DIR && pwd" "Built-in cd"
    run_test 24 "pwd" "Built-in pwd"
    run_test 25 "export" "Built-in export sem args"
    run_test 26 "env" "Built-in env"
    run_test 27 "unset PATH && ls" "Built-in unset (PATH)"
    run_test 28 "exit 42" "Built-in exit com código"
}

# 6. Testes de sinais
test_signals() {
    echo -e "${BLUE}\n[6. TESTANDO SINAIS]${RESET}"
    
    # Teste interativo (Ctrl+C)
    echo "Verifique manualmente:"
    echo "1. Execute ./minishell"
    echo "2. Digite um comando longo (ex: cat /dev/random)"
    echo "3. Pressione Ctrl+C - deve mostrar novo prompt"
    echo "4. Pressione Ctrl+D - deve sair do minishell"
    echo "5. Pressione Ctrl+\\ - não deve fazer nada"
    
    # Registrar como manual no log
    echo "Test 29: MANUAL - Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)" >> $LOG_FILE
}

# 7. Testes de citações
test_quotes() {
    echo -e "${BLUE}\n[7. TESTANDO CITAÇÕES]${RESET}"
    
    run_test 30 "echo \"test \$HOME\"" "Aspas duplas com expansão"
    run_test 31 "echo 'test \$HOME'" "Aspas simples sem expansão"
    run_test 32 "echo \"test 'inner' quotes\"" "Aspas aninhadas"
    run_test 33 "echo 'test \"inner\" quotes'" "Aspas aninhadas"
}

# 8. Testes de erros
test_errors() {
    echo -e "${BLUE}\n[8. TESTANDO TRATAMENTO DE ERROS]${RESET}"
    
    run_test 34 "command_does_not_exist" "Comando inexistente"
    run_test 35 "ls nonexistent_file" "Arquivo inexistente"
    run_test 36 "cd nonexistent_dir" "Diretório inexistente"
    run_test 37 "echo \"unclosed quote" "Aspas não fechadas"
    run_test 38 "|" "Pipe sem comandos"
    run_test 39 ">" "Redirecionamento sem arquivo"
}

# ==============================================
# TESTES BÔNUS (OPCIONAIS)
# ==============================================

test_bonus() {
    echo -e "${BLUE}\n[9. TESTES BÔNUS (OPCIONAIS)]${RESET}"
    
    # && e ||
    run_test 40 "true && echo success" "Operador &&"
    run_test 41 "false || echo failure" "Operador ||"
    
    # Wildcards
    run_test 42 "ls *.sh" "Wildcard *"
    run_test 43 "ls test_*" "Wildcard prefix"
}

# ==============================================
# FUNÇÃO PRINCIPAL
# ==============================================

main() {
    init_testing
    
    # Testes obrigatórios
    test_simple_commands
    test_redirections
    test_pipes
    test_environment_vars
    test_builtins
    test_signals
    test_quotes
    test_errors
    
    # Testes bônus (descomente se implementou)
    # test_bonus
    
    # Verificar vazamentos de memória no último teste
    check_leaks
    
    echo -e "${BLUE}\n[RELATÓRIO FINAL]${RESET}"
    echo "Resultados dos testes salvos em $LOG_FILE"
    echo "Logs do Valgrind salvos em $VALGRIND_LEAKS"
    
    # Resumo
    PASS_COUNT=$(grep -c "PASS" $LOG_FILE)
    FAIL_COUNT=$(grep -c "FAIL" $LOG_FILE)
    echo -e "\n${GREEN}PASS: $PASS_COUNT${RESET} ${RED}FAIL: $FAIL_COUNT${RESET}"
}

main