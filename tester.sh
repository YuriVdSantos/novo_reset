#!/bin/bash

<<<<<<< HEAD
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
=======
# --- Configurações ---
# Caminho para o executável do minishell.
# O script tentará compilá-lo usando 'make'.
MINISHELL_EXEC="./minishell"

# --- Cores para o output ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # Sem Cor

# --- Contadores de Testes ---
PASSED_TESTS=0
FAILED_TESTS=0

# --- Funções de Ajuda ---

# Imprime o cabeçalho de uma seção de testes
print_header() {
    echo -e "\n${BLUE}=======================================================================${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}=======================================================================${NC}"
}

# Registra um teste que passou
print_pass() {
    echo -e "${GREEN}[  OK  ]${NC} $1"
    ((PASSED_TESTS++))
}

# Registra um teste que falhou
print_fail() {
    echo -e "${RED}[ FALHA ]${NC} $1"
    ((FAILED_TESTS++))
}

# Imprime uma informação
print_info() {
    echo -e "        ${YELLOW}↳ INFO:${NC} $1"
}

# Prepara o ambiente de teste
setup() {
    # Cria arquivos e diretórios necessários para os testes
    echo "linha 1 do infile" > infile
    echo "linha 2 do infile" >> infile
    touch empty_file
}

# Limpa o ambiente de teste
cleanup() {
    print_header "Limpeza Final"
    rm -f infile outfile empty_file ls_out.txt diff.log minishell.log bash.log
    rm -rf test_dir
    print_info "Arquivos e diretórios temporários removidos."
}

# --- Função Principal de Teste ---

# Executa um comando no minishell e no bash, e compara os resultados.
# Uso: run_test "Descrição do Teste" "comando a ser testado"
run_test() {
    local description="$1"
    local command="$2"

    echo -e "\n-> ${YELLOW}Teste:${NC} $description"
    echo -e "   ${YELLOW}Comando:${NC} '$command'"

    # --- Execução no Bash ---
    # Usamos um subshell para não afetar o script principal
    (exit_status=0; eval "$command" > bash.log 2>&1 || exit_status=$?; echo "$exit_status" >> bash.log)
    local bash_status=$(tail -n1 bash.log)
    local bash_output=$(sed '$d' bash.log)

    # --- Execução no Minishell ---
    # Usamos 'timeout' para evitar que o teste trave em loops infinitos
    # Adicionamos 'exit' para garantir que o minishell termine após o comando
    echo "$command" | timeout 8s "$MINISHELL_EXEC" > minishell.log 2>&1
    local minishell_status=$?

    # Verifica se o minishell deu timeout
    if [ $minishell_status -eq 124 ]; then
        print_fail "'$command'"
        print_info "O Minishell atingiu o tempo limite (8s). Pode haver um loop infinito."
        return
    fi
    
    # Precisamos do status de saída do comando, não do minishell. Vamos pegar com $?
    echo "$command; echo \$?" | timeout 8s "$MINISHELL_EXEC" > minishell.log 2>&1
    minishell_status=$(tail -n1 minishell.log | tr -d '\n')
    local minishell_output=$(sed '$d' minishell.log)


    # --- Comparação ---
    local outputs_match=1
    diff <(echo "$bash_output") <(echo "$minishell_output") > diff.log || outputs_match=0
    
    # Compara o status de saída
    if [ "$minishell_status" != "$bash_status" ]; then
        print_fail "'$description'"
        print_info "Status de saída divergente. Esperado (bash): $bash_status, Recebido (minishell): $minishell_status"
        # Não retorna para que a diferença de saída também seja mostrada se houver
    fi

    # Compara a saída padrão (stdout/stderr)
    if [ $outputs_match -eq 0 ]; then
        # Se o status de erro for o mesmo, podemos ser lenientes com a mensagem de erro
        if [ "$minishell_status" == "$bash_status" ] && [ "$bash_status" -ne 0 ]; then
             print_pass "'$description' (Status de erro $bash_status OK, mensagem pode variar)"
        else
            print_fail "'$description'"
            print_info "As saídas (stdout/stderr) são diferentes. Veja o diff:"
            cat diff.log | sed 's/^/            /'
        fi
    fi

    # Se ambos status e saídas forem iguais (e o status de falha não foi setado antes)
    if [ "$minishell_status" == "$bash_status" ] && [ $outputs_match -eq 1 ]; then
        print_pass "'$description'"
    fi
}

