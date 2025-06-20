#!/bin/bash

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