# --- Seções de Teste ---

run_compilation_tests() {
    print_header "Parte 1: Compilação"
    if [ ! -f "Makefile" ]; then
        print_fail "Makefile não encontrado."
        exit 1
    fi
    print_pass "Makefile encontrado."

    if ! make -n | grep -q -- "-Wall -Wextra -Werror"; then
        print_fail "Makefile não parece usar as flags -Wall -Wextra -Werror."
    else
        print_pass "Makefile parece usar as flags -Wall -Wextra -Werror."
    fi

    echo "Compilando o projeto..."
    make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        print_fail "Falha na compilação do minishell. Verifique os erros com 'make'."
        exit 1
    fi
    
    if [ ! -x "$MINISHELL_EXEC" ]; then
        print_fail "Executável '$MINISHELL_EXEC' não encontrado ou não é executável após 'make'."
        exit 1
    fi
    print_pass "Minishell compilado com sucesso."

    local make_output
    make_output=$(make 2>&1)
    if [[ "$make_output" == *"up to date"* || "$make_output" == *"à jour"* || -z "$make_output" ]]; then
        print_pass "Makefile não recria o link desnecessariamente."
    else
        print_fail "Makefile parece recriar o link desnecessariamente."
    fi
}

run_basic_tests() {
    print_header "Parte 2: Comandos, Argumentos, Aspas e Variáveis"
    # Comandos simples
    run_test "Comando simples com caminho absoluto" "/bin/ls"
    run_test "Comando vazio (apenas Enter)" ""
    run_test "Comando com apenas espaços" "   "
    # Argumentos
    run_test "Comando com argumentos" "/bin/ls -l -a"
    # Aspas
    run_test "Aspas duplas com espaços" "echo \"hello      world\""
    run_test "Aspas simples não interpreta variável" "echo '\$USER'"
    run_test "Aspas duplas interpreta variável" "echo \"\$USER\""
    # Variáveis
    run_test "Variável de ambiente inexistente" "echo \$VAR_QUE_NAO_EXISTE_12345"
    run_test "Status de retorno de sucesso" "/bin/true; echo \$?"
    run_test "Status de retorno de falha" "/bin/false; echo \$?"
    run_test "Status de comando inválido" "comandoquenaoexiste; echo \$?"
}

run_builtins_tests() {
    print_header "Parte 3: Built-ins (echo, cd, pwd, export, unset, env, exit)"
    # echo
    run_test "echo com a opção -n" "echo -n ola mundo"
    run_test "echo com múltiplas opções -n" "echo -n -n -n ola"
    # pwd e cd
    run_test "pwd simples" "pwd"
    run_test "cd para / e depois pwd" "cd /; pwd"
    run_test "cd para diretório de teste e depois pwd" "mkdir test_dir; cd test_dir; pwd; cd ..; rmdir test_dir"
    run_test "cd para diretório inexistente" "cd dir_inexistente_123"
    # export, unset, env
    run_test "export de variável" "export TEST_VAR=123; env | grep TEST_VAR"
    run_test "unset de variável" "export TEST_VAR=123; unset TEST_VAR; env | grep TEST_VAR"
    
    # Teste de 'exit' (separado, pois encerra o shell)
    echo -e "\n-> ${YELLOW}Teste:${NC} 'exit com código de status'"
    echo "exit 42" | "$MINISHELL_EXEC" > /dev/null 2>&1
    if [ $? -eq 42 ]; then
        print_pass "'exit 42'"
    else
        print_fail "'exit 42' - Status esperado: 42, Recebido: $?"
    fi
}

run_path_tests() {
    print_header "Parte 4: Caminhos (PATH)"
    run_test "Comando do PATH (ls)" "ls"
    run_test "Comando com PATH unset" "unset PATH; ls"
    run_test "Caminho relativo" "mkdir test_dir; touch test_dir/file; ls ./test_dir; rmdir test_dir"
}

run_redirects_and_pipes_tests() {
    print_header "Parte 5: Redirecionamentos e Pipes"
    # Redirecionamentos
    run_test "Redirect de saída (>)" "echo 'teste' > outfile; cat outfile"
    run_test "Redirect de append (>>)" "echo 'l1' > outfile; echo 'l2' >> outfile; cat outfile"
    run_test "Redirect de entrada (<)" "wc -c < infile"
    run_test "Here document (<<)" "cat << EOF\nhello\nworld\nEOF"
    # Pipes
    run_test "Pipe simples" "ls | wc -l"
    run_test "Pipe múltiplo" "cat infile | grep 'linha' | wc -l"
    # Misturados
    run_test "Pipe com redirect" "cat infile | grep '1' > outfile; cat outfile"
    run_test "Redirect com pipe" "< infile grep '2' | wc -c"
    run_test "Comando inválido no pipe" "ls -l | comando_invalido_xyz | wc -l"
}


run_manual_tests() {
    print_header "Parte 6: Testes Manuais Obrigatórios"
    print_info "Os testes a seguir são interativos e devem ser verificados manualmente:"
    echo
    echo -e "  ${YELLOW}1. SINAIS (Ctrl+C, Ctrl+D, Ctrl+\\):${NC}"
    echo "     - Em um prompt vazio, digite 'sleep 10' e pressione Enter."
    echo "       - Pressione ${YELLOW}Ctrl+C${NC}. O 'sleep' deve ser interrompido e um novo prompt deve aparecer."
    echo "     - Em um prompt vazio, digite 'sleep 10' e pressione Enter."
    echo "       - Pressione ${YELLOW}Ctrl+\\${NC}. O 'sleep' deve ser encerrado (Quit: 3)."
    echo "     - Em um prompt vazio:"
    echo "       - Pressione ${YELLOW}Ctrl+C${NC}. Deve mostrar um novo prompt."
    echo "       - Pressione ${YELLOW}Ctrl+\\${NC}. Não deve fazer nada."
    echo "       - Pressione ${YELLOW}Ctrl+D${NC}. O minishell deve encerrar."
    echo
    echo -e "  ${YELLOW}2. HISTÓRICO:${NC}"
    echo "     - Digite alguns comandos (ex: 'ls', 'pwd', 'echo teste')."
    echo "     - Use as ${YELLOW}setas para cima e para baixo${NC} para navegar entre os comandos digitados."
    echo
    echo -e "  ${YELLOW}3. VAZAMENTOS DE MEMÓRIA:${NC}"
    echo "     - Execute o minishell com Valgrind: ${YELLOW}valgrind --leak-check=full ./minishell${NC}"
    echo "     - Execute vários comandos, incluindo inválidos e com pipes/redirects."
    echo "     - Saia do minishell (com 'exit' ou Ctrl+D) e verifique o sumário do Valgrind."
    echo "     - Deve indicar 'All heap blocks were freed -- no leaks are possible'."
    echo
    echo -e "  ${YELLOW}4. VARIÁVEIS GLOBAIS:${NC}"
    echo "     - Questione os alunos sobre o uso de variáveis globais e suas justificativas."
    echo "     - Verifique o código para garantir que segue a regra do subject (se houver global,"
    echo "       ela deve apenas armazenar o número de um sinal recebido)."
}

# --- Execução Principal ---
trap cleanup EXIT # Garante que a limpeza seja executada ao sair

setup
run_compilation_tests

# Só continua se a compilação funcionar
if [ -x "$MINISHELL_EXEC" ]; then
    run_basic_tests
    run_builtins_tests
    run_path_tests
    run_redirects_and_pipes_tests
    run_manual_tests
fi

print_header "Resultado Final dos Testes Automatizados"
echo -e "Resultados: ${GREEN}${PASSED_TESTS} Passaram${NC}, ${RED}${FAILED_TESTS} Falharam${NC}"
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}Todos os testes automatizados passaram! Prossiga com os testes manuais.${NC}"
else
    echo -e "${RED}Alguns testes automatizados falharam. Revise os logs acima.${NC}"
fi

>>>>>>> 4f4b6a28cd2015f3ef1c1af4348a430f57d77972
